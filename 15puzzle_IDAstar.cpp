#include <iostream>
#include <cstdio>
#include <vector>
using namespace std;

const int Ntiles = 16;
const int Width = 4;
#define LIMIT 200

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

  void input()
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

  bool isgoal()
  {
    for (int i = 0; i < 16; i++)
      if (X[i] != i)
        return false;
    return true;
  }

  vector<State> EXPAND(State);

  State KID(int);

  friend int NOPS(State);

  friend int NTHOP(State, int);

  friend State *APPLY(State &, int);

  friend int mdincr(int, int, int);

  friend void UNDO(State &, State *);

  friend bool dfs(State, int, int, int);

  friend bool search(State);
};

typedef struct State *Undo;

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
      if (i % Ntiles - Width)
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

} optab;

vector<State> State::EXPAND(State s)
{
  vector<State> kids;

  for (int i = 0; i < optab.number(blank); i++)
  {
    kids.push_back(KID(optab.op(blank, i)));
  }

  return kids;
}

int NOPS(State s)
{
  return optab.number(s.blank);
}

int NTHOP(State s, int n)
{
  return optab.op(s.blank, n);
}

State State::KID(int newblank)
{
  State kid = *this;
  swap(kid.X[blank], kid.X[newblank]);
  kid.h = manhattan_dist(kid.X);
  kid.blank = newblank;
  return kid;
}

int mdincr(int tile, int newblank, int blank)
{
  return -abs(blank % 4) - abs(blank / 4) - abs(tile % 4 - newblank % 4) - abs(tile / 4 - newblank / 4) + abs(newblank % 4) + abs(newblank / 4) + abs(tile / 4 - blank / 4) + abs(tile % 4 - blank % 4);
}

State *APPLY(State &x, int newblank)
{
  Undo u = new State();

  u->h = x.h;
  u->blank = x.blank;
  int tile = x.X[newblank];

  for (int i = 0; i < 16; i++)
  {
    u->X[i] = x.X[i];
  }

  swap(x.X[x.blank], x.X[newblank]);
  x.h += mdincr(tile, newblank, x.blank);
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
int Min;

bool dfs(State x, int depth, int limit, int prev)
{
  if (x.isgoal())
  {
    return true;
  }

  if (depth + x.h > limit)
  {
    if (Min > depth + x.h)
      Min = depth + x.h - limit;
    return false;
  }

  nops = NOPS(x);

  x.show(0);

  for (int i = 0; i < nops; i++)
  {
    Undo u;

    u = APPLY(x, NTHOP(x, i));

    if (x.blank == prev)
    {
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
  int f = x.h;

  Min = 1001001001;

  for (int limit = f; limit <= LIMIT; limit += Min)
  {
    vec.clear();

    if (dfs(x, 0, limit, x.h))
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
