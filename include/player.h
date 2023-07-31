#include <stdio.h>

typedef struct Player {

  char *name;
  struct Hand *hand;

} Player;

typedef struct Hand {

  struct Card **cards;

} Hand;