#include "clock.h"
#include <iostream>
#include <iomanip>
#include <random>
#include <fstream>
#include <ctime>
#include <string>
#include <chrono>
using namespace std;

/*** IMPORTANT NOTES ***/
// 1. Throughout the code, comments might say 1 to 52 but it really means 1 to
//    the total number of cards in the user-specified deck. The same applies to
//    other special numbers for a traditional deck, e.g. 13 => number of denoms.
// 2. Throughout the code, each individual card is assigned a unique number.
//    e.g. For a traditional 1 to 52 deck, 1 means a denomination-1 card and
//    13 a King card, 14 would represent a denomination 1, and 26 another King
//    ... but with a different suit and so on.

Clock::Clock(int den, int sui) : denoms(den), suits(sui)
{
  vector<int> selectionDeck;          // The deck that is to be selected from by the Random Number Generator.
  int deckMax = denoms * suits;       // Maximum number of cards in a deck.
  int leftoverDeckMax = deckMax - 1;  // RNG will choose a card between 1 & 52, then 1 & 51, then 1 & 50...49...48...down to 1. This variable stores the decreasing number.
  int array_len = denoms;             // Array to store information for each restart.
  int array_wid = 4;                  // 4 pieces of information is needed for each restart, i.e. Win Count, Restart Occurences, Cumulative Cards Left, and Probability

  // Random number generator code from:
  // https://stackoverflow.com/questions/5008804/generating-random-integer-from-a-range .
  random_device rd;
  mt19937 rng(rd());
  uniform_int_distribution<int> uni(0,leftoverDeckMax);
  auto randNo = uni(rng);

    // Resize vector to store data for each restart.
    probArray.resize(array_wid);
    for (int i = 0; i < array_wid; i++)
    {
      probArray[i].resize(array_len);
    }

    // Create array of decks. Each deck is a stack of ints.
    // Deck 0 is the Main Deck with 52 cards. Deck 1 to Deck {denoms} will be
    // the actual decks with which the game is played.
    allDecks = new stack<int>[denoms + 1];
    if (allDecks == nullptr)
    {
      cout << "ERROR CREATING DECK ARRAY" << endl;
      return;
    }

    // Intialize selectionDeck to a brand new ordered deck 1 to 52,
    // which will be the deck that is selected from
    // to populate the Main Deck.
    for (int i = 0; i < deckMax; i++)
    {
        selectionDeck.push_back(i+1);
    }
    // Populate main deck of 52 cards.
    for (int i = 0; i < deckMax; i++)
    {
      // leftoverDeckMax decrements with each selection
      // Choose 1 from 52, then 1 from 51, 1 from 50...
      uniform_int_distribution<int> uni(0,leftoverDeckMax);
      randNo = uni(rng);
      allDecks[0].push(selectionDeck[randNo]);
      selectionDeck.erase(selectionDeck.begin() + randNo);
      leftoverDeckMax--;
    }
    // Divide ordered main deck into {denoms} smaller decks.
    for (int j = 0; j < denoms; j++)
    {
      for (int k = 0; k < suits; k++)
      {
        if (!allDecks[0].empty())
        {
          allDecks[j+1].push(allDecks[0].top());
          allDecks[0].pop();
        }
      }
    }
}

Clock::~Clock() {
  delete[] allDecks;
}

void Clock::reset() {
  delete[] allDecks;
  initialize();
}

