#include <stdio.h>

typedef struct Players_t {

  int maxPlayers;
  struct Player_t **player;

} Players_t;

typedef struct Player_t {

  char name[20];
  int32_t tmpMoney;
  int32_t socket;
  struct Hand_t *hand;

} Player_t;

typedef struct Hand_t {

  int8_t size; // Will never be greater than 255
  struct Card_t **cards;

} Hand_t;

void freePlayer(Players_t *players) {

  for(int j=0; j<players->maxPlayers; j++) {

    Player_t *player = players->player[j];
    
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