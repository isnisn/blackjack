/*
* author: Andreas Nilsson
* an224qi@student.lnu.se
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define DECK_SIZE 52
#define NO_DECKS 1

char *suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
char *nums[] = {"","","2","3","4","5","6","7","8","9","10","Jack","Queen", "King", "Ace"};
char *rep[] = {};

char *cards[] = {
    "🂲", "🂳", "🂴", "🂵", "🂶", "🂷", "🂸", "🂹", "🂺", "🂻", "🂽", "🂾", "🂡", // Hearts
    "🃂", "🃃", "🃄", "🃅", "🃆", "🃇", "🃈", "🃉", "🃊", "🃋", "🃍", "🃎", "🃁", // Diamonds
    "🃒", "🃓", "🃔", "🃕", "🃖", "🃗", "🃘", "🃙", "🃚", "🃛", "🃝", "🃞", "🃑", // Clubs
    "🂢", "🂣", "🂤", "🂥", "🂦", "🂧", "🂨", "🂩", "🂪", "🂫", "🂭", "🂮", "🂱" // Spades
};

typedef struct Card {
  
  int suit;
  int num;
  int unicode;
  int value;

} Card;

typedef struct Deck {

  int size; // Size of the deck
  Card **cards; // Card-array of Card-pointers to card-structs.
  
} Deck;

Card *createCard(Deck *deck, int, int, int, int);
void printDeck(Deck *deck);
void resizeDeck(Deck *deck, int newSize);
static int randInt(int n);
void shuffle(Deck *deck);
Card *getRndCard(Deck *deck);
void createDeck(Deck *deck);
void freeDeck(Deck *deck);
void printCard(Card *card);
Deck *initDeck();

Card *createCard(Deck *deck, int num, int suit, int unicode, int value) {

  Card *card = malloc(sizeof(Card));
  
  card->value = value;
  card->num = num;
  card->suit = suit;
  card->unicode = unicode;

  return card;

}

void printDeck(Deck *deck) {

char *suit, *num, *unicode;
int value;


  for(int i=0;i<deck->size;i++) {

    suit = suits[deck->cards[i]->suit];
    num = nums[deck->cards[i]->num];
    unicode = cards[deck->cards[i]->unicode];
    value = deck->cards[i]->value;

    printf("Card[%d] is %s of %s -> %s num val: %d\n",i, num, suit, unicode, value);
  }
}

void resizeDeck(Deck *deck, int newSize) {

  deck->cards = realloc(deck->cards, newSize * sizeof(Card *));

}

static int randInt(int n) {

    int limit = RAND_MAX - RAND_MAX % n;
    int rnd;

    do {
        rnd = rand();
    } 
    while (rnd >= limit);

    return rnd % n;
}

void shuffle(Deck *deck) {
  
    int i, j;
    Card *tmp;
 
    for (i = deck->size - 1; i > 0; i--) {
        j = randInt(i + 1);
        tmp = deck->cards[j];
        deck->cards[j] = deck->cards[i];
        deck->cards[i] = tmp;
   }
}

Card *getRndCard(Deck *deck) {
  
  if(deck->size == 0)
    return NULL;

  int rnd = 0;
  Card *card = malloc(sizeof(Card *));

  rnd = rand() % deck->size;
  
  card->suit = deck->cards[rnd]->suit;
  card->num = deck->cards[rnd]->num;
  card->unicode = deck->cards[rnd]->unicode;
  card->value = deck->cards[rnd]->value;

  printf("Dealing: ");
  printCard(card);

  free(deck->cards[rnd]);
  deck->cards[rnd] = NULL;

  // Shift the remaining cards to the left to fill the gap
  for (int i = rnd; i < deck->size - 1; i++) {
      deck->cards[i] = deck->cards[i + 1];
  }

  // Resize deck
  deck->size -= 1;

  return card;

}

void createDeck(Deck *deck) {

  int num = 2;
  int deckIndex = 0;
  int noDecks = 1;
  int unicode = 0;
  int value = 2;

  while(noDecks <= NO_DECKS) {

    int suit = 0;

    for(int j=0;j<4;j++) {
    
    // Create 13 cards 2 - 14 with suit.
      for(int i=0;i<13;i++) {
        deck->cards[deckIndex] = createCard(deck, num, suit, unicode, value);
        ++value;
        ++num;
        ++deckIndex;
        ++unicode;
      }
      value = 2;
      num = 2;
      ++suit;
    }

    ++noDecks;
  }
}

/* Juhuuu some tidying up */
void freeDeck(Deck *deck) {
  
  // Frees the deck.
  for (int i = 0; i < deck->size; i++)
  {
      Card *card = deck->cards[i];
      free(card);
  }
  
  free(deck->cards);
  free(deck);
}

void printCard(Card *card) {
  
    char *suit = suits[card->suit];
    char *num = nums[card->num];
    char *unicode = cards[card->unicode];
    int value = card->value;

    printf("Card is %s [%s of %s] num val: %d\n", unicode, num, suit, value);
}

Deck *initDeck() {

  Deck *deck = malloc(sizeof(Deck *));
  deck->size = DECK_SIZE * NO_DECKS;

  // Create space for 52 Card(s). More specifically size of the deck.
  deck->cards = calloc(deck->size, sizeof(Card *));

  // Create cards in the deck.
  createDeck(deck);

  return deck;
}


