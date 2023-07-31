#include <stdio.h>

typedef struct Players {

  int32_t maxPlayers;
  struct Player **player;

} Players;

typedef struct Player {

  char name[20];
  struct Hand *hand;

} Player;

typedef struct Hand {

  int size;
  struct Card **cards;

} Hand;

void freePlayer(Players *players) {

  for(int j=0; j<players->maxPlayers; j++) {

    Player *player = players->player[j];
    
    for(int i=0; i<player->hand->size; i++) {
      free(player->hand->cards[i]);
    }

    free(player->hand->cards);
    free(player->hand);
    free(player);
  }

  free(players->player);
  free(players);

}