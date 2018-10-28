#include "clock.h"
#include <iostream>
#include <climits>
using namespace std;

int main() {
  int denom = 0;
  int suit = 0;
  int start = 0;
  long long iters = 100000;
  char skip = '\0';

    cout << "*********************" << endl;
    cout << "   CLOCK SOLITAIRE   " << endl;
    cout << "*********************" << endl;
    do
    {
  	  cin.clear();
  	  cout << "Please enter number of denominations >";
  	  cin >> denom;
    } while (denom < 1 || denom > INT_MAX);
    do
    {
  	  cin.clear();
  	  cout << endl << "Please enter number of suits >";
  	  cin >> suit;
    } while (suit < 1 || suit > INT_MAX);
    do
    {
  	  cin.clear();
  	  cout << endl << "Please enter the denomination of the starting deck >";
  	  cin >> start;
    } while (start < 1 || start > denom);
    do
    {
  	  cin.clear();
  	  cout << endl << "How many games to play? >";
  	  cin >> iters;
    } while (iters < 1 || iters > LLONG_MAX);

    Clock atomic(denom, suit);
    atomic.playIters(iters);
    atomic.exportToFile();
    cout << "To EXIT: Please type anything and press 'Enter' >";
    cin >> skip;

  return 0;
}