void Clock::initialize() {
  vector<int> selectionDeck;          // The deck that is to be selected from by the Random Number Generator.
  int array_len = denoms;             // Array to store information for each restart.
  int array_wid = 4;                  // 4 pieces of information is needed for each restart, i.e. Win Count, Restart Occurences, Cumulative Cards Left, and Probability
  int deckMax = denoms * suits;       // Maximum number of cards in a deck.
  int leftoverDeckMax = deckMax - 1;  // RNG will choose a card between 1 & 52, then 1 & 51, then 1 & 50...49...48...down to 1. This variable stores the decreasing number.
  // Random number generator code from:
  // https://stackoverflow.com/questions/5008804/generating-random-integer-from-a-range .
  random_device rd;
  mt19937 rng(rd());
  uniform_int_distribution<int> uni(0,leftoverDeckMax);
  auto randNo = uni(rng);

    // Resize vector to store data for each restart.
    probArray.resize(array_wid);
    for (int i = 0; i < array_wid; i++)
    {
      probArray[i].resize(array_len);
    }

    // Create array of decks. Each deck is a stack of ints.
    // Deck 0 is the Main Deck with 52 cards. Deck 1 to Deck {denoms} will be
    // the actual decks with which the game is played.
    allDecks = new stack<int>[denoms + 1];
    if (allDecks == nullptr)
    {
      cout << "ERROR CREATING DECK ARRAY" << endl;
      return;
    }

    // Intialize selectionDeck to a brand new ordered deck 1 to 52,
    // which will be the deck that is selected from
    // to populate the Main Deck.
    for (int i = 0; i < deckMax; i++)
    {
        selectionDeck.push_back(i+1);
    }
    // Populate main deck of 52 cards.
    for (int i = 0; i < deckMax; i++)
    {
      // leftoverDeckMax decrements with each selection
      // Choose 1 from 52, then 1 from 51, 1 from 50...
      uniform_int_distribution<int> uni(0,leftoverDeckMax);
      randNo = uni(rng);
      allDecks[0].push(selectionDeck[randNo]);
      selectionDeck.erase(selectionDeck.begin() + randNo);
      leftoverDeckMax--;
    }
    // Divide ordered main deck into {denoms} smaller decks.
    for (int j = 0; j < denoms; j++)
    {
      for (int k = 0; k < suits; k++)
      {
        if (!allDecks[0].empty())
        {
          allDecks[j+1].push(allDecks[0].top());
          allDecks[0].pop();
        }
      }
    }
}

void Clock::setDenoms(int d)
{
  denoms = d;
}

void Clock::setSuits(int s)
{
  denoms = s;
}

int Clock::getDenoms()
{
  return denoms;
}

int Clock::getSuits()
{
  return suits;
}

// Flip the top card of a certain deck.
// Returns the number that represents the card.
// deckNo represents index of allDecks[].
int Clock::flipTop(int deckNo)
{
  int storedNo = 0;
  int flipDenom = -1;

    storedNo = allDecks[deckNo].top();
    allDecks[deckNo].pop();
    // Translate Raw Card Identification Number into denomination.
    // Raw Card Identification Number
    // i.e. 1 is 1, 13 is King, 14 is 1, 26 is King, 28 is 2, and so on.
    // Denomination
    // i.e. 1 to 13 only.
    flipDenom = storedNo % denoms;
    if (flipDenom == 0)
    {
      flipDenom = denoms;
    }

  return flipDenom;
}

// Plays either a game or a restart.
bool Clock::play(int startDeck)
{
  int flipDeck = startDeck;

    // While no decks which was indicated to be flipped is empty
    // i.e. Every deck that is to be flipped has at least a card left to be flipped.
    while (!allDecks[flipDeck].empty())
    {
      flipDeck = flipTop(flipDeck);
    }

    // You win when you are at the end of the game (all decks empty)
    // and the last denomination indicated to be flipped is the starting denomination.
    if (flipDeck == startDeck && isAllEmpty())
    {
      return true;
    }

  return false;
}

// Check whether the decks that are being played (Decks 1 to {denoms}) are empty.
bool Clock::isAllEmpty()
{
  for (int i = 1; i <= denoms; i++)
  {
    if (!allDecks[i].empty())
    {
      return false;
    }
  }
  return true;
}

