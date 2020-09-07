#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
using namespace std;
#define UNDEFINED -1
//UNDEFINEDは未定を表します。

class NQueen
{
private:
  int N;
  vector<int> A;
  //vector AはRow毎の印を置くColumnの数字を表します。
  vector<vector<int>> D;
  //vector D[i]はRow毎の印を置けるColumnの数字の合計を表します。

public:
  void input();
  void output();
  bool check(int, int);
  bool backtracking(int);
  bool search();
  bool MAC(int);
  bool REVISE(int, int);
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
  //全てのRowの数字がまだ決まっていないことを表します。

  D.resize(N);

  for (int i = 0; i < N; i++)
  {
    for (int t = 0; t < N; t++)
    {
      D[i].push_back(t);
      //全てのRowがまだ全てのColumnに印を置ける可能性があることを表します。
    }
  }
}
//インプットのメソッド

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
//アウトプットのメソッド

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
    else if (abs(A[t] - n) == abs(i - t))
    {
      ok = false;
    }
  }

  return ok;
}
//印の位置が制約を全て満たしていることを確認するメソッド

bool NQueen::MAC(int i)
{
  queue<pair<int, int>> Q;

  for (int t = i + 1; t < N; t++)
  {
    Q.push(make_pair(t, i));
  }
  //i＋1はまだ未定なので全てQに入れます。

  while (!Q.empty())
  {
    pair<int, int> X = Q.front();
    Q.pop();

    int x = X.first, y = X.second;

    if (REVISE(x, y))
    {
      //REVISEが大丈夫なら
      if (D[x].size() == 0)
      {
        return false;
        //D[x]のサイズが0ならfalse
      }
      for (int k = i + 1; k < N; k++)
      {
        if (A[k] == UNDEFINED && y != k && k != x)
        {
          Q.push(make_pair(k, x));
          //まだ未定でxでもyでもないRowを入れます
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
  //cancelにD[i]の削除する要素を入れます

  for (auto x : D[i])
  {
    bool mark = false;

    for (auto y : D[j])
    {
      if (x != y)
      {
        if (abs(x - y) != abs(i - j))
        {
          mark = true;
          //制約を満たすyがあったらmarkがtrueにしてD[i]の要素を削除するのをやめます
        }
      }
    }

    if (!mark)
    {
      cancel.push_back(x);
      revised = true;
    }
  }

  for (auto x : cancel)
  {
    D[i].erase(remove(D[i].begin(), D[i].end(), x), D[i].end());
  }
  //cancelに入っている要素をD[i]から消します

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
      const NQueen X = *this;
      //inferenceがfalseだったり、backtrackingがfalseだったりした時用にconst Xに今の状態を保存しておきます

      A[i] = x;
      D[i] = {x};

      bool inference = MAC(i);

      if (inference)
      {
        if (backtracking(i + 1))
        {
          return true;
        }
      }

      *this = X;
      //失敗したら元に戻します
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
