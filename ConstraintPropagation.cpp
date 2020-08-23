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

  pair<bool, Puzzle> MAC(int);

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
    else
    {
      A[i].second.push_back(A[i].first);
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
  for (int i = 0; i < 81; i++)
  {
    for (vector<int>::iterator ite = A[i].second.begin(); ite != A[i].second.end(); ite++)
    {
      cout << *ite;
    }
    cout << endl;
  }
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

pair<bool, Puzzle> Puzzle::MAC(int i)
{
  queue<pair<int, int>> Q;
  const Puzzle B = *this;

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
        return make_pair(false, B);
      for (int k = 0; k < 81; k++)
      {
        if (constraints.check(k, x) && k != y && A[k].first == 0)
        {
          Q.push(make_pair(k, x));
        }
      }
    }
  }
  return make_pair(true, B);
}

bool Puzzle::REVISE(int i, int j)
{
  bool revised = false;

  vector<int> x;
  for (vector<int>::iterator ite = A[i].second.begin(); ite != A[i].second.end(); ite++)
  {
    x.push_back(*ite);
  }

  for (vector<int>::iterator ite = x.begin(); ite != x.end(); ite++)
  {
    bool mark = true;

    for (vector<int>::iterator it = A[j].second.begin(); it != A[j].second.end(); it++)
    {
      if (*ite != *it)
      {
        mark = false;
      }
    }

    if (mark)
    {
      A[i].second.erase(remove(A[i].second.begin(), A[i].second.end(), *ite), A[i].second.end());
      revised = true;
    }
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
      A[i].first = *ite;

      pair<bool, Puzzle> X = (*this).MAC(i);

      if (X.first)
      {
        if ((*this).backtracking_search(i + 1))
        {
          return true;
        }
        *this = X.second;
      }
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

  end = std::chrono::system_clock::now();

  cout << "かかった時間は" << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "(millisec)です\n";

  return 0;
}