// Iterates through a total of {iters} playOnce() calls.
// Prints time taken to iterate through'em all.
// Then prints final results.
void Clock::playIters(long long iters)
{
  cout << "Nope, you sure don't have a gambling problem..." << endl;
  // Starts clock that finds elapsed time
  chrono::steady_clock::time_point begin = chrono::steady_clock::now();
  for (long long i = 0; i < iters; i++)
  {
    playOnce();
    reset();
  }
  chrono::steady_clock::time_point end= chrono::steady_clock::now();
  int64_t minutes = chrono::duration_cast<chrono::minutes>(end - begin).count();
  int64_t seconds = chrono::duration_cast<chrono::seconds>(end - begin).count();
  seconds = seconds - minutes * 60;
  for (int j = 0; j < denoms; j++)
  {
    // Probability of winning a game with {j} restarts.
    // Probability = Win Count for {j} restarts / Total Iterations
    probArray[3][j] = probArray[0][j] / iters;
    // Calculates average cards left for {j} restarts.
    // Averages over all occurences of that restart
    // i.e. Restarts that win (0 cards left) and those that lose (>0 cards left)
    probArray[2][j] = probArray[2][j] / probArray[1][j];
    cout << endl << j << " RESTART(S)" << endl;
    cout << "_____________" << endl;
    cout << "WIN COUNT: "  << probArray[0][j] << " || ";
    cout << "RESTART OCCURENCES: "  << probArray[1][j] << " || ";
    cout << setprecision(5) << "AVG. CARDS LEFT: "  << probArray[2][j] << " || ";
    cout << setprecision(15) << "PROBABILITY: "  << probArray[3][j] << endl << endl;
  }
  cout << endl << "RAN " << iters << " ITERATIONS" << endl;
  cout << "in " << minutes << " MIN(S) " << seconds << " SEC(S)" << endl << endl;
}

void Clock::playOnce()
{
  bool win = false;
  int restarts = -1;
  int cardsLeft = 0;
  int smallestDeck = 0;

    restarts = -1;
    while (!win && restarts < denoms - 1)
    {
      restarts++;
      probArray[1][restarts] += 1.0;        // Restart occurence counts are doubles, just because the array to store it is a double array.

      if (restarts == 0) {
        win = play(denoms);
      }
      else    // else for the games that did not win with 0 restarts...
      {       // We choose to restart the game with the remaining populated deck with the smallest denomination.
        smallestDeck = 0;
        do
        {
          smallestDeck++;
        } while (allDecks[smallestDeck].empty() && smallestDeck < denoms);
        win = play(smallestDeck);
      }

      if (!win)
      {
        cardsLeft = 0;
        for (int i = 1; i <= denoms; i++)
        {
          cardsLeft = cardsLeft + allDecks[i].size();
        }
        // Accumulate the cards-left counts to be processed at the end of...
        // ...playIters() to become average cards left.
        probArray[2][restarts] = probArray[2][restarts] + cardsLeft;
      }
    }
    if (win)
    {
      probArray[0][restarts] += 1.0;
    }
}

// Function to obtain local time (for dynamic file name purposes).
// Credits to Galik
// https://stackoverflow.com/questions/38034033/c-localtime-this-function-or-variable-may-be-unsafe
inline tm localtime_xp(time_t timer)
{
	tm now{};
#if defined(__unix__)
	localtime_r(&timer, &now);
#elif defined(_MSC_VER)
	localtime_s(&now, &timer);
#else
	static mutex mtx;
	lock_guard<mutex> lock(mtx);
	now = *localtime(&timer);
#endif
	return now;
}

// Exports results to CSV file
void Clock::exportToFile()
{
  ofstream outFile;
  time_t t = time(0);
  struct tm now = localtime_xp(t);
  char buffer[20];
  strftime (buffer,20,"%b%d_%H-%M-%S",&now);
  string filename;

    filename = string("solitaire_") + string(buffer) + string(".csv");
    outFile.open(filename, ios::app);
    if (outFile)
    {
      outFile << "NO. OF RESTARTS, WIN COUNT, RESTART OCCURENCES, AVG. CARDS LEFT (wins and losses), PROB (wins over total iterations)" << endl;
      for (int j = 0; j < denoms; j++)
      {
        outFile << j << ",";
        outFile << probArray[0][j] << ",";
        outFile << probArray[1][j] << ",";
        outFile << setprecision(5) << probArray[2][j] << ",";
        outFile << setprecision(15) << probArray[3][j] << endl;
      }
      cout << endl << endl << "Successfully exported to CSV file: " << filename << endl;
      cout << "You can find the file in the same folder as this application." << endl << endl << endl;
      outFile.close();
    }
    else
    {
      cout << "Error creating export file.";
    }
}
