/*
* Lets create a simple game of blackjack.
*
* author: Andreas Nilsson
* an224qi@student.lnu.se
*/

#include <string.h>
#include <time.h>
#include <stdint.h>
#include "include/deck.h"
#include "include/player.h"

typedef struct Dealer_t {
  Hand_t *hand;
  int wins;
  int losses;

} Dealer_t;

Dealer_t *initDealer() {
  return malloc(sizeof(Dealer_t));
}

int main() {

  printf("++++++++++ Welcome to Blackjack! ++++++++++++ \n Starting game, good luck! \n\n\n");

  size_t maxInitialCards = 2;
  size_t maxCards = 10;

  size_t maxPlayers = 4;

  // Initialize a deck and a dealer.
  Deck_t *deck = initDeck(); 
  shuffle(deck);

  Dealer_t *dealer = initDealer(); 

  // Seed the rng, only once per thread!  
  srand(time(NULL)); 

  // Assign hand to dealer
  Hand_t *hand = malloc(sizeof(Hand_t));
  hand->cards = calloc(maxCards, sizeof(Card_t *));
  hand->size = maxInitialCards;

  for(size_t i=0;i<maxInitialCards;i++) {  
    Card_t *card = getRndCard(deck);

    if(card != NULL) {
      hand->cards[i] = card;
    }
  }

  dealer->hand = hand;

  printf("Dealer shows:");
  printCard(hand->cards[0]);

  // New players list.
  Players_t *players = malloc(sizeof(Players_t));
  players->maxPlayers = maxPlayers;
  players->player = calloc(players->maxPlayers, sizeof(Player_t*));

  // Create #maxPlayers of players.
  for(int i=0; i<players->maxPlayers; i++) {
    Player_t *player = malloc(sizeof(Player_t));
    
    // Assign a player-name, max 12 char.
    char name[20] = "Player";
    sprintf(name, "%d",i+1);
    strncpy(player->name, name,sizeof(name));

    // Assign a hand and cards to the hand (Move to own function later)
    Hand_t *hand = malloc(sizeof(Hand_t));
    hand->cards = calloc(maxCards, sizeof(Card_t *));
    hand->size = maxInitialCards;

    // Dealing n cards.
    for(size_t i=0;i<maxInitialCards;i++) {  
      Card_t *card = getRndCard(deck);

      if(card != NULL) {
        hand->cards[i] = card;
      }
    }

    player->hand = hand;
    players->player[i] = player;
  }

  /* ********Test printing players, game logic nad other stuff******* */
  Card_t *crd;
  Player_t *tmpPlayer;
  Hand_t *tmpHand;
  
  for(int i=0;i<players->maxPlayers;i++) {

    int choice = 0;
    char *name = players->player[i]->name;

    tmpPlayer = players->player[i];
    tmpHand = players->player[i]->hand;
    
    printf("\n\n---- Player: %s \n",name);

    for(int j = 0;j<tmpHand->size;j++) {
      crd = players->player[i]->hand->cards[j];
      printCard(crd);
    }

    // Check for blackjack.
  
    int cardCount = 0;
    int cardSum = 0;
    
    // Calculate players initial sum of the cards.
    for(size_t j = 0; j<maxInitialCards+cardCount;j++)
        cardSum += tmpHand->cards[j]->value > 10 ? 10 : tmpHand->cards[j]->value;

    while(choice != 2 && cardSum < 22) {
      
      printf("-- Cardsum is: %d \n", cardSum);
      printf("Do you want to 1 -> hit or 2 -> stand? (1 or 2) > ");
      scanf("%d",&choice);

      switch (choice)
      {
      case 1:
        Card_t *card = getRndCard(deck);
        
        tmpHand->cards[cardCount+maxInitialCards] = card;
        tmpHand->size += 1;

        cardSum += card->value > 10 ? 10 : card->value;
        printCard(card);

        // Bust?
        if(cardSum > 21) {
          printf("xxxxx Sorry, over 21! \n");
        }

        ++cardCount;

        break;
      
      case 2:
    
        break;
      }
    }

  }

  // All players have made their choices, now on to the dealer.

  printf("\n\n Dealer: \n");
  
  int8_t sum = 0;
  hand = dealer->hand;

  // First sum up the two cards dealer has on hand.
  for(size_t i = 0; i < maxInitialCards; ++i) {
    if(hand->cards[i]->value) {
      sum += hand->cards[i]->value > 10 ? 10 : hand->cards[i]->value;
    }
    printCard(hand->cards[i]);
  }
  printf("Dealer has initially: %d \n\n", sum);

  while(sum < 16) {

    crd = getRndCard(deck);

    if(crd->value > 10)
    sum += 10;
      else
    sum += crd->value;
    
    printCard(crd);
    printf("Dealer has: %d \n\n", sum);
    sleep(1);
    free(crd);
  
  }

  // Dealer is fat?
  if(sum > 21) {
    printf("Dealer bust... :) \n)");
  }


  for(int i=0; i<maxPlayers; i++) {

    tmpPlayer = players->player[i];

  }

  /* *************** */

  printf("Remaining cards: %d \n", deck->size);

  // Free resources, no memory leaks plz
  freeDeck(deck);
  freePlayer(players);

  return 0;
}