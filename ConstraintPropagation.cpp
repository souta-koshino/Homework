#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
using namespace std;

class Puzzle
{
private:
  pair<int, vector<int>> A[81];

public:
  void input(void);

  void output(void);

  bool horizontal_check(int, int);

  bool vertical_check(int, int);

  bool cell_check(int, int);

  bool assign(int, int);

  bool complete(void);

  bool MAC(int);

  bool REVISE(int, int);

  bool backtracking_search(int);

  void check(void);
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
        if (i == j)
        {
          C[i][j] = 0;
        }
        else if (i / 9 == j / 9)
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
        A[i].first = 0;
      }
      else
      {
        A[i].first = tmp - '0';
      }
    }
  }

  for (int i = 0; i < 81; i++)
  {
    if (A[i].first == 0)
    {
      for (int n = 1; n <= 9; n++)
      {
        if ((*this).assign(i, n))
          A[i].second.push_back(n);
      }
    }
  }
}

void Puzzle::output()
{
  for (int i = 0; i < 81; i++)
  {
    if (i % 9 == 0)
      cout << endl;
    cout << A[i].first << " ";
  }
  cout << endl
       << endl;
}

void Puzzle::check(void)
{
  int c = 0;
  for (int i = 0; i < 81; i++)
  {
    for (vector<int>::iterator ite = A[i].second.begin(); ite != A[i].second.end(); ite++)
    {
      c += *ite;
    }
  }
  cout << c << endl;
}

bool Puzzle::horizontal_check(int i, int n)
{
  for (int j = 0; j < 81; j++)
  {
    if (i / 9 == j / 9)
    {
      if (A[j].first == n)
      {
        return false;
      }
    }
  }
  return true;
}

bool Puzzle::vertical_check(int i, int n)
{
  for (int j = 0; j < 81; j++)
  {
    if (i % 9 == j % 9)
    {
      if (A[j].first == n)
      {
        return false;
      }
    }
  }
  return true;
}

bool Puzzle::cell_check(int i, int n)
{

  for (int j = 0; j < 81; j++)
  {
    if (i % 9 / 3 == j % 9 / 3 && i / 9 / 3 == j / 9 / 3)
    {
      if (A[j].first == n)
        return false;
    }
  }
  return true;
}

bool Puzzle::assign(int i, int n)
{
  if (vertical_check(i, n) && horizontal_check(i, n) && cell_check(i, n))
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
  for (int i = 0; i < 81; i++)
  {
    if (A[i].first == 0)
      return false;
  }
  return true;
}

bool Puzzle::MAC(int i)
{
  A[i].second.clear();

  queue<pair<int, int>> Q;

  for (int j = 0; j < 81; j++)
  {
    if (constraints.check(j, i) && A[j].first == 0)
      Q.push(make_pair(j, i));
  }

  while (!Q.empty())
  {
    pair<int, int> P = Q.front();
    Q.pop();

    int x = P.first, y = P.second;

    if ((*this).REVISE(x, y))
    {
      if (A[x].second.size() == 0)
        return false;
      for (int k = 0; k < 81; k++)
      {
        if (constraints.check(k, x) && k != y && A[k].first == 0)
        {
          Q.push(make_pair(k, x));
        }
      }
    }
  }
  return true;
}

bool Puzzle::REVISE(int i, int j)
{
  bool revised = false;

  vector<int> cancel;

  for (vector<int>::iterator ite = A[i].second.begin(); ite != A[i].second.end(); ite++)
  {
    bool mark = true;

    if (A[j].second.begin() == A[j].second.end())
    {
      if (*ite != A[j].first)
      {
        mark = false;
      }
    }
    else
    {
      for (vector<int>::iterator it = A[j].second.begin(); it != A[j].second.end(); it++)
      {
        if (*ite != *it)
        {
          mark = false;
        }
      }

      if (mark)
      {
        cancel.push_back(*ite);
        revised = true;
      }
    }
  }

  for (vector<int>::iterator ite = cancel.begin(); ite != cancel.end(); ite++)
  {
    A[i].second.erase(remove(A[i].second.begin(), A[i].second.end(), *ite), A[i].second.end());
  }

  return revised;
}

bool Puzzle::backtracking_search(int i)
{
  if (i == 81)
  {
    if ((*this).complete())
    {
      return true;
    }
    return false;
  }

  if (A[i].first != 0)
  {
    return (*this).backtracking_search(i + 1);
  }

  for (vector<int>::iterator ite = A[i].second.begin(); ite != A[i].second.end(); ite++)
  {
    if ((*this).assign(i, *ite))
    {
      const Puzzle X = *this;

      A[i].first = *ite;

      if ((*this).MAC(i))
      {
        if ((*this).backtracking_search(i + 1))
        {
          return true;
        }
      }

      *this = X;
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
  x.check();

  cout << "The problem is below" << endl;

  x.output();
  x.check();

  cout << endl;

  if (x.backtracking_search(0))
  {
    cout << "The answer is below" << endl;
    x.output();
  }
  else
  {
    cout << "fail\n";
    x.output();
  }
  x.check();

  end = std::chrono::system_clock::now();

  cout << "かかった時間は" << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "(millisec)です\n";

  return 0;
}
