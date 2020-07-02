#include <iostream>
#include <queue>
#include <set>
#include <algorithm>
#include <cstdlib>
using namespace std;
/*各種ライブラリをインクルード*/

/*パズルのボード上の空いている部分(以下では空白)を-1と呼ぶことにする*/

class Board
{
private:
  int X[3][3];      /*多次元配列Xがパズルのボードを表す*/
  pair<int, int> b; /*空白の位置をpairで表現する。変数名はbとした*/

public:
  Board();                     /*パズルのボードを作成するコンストラクタ*/
  void show();                 /*パズルのボードを表示するメソッド*/
  void input();                /*初期状態のパズルのボードを入力するメソッド*/
  int pattern();               /*空白の位置を表すメソッド。0~8の整数を返す。*/
  friend void search(Board x); /*uniform cost searchを行う(フレンド)関数*/
  bool operator==(Board bo);   /*ボードが等しいことを表す演算子(オーバーロード)*/
  bool operator<(const Board &bo) const
  {
    return X[0][0] < bo.X[0][0];
  }
  /*ヒューリスティック関数が同じだった場合はどの順で探索すればいいかがわからない。
  なので、適当にX[0][0]の数字で順番を決める。
  その時に必要な比較の演算子(オーバーロード)*/
  friend int h(Board x); /*ヒューリスティックス関数*/
};
/*パズルのボードを表すクラスを定義する*/

Board::Board()
{
  X[0][0] = -1;
  X[0][1] = 1;
  X[0][2] = 2;
  X[1][0] = 3;
  X[1][1] = 4;
  X[1][2] = 5;
  X[2][0] = 6;
  X[2][1] = 7;
  X[2][2] = 8;
  b.first = 0;
  b.second = 0;
}
/*パズルのボードを作成するコンストラクタ*/
/*Russell & Norvingで初回されていたボードの正解の状態を標準としている*/

bool Board::operator==(Board bo)
{
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      if (X[i][j] != bo.X[i][j])
        return false;
    }
  }
  return true;
}
/*ボードの状態が正しいかどうかをbool値で返す演算子。==演算子をオーバーロードする形になっている。*/

void Board::show()
{
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      cout << X[i][j] << " ";
    }
    cout << endl;
  }
  cout << endl;
}
/*パズルのボードを表示するメソッド*/

int input_;
void Board::input()
{
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      cin >> input_;
      X[i][j] = input_;
    }
  }
  return;
}
/*初期状態のパズルのボードを入力するメソッド*/

int Board::pattern()
{
  if (b.first == 0 && b.second == 0)
    return 0;
  else if (b.first == 0 && b.second == 1)
    return 1;
  else if (b.first == 0 && b.second == 2)
    return 2;
  else if (b.first == 1 && b.second == 0)
    return 3;
  else if (b.first == 1 && b.second == 1)
    return 4;
  else if (b.first == 1 && b.second == 2)
    return 5;
  else if (b.first == 2 && b.second == 0)
    return 6;
  else if (b.first == 2 && b.second == 1)
    return 7;
  else
    return 8;
}
/*空白の位置に応じて0~8の整数を返すメソッド*/

/*
空白の位置に応じて以下の値が返される。
012
345
678

例えば、(*は何らかの数)
***
**-1
***
の場合は、5が返ってくる。
*/

Board answer;
/*答えの状態を表すパズルのボード*/

int h(Board x)
{
  int count = 0;
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      if (x.X[i][j] != answer.X[i][j])
        count++;
    }
  }
  return count;
}
/*answerとピースの位置が違うものをカウント*/

Board tmp;
Board y;
Board tmpp;
int test;
int depth;
int f;
/*以下の関数で使う変数*/

