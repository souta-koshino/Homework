#include <iostream>
#include <queue>
#include <map>
#include <algorithm>
#include <cstdlib>
using namespace std;
/*各種ライブラリをインクルード*/

/*パズルのボード上の空いている部分(以下では空白)を-1と呼ぶことにする*/

class Board
{
private:
  int X[9];  /*配列Xがパズルのボードを表す*/
  int blank; /*空白の場所を表す変数*/

public:
  Board();                   /*パズルのボードを作成するコンストラクタ*/
  void show();               /*パズルのボードを表示するメソッド*/
  void input();              /*初期状態のパズルのボードを入力するメソッド*/
  friend void search(Board); /*uniform cost searchを行う(フレンド)関数*/
  bool operator==(Board);    /*ボードが等しいことを表す演算子(オーバーロード)*/
  bool operator<(const Board &b) const
  {
    for (int i = 0; i < 9; i++)
    {
      if (X[i] == b.X[i])
        continue;
      return X[i] > b.X[i];
    }
    return false;
  } /*mapを使用するにはBoardの順序が必要なので適当に定義しておく。*/
};
/*パズルのボードを表すクラスを定義する*/

Board::Board()
{
  X[0] = -1;
  for (int i = 1; i < 9; i++)
    X[i] = i;
}
/*パズルのボードを作成するコンストラクタ*/
/*Russell & Norvingで初回されていたボードの正解の状態を標準としている*/

bool Board::operator==(Board bo)
{
  for (int i = 0; i < 9; i++)
  {
    if (X[i] != bo.X[i])
      return false;
  }
  return true;
}
/*ボードの状態が正しいかどうかをbool値で返す演算子。==演算子をオーバーロードする形になっている。*/

void Board::show()
{
  for (int i = 0; i < 9; i++)
  {
    if (i % 3 != 0)
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
  for (int i = 0; i < 9; i++)
  {
    cin >> input_;
    if (input_ == -1)
      blank = i;
    X[i] = input_;
  }
  return;
}
/*初期状態のパズルのボードを入力するメソッド*/

Board tmp;
Board y;
Board answer;
/*以下の関数で使う変数*/

void search(Board x)
{
  queue<Board> Q;
  map<Board, bool> V;
  /*幅優先探索をするときに必要なキューであるQ。
  　すでに探索された状態を再度探索しないために既に訪れた状態を記憶しておく必要がある。mapという対応関係(連想配列)を表すライブラリを使う。mapの名前はVとした。*/

  Q.push(x);
  V[x] = true;
  /*一番最初に与えられた状態をQとVに入れる。*/

  int dx[] = {-1, 0, 1, 0};
  int dy[] = {0, -1, 0, 1};

  while (!Q.empty())
  {
    /*Qがなくなるまで以下を続ける*/

    tmp = Q.front();
    Q.pop();
    /*Qの先頭からパズルのボードを取り出す*/

    if (tmp == answer)
    {
      tmp.show();
      return;
    }
    /*もし先頭のパズルのボードが正しければ終了*/
    else
    {
      /*先頭のパズルのボードが正しくなければ*/

      int bx = tmp.blank / 3;
      int by = tmp.blank % 3;
      /*空白の(x,y)座標をbx,byとする。*/

      for (int i = 0; i < 4; i++)
      {
        int tx = bx + dx[i];
        int ty = by + dy[i];

        if (tx < 0 || tx >= 3 || ty < 0 || ty >= 3)
          continue;
        /*もしボードから外れる場合は、この場合のiを考えない。*/

        /*空白と交換したボードをyとする。*/
        y = tmp;
        swap(y.X[tmp.blank], y.X[tx * 3 + ty]);
        y.blank = tx * 3 + ty;

        if (!V[y])
        /*まだVの中にyがなかったら…*/
        {
          V[y] = true;
          Q.push(y);
          y.show();
        }
      }
    }
  }
  /*もし見つからなかったらfailedと表示する。*/
  cout << "failed";
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
