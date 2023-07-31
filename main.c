/*
* author: Andreas Nilsson
* an224qi@student.lnu.se
*/

#include <string.h>
#include <stdint.h>
#include "include/deck.h"
#include "include/player.h"

int main() {
  
  // Initialize a deck.
  Deck *deck = initDeck(); 

  // Seed the rng, only once per thread!  
  srand(time(NULL)); 

  // Shuffle the deck
  shuffle(deck);  

  // New player
  Player *player = malloc(sizeof(Player));
  
  char *name = "isn";
  player->name = name;
 
  // Player
  size_t maxSizePlayerHand = 5;
  
  Hand *hand = malloc(sizeof(Hand));
  hand->cards = calloc(maxSizePlayerHand, sizeof(Card *));
  hand->size = maxSizePlayerHand;

  for(int32_t i=0;i<maxSizePlayerHand;i++) {  
    Card *card = getRndCard(deck);

      if(card != NULL) {
        hand->cards[i] = card;
      }
  }
  
  // Assign the hand to the player
  player->hand = hand;

  printf("Remaining cards: %d \n", deck->size);

  // Free resources, no memory leaks plz
  freeDeck(deck);
  freePlayer(player);

  return 0;
}