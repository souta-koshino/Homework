#include <iostream>
#include <queue>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <vector>
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

public:
  Board();                   /*パズルのボードを作成するコンストラクタ*/
  void show();               /*パズルのボードを表示するメソッド*/
  void input();              /*初期状態のパズルのボードを入力するメソッド*/
  friend void psearch(void); /*uniform cost searchを行う(フレンド)関数*/
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
/*マンハッタン距離を返すヒューリスティック関数。x座標はindex/3で、y座標はindex%3で与えられるので、正解の一との絶対値の差を合計する。*/

/*以下の関数で使う変数*/

priority_queue<pair<int, Board>, vector<pair<int, Board> >, greater<pair<int, Board> > > PQ;
map<Board, bool> V;

bool ans = false;

void psearch(void)
{
  while (1)
  {
    mtx.lock();
    if (PQ.empty())
    {
      mtx.unlock();
      return;
    }
    mtx.unlock();

    /*PQがなくなるまで以下を続ける*/
    mtx.lock();
    pair<int, Board> x_board = PQ.top();
    PQ.pop();
    mtx.unlock();
    /*PQの先頭からパズルのボードを取り出す*/

    Board tmp = x_board.second;
    int f = x_board.first;

    mtx.lock();
    if (ans)
    {
      mtx.unlock();
      return;
    }
    mtx.unlock();

    if (tmp == answer)
    {
      printf("success!\n");
      ans = true;
      return;
    }
    /*もし先頭のパズルのボードが正しければ終了*/
    else
    {
      /*先頭のパズルのボードが正しくなければ*/

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

        mtx.lock();
        if (V[y])
        {
          mtx.unlock();
          continue;
        }
        mtx.unlock();

        mtx.lock();
        V[y] = true;
        PQ.push(make_pair(depth + 1 + h(y), y));
        mtx.unlock();
      }
    }
  }

  return;
}

void parallel(void)
{
  Board board;
  board.input();
  /*初期状態のボードを取り込む*/

  PQ.push(make_pair(h(board), board));
  V[board] = true;

  vector<thread> ths(2);

  for (auto &th : ths)
  {
    th = thread(psearch);
  }

  for (auto &t : ths)
  {
    t.join();
  }
}

int main(void)
{
  parallel();

  return 0;
}
