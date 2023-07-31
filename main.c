/*
* author: Andreas Nilsson
* an224qi@student.lnu.se
*/

#include <string.h>
#include <stdint.h>
#include "include/deck.h"
#include "include/player.h"

int main() {
  
  size_t maxSizePlayerHand = 2;
  size_t maxPlayers = 4;

  // Initialize a deck.
  Deck *deck = initDeck(); 

  // Seed the rng, only once per thread!  
  srand(time(NULL)); 

  // Shuffle the deck
  shuffle(deck);

  // New players list.
  Players *players = malloc(sizeof(Players));
  players->maxPlayers = maxPlayers;
  players->player = calloc(players->maxPlayers, sizeof(Player*));

  // Create #maxPlayers of players.
  for(int i=0; i<players->maxPlayers; i++) {
    Player *player = malloc(sizeof(Player));
    
    // Assign a player-name, max 12 char.
    char name[20] = "Player";
    sprintf(name, "%d",i+1);
    strncpy(player->name, name,sizeof(name));

    // Assign a hand and cards to the hand (Move to own function later)
    Hand *hand = malloc(sizeof(Hand));
    hand->cards = calloc(maxSizePlayerHand, sizeof(Card *));
    hand->size = maxSizePlayerHand;

    for(int32_t i=0;i<maxSizePlayerHand;i++) {  
      Card *card = getRndCard(deck);

      if(card != NULL) {
        hand->cards[i] = card;
      }
    }

    player->hand = hand;
    players->player[i] = player;
  }

  /* ********Test printing players nad other stuff******* */
  Card *crd;
  Player *tmpPlayer;
  Hand *tmpHand;
  
  for(int i=0;i<players->maxPlayers;i++) {
    
    //Create tmp-pointer so we can access player, hand and cards.
    tmpPlayer = players->player[i];
    tmpHand = players->player[i]->hand;
    char *name = players->player[i]->name;

    printf("\n\n---- Player: %s \n",name);

      for(int j = 0;j<tmpHand->size;j++) {
        Card *crd = players->player[i]->hand->cards[j];
        printCard(crd);
      }
  }

  /* Deal a flop, turn and river..*/
  printf("\n\n Flop, turn and river: \n");
  
  for(int i = 0;i<5;i++) {
    crd = getRndCard(deck);
    printCard(crd);
    free(crd);
  }

  /* *************** */

  printf("Remaining cards: %d \n", deck->size);

  // Free resources, no memory leaks plz
  freeDeck(deck);
  freePlayer(players);

  return 0;
}