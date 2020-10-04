#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <stdlib.h>
#include <numeric>
#include <iterator>
#include <time.h>
using namespace std;

#define INF 1001001001

class NQueen
{
private:
  const int N = 8;
  int *A;
  /*各columnに対応するrowを収納する配列*/

  int *upperDiagCount;
  int *lowerDiagCount;
  /*対角線に並んでいるものをカウントする配列*/

  int *colCount;

  bool *lowerDiag;
  bool *upperDiag;
  /*対角線が既に埋まっているか否かをtrue or false返す配列*/

  vector<int> candidates;
  /*conflictしているcolumnを保存する*/

  int m;

public:
  void input();
  void search();
  bool complete();
  void output();
};

int conflict(int n)
{
  if (n <= 10)
    return (n > 8) ? 8 : n;
  else if (n < 100)
    return n;
  else if (n < 1000)
    return 30;
  else if (n < 10000)
    return 50;
  else if (n < 100000)
    return 80;
  else
    return 100;
}
/*論文に載っている初期状態の時にconflictするcolumnの数を出力する関数*/

void NQueen::input(void)
{
  srand((unsigned)time(NULL));

  A = new int[N];

  upperDiagCount = new int[2 * N - 1];
  lowerDiagCount = new int[2 * N - 1];
  upperDiag = new bool[2 * N - 1];
  lowerDiag = new bool[2 * N - 1];

  for (int i = 0; i < 2 * N - 1; i++)
  {
    if (i < N)
    {
      A[i] = i;
    }
    lowerDiagCount[i] = upperDiagCount[i] = 0;
    lowerDiag[i] = upperDiag[i] = false;
  }

  int z;

  m = N - conflict(N);
  /*典型的なケースについてconflictするまでのcolumnの数をmで表す*/

  for (int i = 0, last = N; i < m; i++, last--)
  {
    do
    {
      z = i + rand() % last;
    } while (lowerDiag[N - A[z] + i - 1] || upperDiag[A[z] + i]);

    swap(A[i], A[z]);
    upperDiagCount[A[i] + i]++;
    lowerDiagCount[N - A[i] + i - 1]++;
    upperDiag[A[i] + i] = true;
    lowerDiag[N - A[i] + i - 1] = true;
  }
  /*まずはmまでの値をシャッフルによってランダム化する*/

  for (int i = m, last = N - m; i < N; i++, last--)
  {
    z = i + rand() % last;
    swap(A[i], A[z]);
    upperDiagCount[A[i] + i]++;
    lowerDiagCount[N - A[i] + i - 1]++;
  }
  /*m以降同士でシャッフル*/

  for (int i = 0; i < 2 * N - 1; i++)
  {
    upperDiagCount[i] = lowerDiagCount[i] = 0;
  }

  for (int i = 0; i < N; i++)
  {
    upperDiagCount[i + A[i]]++;
    lowerDiagCount[N - A[i] + i - 1]++;

    if (lowerDiagCount[N - A[i] + i - 1] > 1 || upperDiagCount[i + A[i]] > 1)
    {
      candidates.push_back(i);
    }
    /*row=0から見ていき初めてconflictするものをcandidatesに保存*/
  }

  return;
}

void NQueen::output(void)
{
  for (int i = 0; i < N * N; ++i)
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
  return;
}
/*確認用のoutput関数(使わない)*/

