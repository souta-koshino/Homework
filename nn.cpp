#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <cmath>
using namespace std;
#define UNDEFINED -1

class NQueen
{
private:
  int N;
  vector<int> A;
  vector<vector<int>> D;

public:
  void input();
  void output();
  bool check(int, int);
  bool backtracking(int);
  bool search();
  bool MAC(int);
  bool REVISE(int, int);
  void c(void);
};

void NQueen::input(void)
{
  cout << "Nの値を入力してください：";
  cin >> N;

  A.resize(N);

  for (int i = 0; i < N; i++)
  {
    A[i] = UNDEFINED;
  }

  D.resize(N);

  for (int i = 0; i < N; i++)
  {
    for (int t = 0; t < N; t++)
    {
      D[i].push_back(t);
    }
  }
}

void NQueen::output(void)
{
  for (int i = 0; i < N * N; i++)
  {
    if (i % N == 0)
    {
      cout << endl;
    }

    if (A[i / N] == i % N)
    {
      cout << 1 << " ";
    }
    else
    {
      cout << 0 << " ";
    }
  }
  cout << endl
       << endl;
}

void NQueen::c(void)
{
  for (int i = 0; i < N; i++)
  {
    for (auto t : D[i])
    {
      cout << t << " ";
    }
    cout << endl;
  }
  return;
}

bool NQueen::check(int i, int n)
{
  bool ok = true;

  for (int t = 0; t < N; t++)
  {
    if (A[t] == UNDEFINED)
    {
      continue;
    }
    if (A[t] == n)
    {
      ok = false;
    }
    else if ((abs(A[t] + t * N - n - i * N)) % (N - 1) == 0)
    {
      ok = false;
    }
    else if ((abs(A[t] + t * N - n - i * N)) % (N + 1) == 0)
    {
      ok = false;
    }
  }

  return ok;
}

bool NQueen::MAC(int i)
{
  queue<pair<int, int>> Q;

  for (int t = i + 1; t < N; t++)
  {
    Q.push(make_pair(t, i));
  }

  while (!Q.empty())
  {
    pair<int, int> X = Q.front();
    Q.pop();

    int x = X.first, y = X.second;

    if (REVISE(x, y))
    {
      if (D[x].size() == 0)
      {
        cout << "解なし\n";
        return false;
      }
      for (int k = i + 1; k < N; k++)
      {
        if (A[k] == UNDEFINED && y != k && k != x)
        {
          Q.push(make_pair(k, x));
        }
      }
    }
  }

  return true;
}

bool NQueen::REVISE(int i, int j)
{
  bool revised = false;
  vector<int> cancel;

  for (vector<int>::iterator ite = D[i].begin(); ite != D[i].end(); ite++)
  {
    bool mark = true;

    if (D[j].begin() == D[j].end())
    {
      if (abs(i * N + *ite - j * N - A[j]) % N != 0 && abs(i * N + *ite - j * N - A[j]) % (N + 1) != 0 && abs(i * N + *ite - j * N - A[j]) % (N - 1) != 0)
      {
        mark = false;
      }
    }
    else
    {
      for (vector<int>::iterator it = D[j].begin(); it != D[j].end(); it++)
      {
        if (abs(i * N + *ite - j * N - *it) % N != 0 && abs(i * N + *ite - j * N - *it) % (N + 1) != 0 && abs(i * N + *ite - j * N - *it) % (N - 1) != 0)
        {
          mark = false;
        }
      }
    }

    if (mark)
    {
      cancel.push_back(*ite);
      revised = true;
    }
  }

  for (vector<int>::iterator ite = cancel.begin(); ite != cancel.end(); ite++)
  {
    D[i].erase(remove(D[i].begin(), D[i].end(), *ite), D[i].end());
  }

  return revised;
}

bool NQueen::backtracking(int i)
{
  if (i == N)
  {
    return true;
  }

  for (auto x : D[i])
  {
    if (check(i, x))
    {
      cout << "checkを通過\n";
      const NQueen X = *this;

      A[i] = x;
      D[i].clear();

      cout << "MAC前\n";

      c();
      bool inference = MAC(i);

      cout << "MAC後\n";
      c();

      if (inference)
      {
        cout << "MAC(" << i << ")を通過\n";

        if (backtracking(i + 1))
        {
          return true;
        }
      }

      *this = X;
    }
  }

  return false;
}

bool NQueen::search(void)
{
  if (backtracking(0))
  {
    return true;
  }
  else
  {
    return false;
  }
}

int main(void)
{
  NQueen x;

  x.input();

  cout << "The problem is below\n";

  x.output();

  cout << endl;

  if (x.search())
  {
    cout << "The answer is below\n";
    x.output();
  }
  else
  {
    cout << "failed\n";
    x.output();
  }

  return 0;
}
