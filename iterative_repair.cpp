#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <stdlib.h>
using namespace std;

#define INF 1001001001

class NQueen
{
private:
  int N;
  int *A;

  int *colCount;
  int *upperDiagCount;
  int *lowerDiagCount;

public:
  void input();
  int count();
  inline int conflict_count(int, int);
  int HighestConflicts();
  void MinConflicts(int &);
  void search();
  bool complete();
};

void NQueen::input(void)
{
  cout << "Nの値を入力してください：";
  cin >> N;

  A = new int[N];

  colCount = new int[N];
  upperDiagCount = new int[2 * N - 1];
  lowerDiagCount = new int[2 * N - 1];

  for (int i = 0; i < 2 * N - 1; i++)
  {
    if (i < N)
    {
      colCount[i] = 0;
    }
    lowerDiagCount[i] = upperDiagCount[i] = 0;
  }

  vector<int> minConflictCols;
  /*最小のconflictのcolumnを格納するためのvector*/
  int minConflicts;
  int tempConflicts;

  A[0] = rand() % N;

  colCount[A[0]]++;
  upperDiagCount[A[0]]++;
  lowerDiagCount[N - A[0] - 1]++;
  /*Row=0だけはランダム*/

  for (int i = 1; i < N; i++)
  {
    minConflictCols.clear();
    minConflicts = INF;

    for (int j = 0; j < N; j++)
    {
      tempConflicts = conflict_count(i, j);

      if (tempConflicts < minConflicts)
      {
        minConflicts = tempConflicts;
        minConflictCols.clear();
        minConflictCols.push_back(j);
      }
      else if (tempConflicts == minConflicts)
      {
        minConflictCols.push_back(j);
      }
    }
    /*全てのRowのconflictを計算して最小のものをminConflictColsに入れる*/

    A[i] = minConflictCols[rand() % minConflictCols.size()];

    colCount[A[i]]++;
    upperDiagCount[A[i] + i]++;
    lowerDiagCount[N - A[i] + i - 1]++;
    /*minConflictColsからランダムに取り出してRow=iのcolumnを決定し、制約を更新*/
  }

  return;
}

inline int NQueen::conflict_count(int x, int y)
{
  int t = ((colCount[y] + 1) * colCount[y]) / 2 + ((upperDiagCount[y + x] + 1) * upperDiagCount[y + x]) / 2 + ((lowerDiagCount[(N - y) + x - 1] + 1) * lowerDiagCount[(N - y) + x - 1]) / 2;

  return t;
}
/*Row=x, Column=yにQueenを置いた時に犯した制約の数を出力する関数*/

int NQueen::count()
{
  int conflicts = 0;

  for (int i = 0; i < 2 * N - 1; i++)
  {
    if (i < N)
    {
      conflicts += ((colCount[i] - 1) * colCount[i]) / 2;
    }
    conflicts += ((upperDiagCount[i] - 1) * upperDiagCount[i]) / 2;
    conflicts += ((lowerDiagCount[i] - 1) * lowerDiagCount[i]) / 2;
  }

  return conflicts;
}
/*現在のconflictの数を出力する関数*/

int NQueen::HighestConflicts()
{
  int maxConflict = 0;
  int tempConflict;
  vector<int> maxConflictRows;

  for (int i = 0; i < N; i++)
  {
    tempConflict = conflict_count(i, A[i]);

    if (tempConflict > maxConflict)
    {
      maxConflict = tempConflict;
      maxConflictRows.clear();
      maxConflictRows.push_back(i);
    }
    else if (tempConflict == maxConflict)
    {
      maxConflictRows.push_back(i);
    }
  }

  return maxConflictRows[rand() % maxConflictRows.size()];
}
/*最も制約を犯しているRowを出力する関数*/

void NQueen::MinConflicts(int &x)
{
  /*xは現在のconflictの合計を表す変数の参照*/

  int highestconflict = HighestConflicts();

  int prev = A[highestconflict];

  int minConflicts = INF;
  int tempConflict;
  vector<int> minConflictCols;

  x -= ((colCount[prev] - 1) * colCount[prev]) / 2;
  x -= ((upperDiagCount[prev + highestconflict] - 1) * upperDiagCount[prev + highestconflict]) / 2;
  x -= ((lowerDiagCount[N - prev + highestconflict - 1] - 1) * lowerDiagCount[N - prev + highestconflict - 1]) / 2;

  colCount[prev]--;
  upperDiagCount[prev + highestconflict]--;
  lowerDiagCount[N - prev + highestconflict - 1]--;
  /*現在のcolumn=prevについて、xからprevが犯しているconflictを差し引いて、制約についても更新*/

  for (int i = 0; i < N; i++)
  {
    tempConflict = conflict_count(highestconflict, i);

    if (tempConflict < minConflicts)
    {
      minConflicts = tempConflict;
      minConflictCols.clear();
      minConflictCols.push_back(i);
    }
    else if (tempConflict == minConflicts)
    {
      minConflictCols.push_back(i);
    }
  }

  int r = minConflictCols[rand() % minConflictCols.size()];
  /*最も制約を犯していない(最小のconflictとなる)columnを決定*/

  A[highestconflict] = r;
  colCount[r]++;
  upperDiagCount[r + highestconflict]++;
  lowerDiagCount[N - r + highestconflict - 1]++;

  x += ((colCount[r] - 1) * colCount[r]) / 2;
  x += ((upperDiagCount[r + highestconflict] - 1) * upperDiagCount[r + highestconflict]) / 2;
  x += ((lowerDiagCount[N - r + highestconflict - 1] - 1) * lowerDiagCount[N - r + highestconflict - 1]) / 2;
  /*制約とconflictの合計を更新*/

  return;
}

bool NQueen::complete()
{
  bool check = true;

  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < N; j++)
    {
      if (i != j)
      {
        if (abs(A[i] - A[j]) == abs(i - j))
        {
          check = false;
        }
        if (A[i] == A[j])
        {
          check = false;
        }
      }
    }
  }

  return check;
}
/*完成した盤面が正解かを判定する関数*/

void NQueen::search()
{
  int conflict = count();
  /*まず最初に盤面のconflictの合計を計算*/

  while (conflict != 0)
  {
    MinConflicts(conflict);
    /*MinConflicts関数を使って盤面を更新。conclictも同時に更新して0になったら終了。*/
  }

  return;
}

int main(void)
{
  NQueen x;
  x.input();

  chrono::system_clock::time_point start, end;
  start = chrono::system_clock::now();

  x.search();

  if (x.complete())
  {
    /*一応complete関数を使って盤面が正解か判定*/
    cout << "solved!\n";
  }
  else
  {
    cout << "failed...\n";
  }

  end = std::chrono::system_clock::now();

  cout << "かかった時間は" << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "(millisec)です\n";

  return 0;
}
