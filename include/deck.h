/*
* author: Andreas Nilsson
* an224qi@student.lnu.se
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define DECK_SIZE 52
const int NO_DECKS = 4;

char *suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
char *nums[] = {"","","2","3","4","5","6","7","8","9","10","Jack","Queen", "King", "Ace"};
char *rep[] = {};

char *cards[] = {
    "🂲", "🂳", "🂴", "🂵", "🂶", "🂷", "🂸", "🂹", "🂺", "🂻", "🂽", "🂾", "🂡", // Hearts
    "🃂", "🃃", "🃄", "🃅", "🃆", "🃇", "🃈", "🃉", "🃊", "🃋", "🃍", "🃎", "🃁", // Diamonds
    "🃒", "🃓", "🃔", "🃕", "🃖", "🃗", "🃘", "🃙", "🃚", "🃛", "🃝", "🃞", "🃑", // Clubs
    "🂢", "🂣", "🂤", "🂥", "🂦", "🂧", "🂨", "🂩", "🂪", "🂫", "🂭", "🂮", "🂱" // Spades
};

typedef struct Card_t {
  
  int suit;
  int num;
  int unicode;
  int value;

} Card_t;

typedef struct Deck_t {

  int size; // Size of the deck
  Card_t **cards; // Card-array of Card-pointers to card-structs.
  
} Deck_t;

Card_t *createCard(int32_t, int32_t, int32_t, int32_t);
void printDeck(Deck_t *deck);
void resizeDeck(Deck_t *deck, int newSize);
static int randInt(int n);
void shuffle(Deck_t *deck);
Card_t *getRndCard(Deck_t *deck);
void createDeck(Deck_t *deck);
void freeDeck(Deck_t *deck);
void printCard(Card_t *card);
Deck_t *initDeck();

Card_t *createCard(int num, int suit, int unicode, int value) {

  Card_t *card = malloc(sizeof(Card_t));
  
  card->value = value;
  card->num = num;
  card->suit = suit;
  card->unicode = unicode;

  return card;

}

void printDeck(Deck_t *deck) {

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

void resizeDeck(Deck_t *deck, int newSize) {

  deck->cards = realloc(deck->cards, newSize * sizeof(Card_t*));

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

void shuffle(Deck_t *deck) {

    int i, j;
    Card_t *tmp;
 
    for (i = deck->size - 1; i > 0; i--) {
        j = randInt(i + 1);
        tmp = deck->cards[j];
        deck->cards[j] = deck->cards[i];
        deck->cards[i] = tmp;
   }
}

Card_t *getRndCard(Deck_t *deck) {
  
  if(deck->size == 0)
    return NULL;

  int rnd = 0;
  Card_t *card = malloc(sizeof(Card_t));

  rnd = rand() % deck->size;
  
  card->suit = deck->cards[rnd]->suit;
  card->num = deck->cards[rnd]->num;
  card->unicode = deck->cards[rnd]->unicode;
  card->value = deck->cards[rnd]->value;

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

void createDeck(Deck_t *deck) {

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
        deck->cards[deckIndex] = createCard(num, suit, unicode, value);
        ++value;
        ++num;
        ++deckIndex;
        ++unicode;
      }

      value = 2;
      num = 2;
      ++suit;
    }
    unicode = 0;
    ++noDecks;
  }
}

void freeDeck(Deck_t *deck) {
  
  // Frees the deck.
  for (int i = 0; i < deck->size; i++) {
      free(deck->cards[i]);
  }
  
  free(deck->cards);
  free(deck);
}

void printCard(Card_t *card) {
  
    char *suit = suits[card->suit];
    char *num = nums[card->num];
    char *unicode = cards[card->unicode];
    
    printf("Card is %s [%s of %s]\n", unicode, num, suit);
}

Deck_t *initDeck() {

  Deck_t *deck = malloc(sizeof(Deck_t));
  deck->size = DECK_SIZE * NO_DECKS;

  // Create space for 52 Card(s). More specifically size of the deck.
  deck->cards = calloc(deck->size, sizeof(Card_t *));

  // Create cards in the deck.
  createDeck(deck);

  return deck;
}