void search(Board x)
{
  priority_queue<pair<int, Board>, vector<pair<int, Board> >, greater<pair<int, Board> > > PQ;
  vector<Board> visited;
  /*幅優先探索をするときに必要なプライオリティーキューであるPQ。PQには、pair<int,Board>を入れる。pairのfirstの値を比較し、小さなものから取り出していくが、もしpairのfirstの値が同じだった場合は、classの部分で書いた通り、同じものの中から適当に取り出される。
  　すでに探索された状態を再度探索しないために既に訪れた状態を記憶しておく必要がある。記憶の領域をvectorにして、
    visitedと名付けた。*/

  PQ.push(make_pair(h(x), x));
  visited.push_back(x);
  /*一番最初に与えられた状態をPQとvisitedに入れる。*/

  int di[] = {0, -1, 0, 1};
  int dj[] = {1, 0, -1, 0};
  /*以下では、空白と空白ではないピースを交換して新しい状態を探索することになる。
  　空白ではないピースの候補な複数個存在するため、どのピースを交換するかは全探索をすることになる。
  　交換するピースの方向を分かりやすくするために、di[]とdj[]に交換するピースの方向を記憶しておく。
  　以下のように図式化される*/
  /*
     1
     ↑
  2← B →0
     ↓
     3
    もし、空白の左隣のピースと交換したかったら、b.first+di[2]とb.second+dj[2]で指定されるピースを交換する。
  */

  vector<Board>::iterator ite;
  /*後々使うvectorのイテレータ*/

  /*ここからかなりスパゲッティコーディングになります。。。ごめんなさい。。。*/
  while (!PQ.empty())
  {
    /*Qがなくなるまで以下を続ける*/

    tmp = (PQ.top()).second;
    f = (PQ.top()).first;
    PQ.pop();
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

      test = tmp.pattern();
      /*先頭のパズルのボードの空白の位置をtestにいれる。*/

      /*testの値に応じて、空白と交換できるピースの場所が異なるため、switch文で場合分けする。*/
      switch (test)
      {
      case 0:
        y = tmp;
        depth = f - h(y);
        /*yにtmpを写す*/
        swap(y.X[y.b.first][y.b.second], y.X[y.b.first + di[0]][y.b.second + dj[0]]);
        /*右隣と交換できるため、swapを使って交換する*/
        y.b.first = y.b.first + di[0];
        y.b.second = y.b.second + dj[0];
        /*空白の位置を更新する。*/
        ite = find(visited.begin(), visited.end(), y);
        /*visitedに更新した結果がないか確認*/
        if (ite == visited.end())
        /*もしまだvisitedにない、即ち未探索のパズルのボードならQとvisitedに入れる。*/
        {
          PQ.push(make_pair(h(y) + depth + 1, y));
          visited.push_back(y);
          y.show();
        }
        /*以下は同じ操作をする*/
        /*本当にスパゲッティコーディングですみません。。。*/

        y = tmp;
        depth = f - h(y);
        swap(y.X[y.b.first][y.b.second], y.X[y.b.first + di[3]][y.b.second + dj[3]]);
        y.b.first = y.b.first + di[3];
        y.b.second = y.b.second + dj[3];
        ite = find(visited.begin(), visited.end(), y);
        if (ite == visited.end())
        {
          PQ.push(make_pair(h(y) + depth + 1, y));
          visited.push_back(y);
          y.show();
        }

        break;

      case 1:
        y = tmp;
        depth = f - h(y);
        swap(y.X[y.b.first][y.b.second], y.X[y.b.first + di[0]][y.b.second + dj[0]]);
        y.b.first = y.b.first + di[0];
        y.b.second = y.b.second + dj[0];
        ite = find(visited.begin(), visited.end(), y);
        if (ite == visited.end())
        {
          PQ.push(make_pair(h(y) + depth + 1, y));
          visited.push_back(y);
          y.show();
        }
        y = tmp;
        depth = f - h(y);
        swap(y.X[y.b.first][y.b.second], y.X[y.b.first + di[2]][y.b.second + dj[2]]);
        y.b.first = y.b.first + di[2];
        y.b.second = y.b.second + dj[2];
        ite = find(visited.begin(), visited.end(), y);
        if (ite == visited.end())
        {
          PQ.push(make_pair(h(y) + depth + 1, y));
          visited.push_back(y);
          y.show();
        }
        y = tmp;
        depth = f - h(y);
        swap(y.X[y.b.first][y.b.second], y.X[y.b.first + di[3]][y.b.second + dj[3]]);
        y.b.first = y.b.first + di[3];
        y.b.second = y.b.second + dj[3];
        ite = find(visited.begin(), visited.end(), y);
        if (ite == visited.end())
        {
          PQ.push(make_pair(h(y) + depth + 1, y));
          visited.push_back(y);
          y.show();
        }
        break;

      case 2:
        y = tmp;
        depth = f - h(y);
        swap(y.X[y.b.first][y.b.second], y.X[y.b.first + di[2]][y.b.second + dj[2]]);
        y.b.first = y.b.first + di[2];
        y.b.second = y.b.second + dj[2];
        ite = find(visited.begin(), visited.end(), y);
        if (ite == visited.end())
        {
          PQ.push(make_pair(h(y) + depth + 1, y));
          visited.push_back(y);
          y.show();
        }
        y = tmp;
        depth = f - h(y);
        swap(y.X[y.b.first][y.b.second], y.X[y.b.first + di[3]][y.b.second + dj[3]]);
        y.b.first = y.b.first + di[3];
        y.b.second = y.b.second + dj[3];
        ite = find(visited.begin(), visited.end(), y);
        if (ite == visited.end())
        {
          PQ.push(make_pair(h(y) + depth + 1, y));
          visited.push_back(y);
          y.show();
        }
        break;

      case 3:
        y = tmp;
        depth = f - h(y);
        swap(y.X[y.b.first][y.b.second], y.X[y.b.first + di[0]][y.b.second + dj[0]]);
        y.b.first = y.b.first + di[0];
        y.b.second = y.b.second + dj[0];
        ite = find(visited.begin(), visited.end(), y);
        if (ite == visited.end())
        {
          PQ.push(make_pair(h(y) + depth + 1, y));
          visited.push_back(y);
          y.show();
        }
        y = tmp;
        depth = f - h(y);
        swap(y.X[y.b.first][y.b.second], y.X[y.b.first + di[1]][y.b.second + dj[1]]);
        y.b.first = y.b.first + di[1];
        y.b.second = y.b.second + dj[1];
        ite = find(visited.begin(), visited.end(), y);
        if (ite == visited.end())
        {
          PQ.push(make_pair(h(y) + depth + 1, y));
          visited.push_back(y);
          y.show();
        }
        y = tmp;
        depth = f - h(y);
        swap(y.X[y.b.first][y.b.second], y.X[y.b.first + di[3]][y.b.second + dj[3]]);
        y.b.first = y.b.first + di[3];
        y.b.second = y.b.second + dj[3];
        ite = find(visited.begin(), visited.end(), y);
        if (ite == visited.end())
        {
          PQ.push(make_pair(h(y) + depth + 1, y));
          visited.push_back(y);
          y.show();
        }
        break;

      case 4:
        y = tmp;
        depth = f - h(y);
        swap(y.X[y.b.first][y.b.second], y.X[y.b.first + di[0]][y.b.second + dj[0]]);
        y.b.first = y.b.first + di[0];
        y.b.second = y.b.second + dj[0];
        ite = find(visited.begin(), visited.end(), y);
        if (ite == visited.end())
        {
          PQ.push(make_pair(h(y) + depth + 1, y));
          visited.push_back(y);
          y.show();
        }
        y = tmp;
        depth = f - h(y);
        swap(y.X[y.b.first][y.b.second], y.X[y.b.first + di[1]][y.b.second + dj[1]]);
        y.b.first = y.b.first + di[1];
        y.b.second = y.b.second + dj[1];
        ite = find(visited.begin(), visited.end(), y);
        if (ite == visited.end())
        {
          PQ.push(make_pair(h(y) + depth + 1, y));
          visited.push_back(y);
          y.show();
        }
        y = tmp;
        depth = f - h(y);
        swap(y.X[y.b.first][y.b.second], y.X[y.b.first + di[2]][y.b.second + dj[2]]);
        y.b.first = y.b.first + di[2];
        y.b.second = y.b.second + dj[2];
        ite = find(visited.begin(), visited.end(), y);
        if (ite == visited.end())
        {
          PQ.push(make_pair(h(y) + depth + 1, y));
          visited.push_back(y);
          y.show();
        }
        y = tmp;
        depth = f - h(y);
        swap(y.X[y.b.first][y.b.second], y.X[y.b.first + di[3]][y.b.second + dj[3]]);
        y.b.first = y.b.first + di[3];
        y.b.second = y.b.second + dj[3];
        ite = find(visited.begin(), visited.end(), y);
        if (ite == visited.end())
        {
          PQ.push(make_pair(h(y) + depth + 1, y));
          visited.push_back(y);
          y.show();
        }
        break;

      case 5:
        y = tmp;
        depth = f - h(y);
        swap(y.X[y.b.first][y.b.second], y.X[y.b.first + di[1]][y.b.second + dj[1]]);
        y.b.first = y.b.first + di[1];
        y.b.second = y.b.second + dj[1];
        ite = find(visited.begin(), visited.end(), y);
        if (ite == visited.end())
        {
          PQ.push(make_pair(h(y) + depth + 1, y));
          visited.push_back(y);
          y.show();
        }
        y = tmp;
        depth = f - h(y);
        swap(y.X[y.b.first][y.b.second], y.X[y.b.first + di[2]][y.b.second + dj[2]]);
        y.b.first = y.b.first + di[2];
        y.b.second = y.b.second + dj[2];
        ite = find(visited.begin(), visited.end(), y);
        if (ite == visited.end())
        {
          PQ.push(make_pair(h(y) + depth + 1, y));
          visited.push_back(y);
          y.show();
        }
        y = tmp;
        depth = f - h(y);
        swap(y.X[y.b.first][y.b.second], y.X[y.b.first + di[3]][y.b.second + dj[3]]);
        y.b.first = y.b.first + di[3];
        y.b.second = y.b.second + dj[3];
        ite = find(visited.begin(), visited.end(), y);
        if (ite == visited.end())
        {
          PQ.push(make_pair(h(y) + depth + 1, y));
          visited.push_back(y);
          y.show();
        }
        break;

      case 6:
        y = tmp;
        depth = f - h(y);
        swap(y.X[y.b.first][y.b.second], y.X[y.b.first + di[0]][y.b.second + dj[0]]);
        y.b.first = y.b.first + di[0];
        y.b.second = y.b.second + dj[0];
        ite = find(visited.begin(), visited.end(), y);
        if (ite == visited.end())
        {
          PQ.push(make_pair(h(y) + depth + 1, y));
          visited.push_back(y);
          y.show();
        }
        y = tmp;
        depth = f - h(y);
        swap(y.X[y.b.first][y.b.second], y.X[y.b.first + di[1]][y.b.second + dj[1]]);
        y.b.first = y.b.first + di[1];
        y.b.second = y.b.second + dj[1];
        ite = find(visited.begin(), visited.end(), y);
        if (ite == visited.end())
        {
          PQ.push(make_pair(h(y) + depth + 1, y));
          visited.push_back(y);
          y.show();
        }
        break;

      case 7:
        y = tmp;
        depth = f - h(y);
        swap(y.X[y.b.first][y.b.second], y.X[y.b.first + di[0]][y.b.second + dj[0]]);
        y.b.first = y.b.first + di[0];
        y.b.second = y.b.second + dj[0];
        ite = find(visited.begin(), visited.end(), y);
        if (ite == visited.end())
        {
          PQ.push(make_pair(h(y) + depth + 1, y));
          visited.push_back(y);
          y.show();
        }
        y = tmp;
        depth = f - h(y);
        swap(y.X[y.b.first][y.b.second], y.X[y.b.first + di[1]][y.b.second + dj[1]]);
        y.b.first = y.b.first + di[1];
        y.b.second = y.b.second + dj[1];
        ite = find(visited.begin(), visited.end(), y);
        if (ite == visited.end())
        {
          PQ.push(make_pair(h(y) + depth + 1, y));
          visited.push_back(y);
          y.show();
        }
        y = tmp;
        depth = f - h(y);
        swap(y.X[y.b.first][y.b.second], y.X[y.b.first + di[2]][y.b.second + dj[2]]);
        y.b.first = y.b.first + di[2];
        y.b.second = y.b.second + dj[2];
        ite = find(visited.begin(), visited.end(), y);
        if (ite == visited.end())
        {
          PQ.push(make_pair(h(y) + depth + 1, y));
          visited.push_back(y);
          y.show();
        }
        break;

      case 8:
        y = tmp;
        depth = f - h(y);
        swap(y.X[y.b.first][y.b.second], y.X[y.b.first + di[1]][y.b.second + dj[1]]);
        y.b.first = y.b.first + di[1];
        y.b.second = y.b.second + dj[1];
        ite = find(visited.begin(), visited.end(), y);
        if (ite == visited.end())
        {
          PQ.push(make_pair(h(y) + depth + 1, y));
          visited.push_back(y);
          y.show();
        }
        y = tmp;
        depth = f - h(y);
        swap(y.X[y.b.first][y.b.second], y.X[y.b.first + di[2]][y.b.second + dj[2]]);
        y.b.first = y.b.first + di[2];
        y.b.second = y.b.second + dj[2];
        ite = find(visited.begin(), visited.end(), y);
        if (ite == visited.end())
        {
          PQ.push(make_pair(h(y) + depth + 1, y));
          visited.push_back(y);
          y.show();
        }
        break;

      default:
        break;

        break;
      }
    }
  }
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
