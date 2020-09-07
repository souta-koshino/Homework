#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;
#define UNDEFINED -1
//UNDEFINEDは未定を表します。

class NQueen
{
private:
  int N;
  vector<int> A;
  //vector AはRow毎の印を置くColumnの数字を表します。
  vector<vector<pair<int, int>>> D;
  //vector D[i]の中のpairは、firstが侵している制約の数、secondがそのColumnの数字を表します。

public:
  void input();
  void output();
  bool check(int, int);
  bool consistent();
  bool informed_backtracking(queue<int>, queue<int>);
  bool search();
  void update(int);
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
    for (int j = 0; j < N; j++)
    {
      D[i].push_back(make_pair(0, j));
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

bool NQueen::check(int i, int n)
{
  bool ok = true;

  for (int t = 0; t < i; t++)
  {
    if (A[t] == UNDEFINED)
    {
      continue;
    }
    if (A[t] == n)
    {
      ok = false;
    }
    else if (abs(A[t] - n) == abs(i - t))
    {
      ok = false;
    }
  }

  return ok;
}
//印の位置が制約を全て満たしていることを確認するメソッド

bool NQueen::consistent(void)
{
  bool ok = true;

  for (int i = 0; i < N; i++)
  {
    if (A[i] == UNDEFINED)
    {
      ok = false;
    }
    for (int j = 0; j < N; j++)
    {
      if (i != j)
      {
        if (abs(A[i] - A[j]) == abs(i - j))
        {
          ok = false;
        }
        if (A[i] == A[j])
        {
          ok = false;
        }
      }
    }
  }

  return ok;
}
//全てのRowが埋まっていて、制約を一つも侵していないか確認するメソッド

void NQueen::update(int x)
{
  for (int i = 0; i < N; i++)
  {
    int count = 0;

    for (int j = 0; j < x; j++)
    {
      if (A[j] == UNDEFINED)
      {
        continue;
      }

      if (A[j] == i)
      {
        count++;
      }

      if (abs(x - j) == abs(A[j] - i))
      {
        count++;
      }
    }

    D[x][i] = make_pair(count, i);
  }
}
//Row xの各Columnごとにそこに印を置いたら侵すことになる制約の数をD[x]に入れるメソッド

bool NQueen::informed_backtracking(queue<int> VARS_LEFT, queue<int> VARS_DONE)
{
  if (VARS_LEFT.empty())
  {
    if (consistent())
      return true;
    else
      return false;
  }
  //VARS_LEFTがなくてconsistentがtrueなら正解

  int var = VARS_LEFT.front();
  VARS_LEFT.pop();

  VARS_DONE.push(var);

  update(var);
  //論文のアルゴリズムにupdateメソッドを追加しています

  sort(D[var].begin(), D[var].end());
  //制約の数を昇順にソートします

  for (auto x : D[var])
  {
    if (check(var, x.second))
    {
      A[var] = x.second;
      if (informed_backtracking(VARS_LEFT, VARS_DONE))
      {
        return true;
        //再帰してtrueならokを返します。
      }
    }
  }

  return false;
}

bool NQueen::search(void)
{
  queue<int> X, Y;

  for (int i = 0; i < N; i++)
  {
    X.push(i);
  }
  //VARS_LEFTを表すXに全てのRowを入れます

  if (informed_backtracking(X, Y))
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
