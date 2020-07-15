#include <iostream>
#include <cstdio>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

const int Ntiles = 16;
const int Width = 4;
#define LIMIT 100

int input_;

int manhattan_dist(int X[16])
{
  int count = 0;
  for (int i = 0; i < 16; i++)
  {
    {
      count += abs(X[i] / 4 - i / 4) + abs(X[i] % 4 - i % 4);
    }
  }
  return count;
}

class State
{
private:
  int X[16];
  int blank;
  int h;

public:
  State()
  {
    for (int i = 0; i < 16; i++)
    {
      X[i] = i;
    }
    blank = 0;
    h = 0;
  }

  void show(int x)
  {
    cout << "State " << x << " :";
    for (int i = 0; i < 16; i++)
    {
      if (i % 4 != 0)
        printf(" ");
      else
        printf("\n");
      printf("%d", X[i]);
    }
    printf("\n");
  }

  void input(void)
  {
    for (int i = 0; i < 16; i++)
    {
      cin >> input_;
      if (input_ == 0)
        blank = i;
      X[i] = input_;
    }
    h = manhattan_dist(X);
    return;
  }

  bool isgoal(void)
  {
    for (int i = 0; i < 16; i++)
      if (X[i] != i)
        return false;
    return true;
  }

  friend int NOPS(State);

  friend int NTHOP(State, int);

  friend State *APPLY(State &, int);

  friend int mdincr(int, int, int);

  friend void UNDO(State &, State *);

  friend bool dfs(State, int, int, int);

  friend bool search(State);
};

typedef class State *Undo;

class Optab : public State
{
private:
  vector<int> Y[16];

public:
  Optab()
  {
    for (int i = 0; i < 16; i++)
    {
      if (i >= Width)
      {
        Y[i].push_back(i - Width);
      }
      if (i % Width > 0)
      {
        Y[i].push_back(i - 1);
      }
      if (i % Width < Width - 1)
      {
        Y[i].push_back(i + 1);
      }
      if (i < Ntiles - Width)
      {
        Y[i].push_back(i + Width);
      }
    }
  }

  int number(int i)
  {
    return Y[i].size();
  }

  int op(int i, int j)
  {
    return Y[i][j];
  }
};

Optab optab;

int NOPS(State s)
{
  return optab.number(s.blank);
}

int NTHOP(State s, int n)
{
  return optab.op(s.blank, n);
}

int mdincr(int tile, int newblank, int blank)
{
  return abs(newblank % 4) + abs(newblank / 4) + abs(tile / 4 - blank / 4) + abs(tile % 4 - blank % 4) - abs(blank % 4) - abs(blank / 4) - abs(tile % 4 - newblank % 4) - abs(tile / 4 - newblank / 4);
}

State *APPLY(State &x, int newblank)
{
  Undo u = new State;

  u->h = x.h;
  u->blank = x.blank;

  for (int j = 0; j < 16; j++)
  {
    u->X[j] = x.X[j];
  }

  swap(x.X[x.blank], x.X[newblank]);

  x.h += mdincr(x.X[x.blank], newblank, x.blank);
  x.blank = newblank;

  return u;
}

void UNDO(State &x, Undo u)
{
  for (int i = 0; i < 16; i++)
  {
    x.X[i] = u->X[i];
  }

  x.h = u->h;
  x.blank = u->blank;

  delete u;
}

int nops;
vector<State> vec;
int tmp;

bool dfs(State x, int depth, int limit, int prev)
{
  if (x.isgoal())
  {
    return true;
  }

  if (depth + x.h > limit)
  {
    return false;
  }

  nops = NOPS(x);

  for (int i = 0; i < nops; i++)
  {
    Undo u;

    tmp = NTHOP(x, i);

    u = APPLY(x, tmp);

    if (x.blank == prev)
    {
      UNDO(x, u);
      continue;
    }

    if (dfs(x, depth + 1, limit, x.blank))
    {
      vec.push_back(x);
      return true;
    }

    UNDO(x, u);
  }
  return false;
}

bool search(State x)
{
  for (int limit = x.h; limit <= LIMIT; limit++)
  {
    if (dfs(x, 0, limit, x.blank))
    {
      return true;
    }
    else
    {
      continue;
    }
  }
  return false;
}

int t;

int main(void)
{
  State x;
  x.input();

  if (search(x))
  {
    int count = 0;

    reverse(vec.begin(), vec.end());

    for (vector<State>::iterator ite = vec.begin(); ite != vec.end(); ite++)
    {
      count++;
      (*ite).show(count);
    }
  }
  else
    cout << "fail";

  return 0;
}
