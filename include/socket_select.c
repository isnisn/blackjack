/*
*   Socket example using fd and select
*   author: Andreas Nilsson
*/
#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros


#define PORT 8888
#define MAX_CLIENTS 20

typedef struct Client {

    // Socket to the client
    unsigned int socket;

    //Nickname
    char name[15];
    
    //Ip
    struct in_addr sockaddr_in;


} Client;

typedef struct Clients {

    unsigned int max_clients;

    // List of Client
    Client **client;

} Clients;

void add_client(Clients *clients, char *name, int socket, struct in_addr sockaddr_in) {
    
    int c_index = 0;

    // Find next available slot
    for(int i=0; i<clients->max_clients;i++) {
        if(!clients->client[i]) {
            c_index = i;
            break;
        }
    }

    Client *c_one = malloc(sizeof(Client));
    c_one->socket = socket;
    c_one->sockaddr_in = sockaddr_in;
    strncpy(c_one->name, name, sizeof(name)+1);

    // Add client to clients
    clients->client[c_index] = c_one;
}

void print_clients(Clients *clients) {
    // Print a list of all connected clients.
    for(int i=0;i<clients->max_clients;i++) {
        if(clients->client[i]) {
            printf("Name: %s got socket: %d\n", clients->client[i]->name, clients->client[i]->socket);
        }
    }
}

/* Send a message to all clients */
void send_to_all(Clients *clients,char *buffer, char *sender) {

    for(int i=0;i<MAX_CLIENTS;i++) {
        if(clients->client[i]) {
            
            send(clients->client[i]->socket, buffer, strlen(buffer),0);

        }
    }
}

int main(int argc , char *argv[])
{
    // New client-list
    Clients *clients = malloc(sizeof(Clients)); 
    clients->max_clients = MAX_CLIENTS;
    clients->client = calloc(clients->max_clients, sizeof(Client *));

    print_clients(clients);

    /* --------------- */
    struct sockaddr_in address;
	
    // Type of socket created
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

    /* --------------- */

	char *opt = "1";
	char buffer[1025]; //data buffer of 1K
    char *msg = "Whats your nickname? > ";    
    char buf[1025];

    int master_socket , addrlen , new_socket, activity , sd;
	int max_sd;

	//set of socket descriptors
	fd_set readfds;
	char *message = "Welcome to cardservice! \r\n";
	
		
	//create a master socket
	if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	
	//set master socket to allow multiple connections ,
	//this is just a good habit, it will work without this
	if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, &opt,sizeof(opt)) < 0 ) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	
	//bind the socket to localhost port 8888
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0) {

		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	printf("Listening on port %d \n", PORT);
		
	//try to specify maximum of 3 pending connections for the master socket
	if (listen(master_socket, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
		
	//accept the incoming connection
	addrlen = sizeof(address);
	printf("Waiting for connections ...\n");
		
	while(1)
	{
		//clear the socket set
		FD_ZERO(&readfds);
	
		//add master socket to set
		FD_SET(master_socket, &readfds);
		max_sd = master_socket;
		
        for(int i=0; i<clients->max_clients;i++) {
            if(clients->client[i]) {
                sd = clients->client[i]->socket;

                FD_SET(sd, &readfds);

                if(sd > max_sd)
                    max_sd = sd;
            }
        }

        // Wait 4-ever, timeout = NULL
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
	
		if ((activity < 0) && (errno!=EINTR)) {
			printf("select error");
		}
			
		//If something happened on the master socket ,
		//then its an incoming connection
		if (FD_ISSET(master_socket, &readfds)) {
			if ((new_socket = accept(master_socket,(struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
				perror("accept");
				exit(EXIT_FAILURE);
			}
			
			//inform user of socket number - used in send and receive commands
			printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
		
			//send new connection greeting message
			if( send(new_socket, message, strlen(message), 0) != strlen(message) )
			{
				perror("send");
			}
				
			printf("Welcome message sent successfully\n");
			
            send(new_socket, msg, strlen(msg),0);
            ssize_t nbytes = read(new_socket,buf,1024);
            buf[nbytes - (sizeof(int) / 2)] = '\0';
        
            add_client(clients, buf, new_socket, address.sin_addr);
            //memset(buf,0,sizeof(int));

			print_clients(clients);
            
		}

			
		//else its some IO operation on some other socket
		for (int i = 0; i < MAX_CLIENTS; i++)
		{
            if(clients->client[i]) {
			    sd = clients->client[i]->socket;
            } else { 
                continue;
            }

			if (FD_ISSET( sd , &readfds))
			{
				//Check if it was for closing , and also read the
				//incoming message
                ssize_t valread;

				if ((valread = read( sd , buffer, 1024)) == 0)
				{
					//Somebody disconnected , get his details and print
					getpeername(sd , (struct sockaddr*)&address , \
						(socklen_t*)&addrlen);
					printf("Host disconnected , ip %s , port %d \n" ,inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
						
					//Close the socket and mark as 0 in list for reuse
					close(sd);
					clients->client[i] = NULL;

				} else {
					//set the string terminating NULL byte on the end
					//of the data read
					buffer[valread] = '\0';
                    send_to_all(clients, buffer, clients->client[i]->name);
				}
			}
		}
	}
		
	return 0;
}
