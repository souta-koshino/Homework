#include <iostream>
#include <queue>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <chrono>
using namespace std;
/*各種ライブラリをインクルード*/

/*パズルのボード上の空いている部分(以下では空白)を-1と呼ぶことにする*/

class Board
{
private:
  int X[16]; /*配列Xがパズルのボードを表す*/
  int blank; /*空白の場所を表す変数*/

public:
  Board();                   /*パズルのボードを作成するコンストラクタ*/
  void show();               /*パズルのボードを表示するメソッド*/
  void input();              /*初期状態のパズルのボードを入力するメソッド*/
  friend void search(Board); /*uniform cost searchを行う(フレンド)関数*/
  bool operator==(Board);    /*ボードが等しいことを表す演算子(オーバーロード)*/
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
  for (int i = 0; i < 16; i++)
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
      printf(" ");
    else
      printf("\n");
    printf("%d", X[i]);
  }
  printf("\n");
}
/*パズルのボードを表示するメソッド*/

int input_;

void Board::input()
{
  for (int i = 0; i < 16; i++)
  {
    cin >> input_;
    if (input_ == 0)
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
    {
      count += abs(x.X[i] / 4 - i / 4) + abs(x.X[i] % 4 - i % 4);
    }
  }
  return count;
}
/*マンハッタン距離を返すヒューリスティック関数。x座標はindex/3で、y座標はindex%3で与えられるので、正解の一との絶対値の差を合計する。*/

Board tmp;
Board y;
int f;
int depth;
/*以下の関数で使う変数*/

void search(Board x)
{
  chrono::system_clock::time_point start, end;

  start = chrono::system_clock::now();

  priority_queue<pair<int, Board>, vector<pair<int, Board> >, greater<pair<int, Board> > > PQ;
  map<Board, bool> V;
  /*探索をするときに必要なプライオリティキューであるPQ。
  　すでに探索された状態を再度探索しないために既に訪れた状態を記憶しておく必要がある。mapという対応関係(連想配列)を表すライブラリを使う。mapの名前はVとした。*/

  PQ.push(make_pair(h(x), x));
  V[x] = true;
  /*一番最初に与えられた状態(とそのヒューリスティック関数)をPQとVに入れる。*/

  int number = 0;

  int dx[] = {-1, 0, 1, 0};
  int dy[] = {0, -1, 0, 1};

  while (!PQ.empty())
  {
    /*PQがなくなるまで以下を続ける*/

    tmp = PQ.top().second;
    f = PQ.top().first;
    PQ.pop();
    /*PQの先頭からパズルのボードを取り出す*/

    if (tmp == answer)
    {
      tmp.show();

      end = std::chrono::system_clock::now();

      double tim = chrono::duration_cast<chrono::seconds>(end - start).count();

      int number_of_nodes_per_second = number / tim;

      printf("生成されたノードの総数は %d です\n", number);
      printf("かかった時間は %.0f (sec) です\n", tim);

      printf("一秒間に展開されるノードの数は ");
      printf("%d", number_of_nodes_per_second);
      printf(" です。\n");

      return;
    }
    /*もし先頭のパズルのボードが正しければ終了*/
    else
    {
      /*先頭のパズルのボードが正しくなければ*/

      /*以下は課題1のbのプログラムと同じ。*/
      int bx = tmp.blank / 4;
      int by = tmp.blank % 4;

      for (int i = 0; i < 4; i++)
      {
        int tx = bx + dx[i];
        int ty = by + dy[i];

        if (tx < 0 || tx >= 4 || ty < 0 || ty >= 4)
          continue;

        y = tmp;
        depth = f - h(y);
        swap(y.X[tmp.blank], y.X[tx * 4 + ty]);
        y.blank = tx * 4 + ty;

        if (!V[y])
        {
          V[y] = true;
          PQ.push(make_pair(depth + 1 + h(y), y));
          y.show();
          number++;
        }
      }
    }
  }
  cout << "Failed";
  return;
}

int main(void)
{
  Board board;
  board.input();
  /*初期状態のボードを取り込む*/

  search(board);
  /*探索をする*/

  return 0;
}
