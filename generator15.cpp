#include <iostream>
#include <random>
#include <ctime>
#include <cstdlib>
using namespace std;

#define B (int)0

class Board
{
private:
  int X[16];
  int blank;

public:
  Board();
  void show();
  void shuffle(int);
};

Board::Board(void)
{
  for (int i = 0; i < 16; i++)
    X[i] = i;
  blank = 0;
}

void Board::show(void)
{
  for (int i = 0; i < 16; i++)
  {
    if (i % 4 == 0)
      cout << endl;
    cout << X[i];

    if ((i + 1) % 4 != 0)
    {

      if (X[i] >= 10)
      {
        cout << " ";
      }
      else
      {
        cout << "  ";
      }
    }
  }
}

void Board::shuffle(int tim)
{
  srand((unsigned int)time(NULL) + tim);

  int N = rand() % 200 + 500;

  int a;
  int n;

  int ax;
  int ay;
  int tmp;

  const int dx[] = {0, 1, 0, -1};
  const int dy[] = {1, 0, -1, 0};

  while (--N >= 0)
  {
    while (1)
    {
      a = blank;

      n = rand() % 4;

      ax = a / 4 + dx[n];
      ay = a % 4 + dy[n];

      if (ax < 0 || ax >= 4 || ay < 0 || ay >= 4)
      {
        continue;
      }
      else
      {
        a = ax * 4 + ay;

        swap(X[a], X[blank]);

        blank = a;
        break;
      }
    }
  }

  return;
}

int main(void)
{
  int how_many_boards;

  cout << "何枚のボードを作成しますか？\n";
  cin >> how_many_boards;

  for (int t = 1; t <= how_many_boards; t++)
  {
    Board board;

    board.shuffle(t);

    cout << "Board # " << t << endl;

    board.show();

    cout << endl;
  }
  return 0;
}
