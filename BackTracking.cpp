#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <functional>
using namespace std;

class Puzzle
{
private:
  int A[9][9];

public:
  void input(void);

  void output(void);

  bool horizontal_check(int, int, int);

  bool vertical_check(int, int, int);

  bool cell_check(int, int, int);

  bool assign(int, int, int);

  bool complete(void);

  friend bool backtracking_search(Puzzle &, int, int);
};

void Puzzle::input()
{
  char tmp;
  for (int i = 0; i < 81; i++)
  {
    cin >> tmp;
    {
      if (tmp == '.')
      {
        A[i / 9][i % 9] = 0;
      }
      else
      {
        A[i / 9][i % 9] = tmp - '0';
      }
    }
  }
}

void Puzzle::output()
{
  for (int i = 0; i < 9; i++)
  {
    for (int j = 0; j < 9; j++)
    {
      cout << A[i][j] << " ";
    }
    cout << endl;
  }
}

bool Puzzle::horizontal_check(int x, int y, int n)
{
  for (int i = 0; i < 9; i++)
  {
    if (A[x][i] == n)
    {
      return false;
    }
  }
  return true;
}

bool Puzzle::vertical_check(int x, int y, int n)
{
  for (int i = 0; i < 9; i++)
  {
    if (A[i][y] == n)
    {
      return false;
    }
  }
  return true;
}

bool Puzzle::cell_check(int x, int y, int n)
{
  for (int i = (x / 3) * 3; i < (x / 3) * 3 + 3; i++)
  {
    for (int j = (y / 3) * 3; j < (y / 3) * 3 + 3; j++)
    {
      if (A[i][j] == n)
      {
        return false;
      }
    }
  }
  return true;
}

bool Puzzle::assign(int x, int y, int n)
{
  if (vertical_check(x, y, n) && horizontal_check(x, y, n) && cell_check(x, y, n))
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool Puzzle::complete(void)
{
  for (int i = 0; i < 9; i++)
  {
    for (int j = 0; j < 9; j++)
    {
      if (A[i][j] == 0)
      {
        return false;
      }
    }
  }
  return true;
}

bool backtracking_search(Puzzle &puzzle, int i, int j)
{
  if (i == 8 && j == 9)
  {
    if (puzzle.complete())
    {
      return true;
    }
    return false;
  }

  if (j == 9)
  {
    i++;
    j = 0;
  }

  if (puzzle.A[i][j] != 0)
  {
    return backtracking_search(puzzle, i, j + 1);
  }

  for (int next = 1; next <= 9; next++)
  {
    if (puzzle.assign(i, j, next))
    {
      puzzle.A[i][j] = next;
      if (backtracking_search(puzzle, i, j + 1))
        return true;
      puzzle.A[i][j] = 0;
    }
  }
  return false;
}

int main(void)
{
  Puzzle x;
  x.input();

  cout << "The problem is below" << endl;

  x.output();

  cout << endl;

  if (backtracking_search(x, 0, 0))
  {
    cout << "The answer is below" << endl;
    x.output();
  }
  else
  {
    cout << "fail";
  }
  return 0;
}
