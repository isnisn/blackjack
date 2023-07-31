#include <stdio.h>

typedef struct Player {

  char *name;
  struct Hand *hand;

} Player;

typedef struct Hand {

  int size;
  struct Card **cards;

} Hand;

void freePlayer(Player *player) {

  // Frees the playerresources
  for(int i=0; i<player->hand->size; i++) {
    free(player->hand->cards[i]);
  }
  free(player->hand->cards);
  free(player->hand);
  free(player);

}