bool NQueen::complete()
{
  int conflicts = 0;
  colCount = new int[N];

  for (int i = 0; i < 2 * N - 1; i++)
  {
    if (i < N)
      colCount[i] = 0;
    upperDiagCount[i] = lowerDiagCount[i] = 0;
  }

  for (int i = 0; i < N; i++)
  {
    if (i < N)
      colCount[A[i]]++;
    upperDiagCount[A[i] + i]++;
    lowerDiagCount[N - A[i] + i - 1]++;
  }

  for (int i = 0; i < 2 * N - 1; i++)
  {
    if (i < N)
    {
      conflicts += ((colCount[i] - 1) * colCount[i]) / 2;
    }
    conflicts += ((upperDiagCount[i] - 1) * upperDiagCount[i]) / 2;
    conflicts += ((lowerDiagCount[i] - 1) * lowerDiagCount[i]) / 2;
  }

  if (conflicts == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}
/*最後答えが出た時に確認する用のcomplete関数(なくてもいい)*/

void NQueen::search()
{
  int swaps_performed;

  while (true)
  {
    swaps_performed = 0;
    /*swapした回数*/

    for (auto i : candidates)
    {
      for (int j = 0; j < N; j++)
      {
        if (i == j)
        {
          continue;
        }

        int prev = upperDiagCount[i + A[i]] + lowerDiagCount[N - A[i] + i - 1] + upperDiagCount[j + A[j]] + lowerDiagCount[N - A[j] + j - 1];
        /*swap前のconflictの合計*/

        upperDiagCount[A[i] + i]--;
        lowerDiagCount[N - A[i] + i - 1]--;
        upperDiagCount[A[j] + j]--;
        lowerDiagCount[N - A[j] + j - 1]--;

        swap(A[i], A[j]);

        upperDiagCount[A[i] + i]++;
        lowerDiagCount[N - A[i] + i - 1]++;
        upperDiagCount[A[j] + j]++;
        lowerDiagCount[N - A[j] + j - 1]++;

        /*swapしてupperDiagCountとlowerDiagCountを更新*/

        int current = upperDiagCount[i + A[i]] + lowerDiagCount[N - A[i] + i - 1] + upperDiagCount[j + A[j]] + lowerDiagCount[N - A[j] + j - 1];
        /*swap後のconflictの合計*/

        if (prev > current)
        {
          swaps_performed++;
          /*swapしてconflictの合計が減ったらswap_performedをインクリメント*/
        }
        else
        {
          upperDiagCount[A[i] + i]--;
          lowerDiagCount[N - A[i] + i - 1]--;
          upperDiagCount[A[j] + j]--;
          lowerDiagCount[N - A[j] + j - 1]--;

          swap(A[i], A[j]);

          upperDiagCount[A[i] + i]++;
          lowerDiagCount[N - A[i] + i - 1]++;
          upperDiagCount[A[j] + j]++;
          lowerDiagCount[N - A[j] + j - 1]++;
          /*減らなかったら元に戻す*/
        }
      }
    }

    if (swaps_performed == 0)
    {
      return;
    }
    /*candidates内の全てのcolumnについてswapが行われなかったらwhileループを終了*/

    candidates.clear();

    for (int i = 0; i < 2 * N - 1; i++)
    {
      upperDiagCount[i] = lowerDiagCount[i] = 0;
    }

    for (int i = 0; i < N; i++)
    {
      upperDiagCount[i + A[i]]++;
      lowerDiagCount[N - A[i] + i - 1]++;

      if (lowerDiagCount[N - A[i] + i - 1] > 1 || upperDiagCount[i + A[i]] > 1)
      {
        candidates.push_back(i);
      }
    }
    /*candidatesを更新*/
  }
}

int main(void)
{
  chrono::system_clock::time_point start, end;
  start = chrono::system_clock::now();

  NQueen x;
  x.input();

  end = std::chrono::system_clock::now();
  cout << "かかった時間は" << chrono::duration_cast<chrono::seconds>(end - start).count() << "(sec)です\n";

  start = chrono::system_clock::now();

  x.search();

  if (x.complete())
  {
    /*一応complete関数を使って盤面が正解か判定*/
    cout << "solved!\n";
  }
  else
  {
    while (true)
    {
      /*失敗したらもう一度実行します*/
      x.input();
      x.search();
      if (x.complete())
      {
        /*ここでも一応complete関数を使って盤面が正解か判定*/
        cout << "solved!\n";
        break;
      }
    }
  }

  end = std::chrono::system_clock::now();

  cout << "かかった時間は" << chrono::duration_cast<chrono::seconds>(end - start).count() << "(sec)です\n";

  return 0;
}
