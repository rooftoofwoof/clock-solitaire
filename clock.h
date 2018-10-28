/*
// Methods
restart()
reset()
// Algorithm
Random number generator (or something else) - ARRANGES 52 cards
DIVIDE into 13 decks of 4 each
 - i.e. 1st to 4th, 5th to 8th...
If middle deck is 1st to 4th, then PICK the 1st card as the 1st card to be flipped
CHECK which number it is
FLIP the top card of the corresponding deck
USE the denomination of that card to FLIP another decks
Continue until win (all decks run dry AND king is the last card shown)
Or if the corresponding deck of the flipped card runs dry
Record win or loss
Loop for specified number of times
Stop Loop
See record
*/
#ifndef CLOCK_H
#define CLOCK_H
#include <iostream>
#include <stack>
#include <vector>

class Clock {
  private:
    int denoms;
    int suits;
    std::stack<int>* allDecks;
    void initialize();
    int rdrand16_step (uint16_t *rand);
    std::vector<std::vector<double>> probArray;

  public:
    Clock(int denoms, int suits);
    ~Clock();
    void reset();
    int getDenoms();
    int getSuits();
    void setDenoms(int denoms);
    void setSuits(int suits);
    bool play(int startDeck);
    bool isAllEmpty();
    int flipTop(int deckNo);
    void playOnce();
    void playIters(long long iters);
    void exportToFile();
};

#endif
