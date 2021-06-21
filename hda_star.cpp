#include <iostream>
#include <queue>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <vector>
#include <cstdlib>
using namespace std;
/*各種ライブラリをインクルード*/

/*パズルのボード上の空いている部分(以下では空白)を-1と呼ぶことにする*/

const int dx[] = {-1, 0, 1, 0};
const int dy[] = {0, -1, 0, 1};
mutex mtx;

class Board
{
private:
  int X[16]; /*配列Xがパズルのボードを表す*/
  int blank; /*空白の場所を表す変数*/
  int cost;

public:
  Board();      /*パズルのボードを作成するコンストラクタ*/
  void show();  /*パズルのボードを表示するメソッド*/
  void input(); /*初期状態のパズルのボードを入力するメソッド*/
  friend void psearch(int);
  friend void parallel();
  bool operator==(Board); /*ボードが等しいことを表す演算子(オーバーロード)*/
  bool operator<(const Board &b) const
  {
    for (int i = 0; i < 16; i++)
    {
      if (X[i] == b.X[i])
        continue;
      return X[i] > b.X[i];
    }
    return false;
  } /*mapを使用するにはBoardの順序が必要なので適当に定義しておく。*/

  friend int h(Board); /*ヒューリスティック関数を表す(フレンド)関数*/
};
/*パズルのボードを表すクラスを定義する*/

void psearch(int);
void parallen(void);

Board::Board()
{
  X[0] = -1;
  for (int i = 1; i < 16; i++)
    X[i] = i;
}
/*パズルのボードを作成するコンストラクタ*/
/*Russell & Norvingで初回されていたボードの正解の状態を標準としている*/

bool Board::operator==(Board bo)
{
  for (int i = 0; i < 16; i++)
  {
    if (X[i] != bo.X[i])
      return false;
  }
  return true;
}
/*ボードの状態が正しいかどうかをbool値で返す演算子。==演算子をオーバーロードする形になっている。*/

void Board::show()
{
  for (int i = 0; i < 16; i++)
  {
    if (i % 4 != 0)
      cout << " ";
    else
      cout << endl;
    cout << X[i];
  }
}
/*パズルのボードを表示するメソッド*/

int input_;

void Board::input()
{
  for (int i = 0; i < 16; i++)
  {
    cin >> input_;
    if (input_ == -1)
      blank = i;
    X[i] = input_;
  }
  return;
}
/*初期状態のパズルのボードを入力するメソッド*/

Board answer;

int h(Board x)
{
  int count = 0;

  for (int i = 0; i < 16; i++)
  {
    if (x.X[i] == -1)
    {
      count += abs(i / 4) + abs(i % 4);
    }
    else
    {
      count += abs(x.X[i] / 4 - i / 4) + abs(x.X[i] % 4 - i % 4);
    }
  }

  return count;
}

const int num = 4;
vector<vector<pair<int, Board>>> incomebuffer(num);
static bool Terminate[num];
static int incumbent = 1001001001;
Board now;

void parallel(void)
{
  Board board;
  board.input();
  /*初期状態のボードを取り込む*/
  pair<int, Board> x(h(board), board);
  vector<pair<int, Board>> v = {x};
  incomebuffer[0] = {v};

  for (int i = 0; i < num; i++)
  {
    Terminate[i] = false;
  }

  vector<thread> ths(num);

  int id = 0;

  for (auto &th : ths)
  {
    th = thread(psearch, id);
    id += 1;
  }

  for (auto &t : ths)
  {
    t.join();
  }

  printf("answer is %d\n", incumbent);
}

void psearch(int id)
{
  map<Board, pair<bool, int>> closedlist;
  priority_queue<pair<int, Board>, vector<pair<int, Board>>, greater<pair<int, Board>>> openlist;
  vector<vector<pair<int, Board>>> outgobuffer(num);

  while (1)
  {
    if (!incomebuffer[id].empty())
    {
      mtx.lock();
      vector<pair<int, Board>> t = incomebuffer[id];
      incomebuffer[id].clear();
      mtx.unlock();

      for (auto x : t)
      {
        if (!closedlist[x.second].first)
        {
          openlist.push(x);
        }
        else if (closedlist[x.second].second > x.first)
        {
          openlist.push(x);
        }
      }
      continue;
    }

    if (openlist.empty() || openlist.top().first >= incumbent)
    {
      Terminate[id] = true;
      bool flag = true;

      for (int i = 0; i < num; i++)
      {
        if (!Terminate[i])
        {
          flag = false;
          break;
        }
      }

      if (flag)
      {
        break;
      }
      else
      {
        Terminate[id] = false;
        continue;
      }
    }

    pair<int, Board> x_board = openlist.top();
    openlist.pop();

    Board tmp = x_board.second;
    int f = x_board.first;

    if (closedlist[tmp].first)
    {
      if (closedlist[tmp].second <= f)
      {
        continue;
      }
    }

    if (tmp == answer)
    {
      if (f < incumbent)
      {
        incumbent = f;
      }

      continue;
    }

    closedlist[tmp] = make_pair(true, f);

    int bx = tmp.blank / 4;
    int by = tmp.blank % 4;

    for (int i = 0; i < 4; i++)
    {
      int tx = bx + dx[i];
      int ty = by + dy[i];

      if (tx < 0 || tx >= 4 || ty < 0 || ty >= 4)
        continue;

      Board y = tmp;
      int depth = f - h(y);
      swap(y.X[tmp.blank], y.X[tx * 4 + ty]);
      y.blank = tx * 4 + ty;

      int hash = rand() % num;

      if (hash == id)
      {
        openlist.push(make_pair(depth + 1 + h(y), y));
      }
      else if (mtx.try_lock())
      {
        incomebuffer[hash].push_back(make_pair(depth + 1 + h(y), y));

        for (vector<pair<int, Board>>::iterator it = outgobuffer[hash].begin(); it != outgobuffer[hash].end(); ++it)
        {
          incomebuffer[hash].push_back(*it);
        }

        mtx.unlock();
        outgobuffer[hash].clear();
      }
      else
      {
        outgobuffer[hash].push_back(make_pair(depth + 1 + h(y), y));
      }
    }
  }
}

int main(void)
{
  parallel();
  return 0;
}
