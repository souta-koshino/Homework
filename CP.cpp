#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
using namespace std;

class Puzzle
{
private:
  int A[9][9];
  vector<vector<int>> DOMAIN;

public:
  void input(void);

  void output(void);

  bool horizontal_check(int, int, int);

  bool vertical_check(int, int, int);

  bool cell_check(int, int, int);

  bool assign(int, int, int);

  bool complete(void);

  friend bool MAC(Puzzle, int);

  friend bool REVISE(Puzzle &, int, int);

  friend bool backtracking_search(Puzzle &, int);

  void update(void);
};

class Constraints
{
private:
  int C[81][81];

public:
  Constraints()
  {
    for (int i = 0; i < 81; i++)
    {
      for (int j = 0; j < 81; j++)
      {
        if (i / 9 == j / 9)
        {
          C[i][j] = 1;
        }
        else if (i % 9 == j % 9)
        {
          C[i][j] = 1;
        }
        else if (i % 9 / 3 == j % 9 / 3 && i / 9 / 3 == j / 9 / 3)
        {
          C[i][j] = 1;
        }
        else
        {
          C[i][j] = 0;
        }
      }
    }
  }

  bool check(int i, int j)
  {
    if (C[i][j] == 1)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
} constraints;

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

  DOMAIN.resize(81);

  for (int i = 0; i < 81; i++)
  {
    if (A[i / 9][i % 9] == 0)
    {
      for (int n = 1; n <= 9; n++)
      {
        if (assign(i / 9, i % 9, n))
          DOMAIN[i].push_back(n);
      }
    }
  }
}

void Puzzle::update(void)
{
  for (int i = 0; i < 81; i++)
  {
    if (A[i / 9][i % 9] == 0)
    {
      for (int n = 1; n <= 9; n++)
      {
        if (assign(i / 9, i % 9, n))
          DOMAIN[i].push_back(n);
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
  cout << endl;
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

bool MAC(Puzzle puzzle, int i)
{
  queue<pair<int, int>> Q;

  for (int j = 0; j < 81; j++)
  {
    if (constraints.check(i, j) == 1 && puzzle.A[j / 9][j % 9] == 0)
    {
      Q.push(make_pair(j, i));
    }
  }

  while (!Q.empty())
  {
    pair<int, int> X = Q.front();
    Q.pop();

    int i = X.first;
    int j = X.second;

    if (REVISE(puzzle, i, j))
    {
      if (puzzle.DOMAIN[i].size() == 0)
      {
        return false;
      }
      for (int k = 0; k < 81; k++)
      {
        if (constraints.check(i, k) == 1 && k != j)
          Q.push(make_pair(k, i));
      }
    }
  }
  return true;
}

bool REVISE(Puzzle &puzzle, int i, int j)
{
  bool revised = false;

  for (auto ite : puzzle.DOMAIN[i])
  {
    bool all = true;

    for (auto iter : puzzle.DOMAIN[j])
    {
      if (iter != ite)
        all = false;
    }

    if (all)
    {
      puzzle.DOMAIN[i].erase(remove(puzzle.DOMAIN[i].begin(), puzzle.DOMAIN[i].end(), ite), puzzle.DOMAIN[i].end());
      revised = true;
    }
  }
  return revised;
}

bool backtracking_search(Puzzle &puzzle, int i)
{
  if (i == 81)
  {
    if (puzzle.complete())
    {
      return true;
    }
    return false;
  }

  if (puzzle.A[i / 9][i % 9] != 0)
  {
    return backtracking_search(puzzle, i + 1);
  }

  for (auto &it : puzzle.DOMAIN[i])
  {
    if (puzzle.assign(i / 9, i % 9, it))
    {
      puzzle.A[i / 9][i % 9] = it;

      if (MAC(puzzle, i))
      {
        if (backtracking_search(puzzle, i + 1))
        {
          return true;
        }
      }

      puzzle.A[i / 9][i % 9] = 0;
    }
  }
  return false;
}

int main(void)
{
  chrono::system_clock::time_point start, end;
  start = chrono::system_clock::now();

  Puzzle x;
  x.input();

  cout << "The problem is below" << endl;

  x.output();

  cout << endl;

  if (backtracking_search(x, 0))
  {
    cout << "The answer is below" << endl;
    x.output();
  }
  else
    cout << "fail";

  end = std::chrono::system_clock::now();
  double tim = chrono::duration_cast<chrono::milliseconds>(end - start).count();
  printf("かかった時間は %.f (millisec) です\n", tim);

  return 0;
}
