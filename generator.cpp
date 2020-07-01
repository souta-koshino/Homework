#include <iostream>
#include <random>
#include <ctime>
#include <cstdlib>
using namespace std;
/*各種ライブラリをインクルード*/

/*パズルのボード上の空いている部分(以下では空白)を-1で表すことにする*/

class Board
{
private:
  int X[3][3];      /*多次元配列Xがパズルのボードを表す*/
  pair<int, int> b; /*空白の位置をpairで表現する。変数名はbとした*/

public:
  Board();           /*パズルのボードのコンストラクタ*/
  void show();       /*パズルのボードを表示するメソッド*/
  void shuffle(int); /*パズルのボードをシャッフルするメソッド*/
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
/*パズルのボードを表す多次元配列に初期の数字を与える。また、空白の位置は左上である*/

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
}
/*パズルのボードを表示するメソッド*/

void Board::shuffle(int tim)
{
  srand((unsigned int)time(NULL) + tim);
  /*以下では乱数を取り扱う、乱数の初期値は関数の引数によって異なるようにした*/
  /*これできちんとした乱数になっているかが少し不安*/

  int N = rand() % 200 + 100;
  /*空白部分と数字を交換する操作を複数回行うことになるが、それを何回行うかを決める変数をNとした。んなボードの状態からでも平均的にはど26回程度で到達できると聞いたので、最低の交換回数は100回確保しておけば完全にシャッフルされているかなという気持ちからNの値を上記のように設定した*/
  /*本当にこれで正しいのか不安*/

  int n;
  /*後々使う乱数を表す変数n*/

  pair<int, int> a;
  /*空白と空白でないピースを交換するときの、空白でないピースの位置*/

  while (N-- >= 0)
  {
    /***以下ではスパゲッティコードになります。。。申し訳ございません。。。***/
    if ((b.first == 0 && b.second == 0) || (b.first == 0 && b.second == 2) || (b.first == 2 && b.second == 0) || (b.first == 2 && b.second == 2))
    /*パズルのボードの隅に空白がある場合*/
    /*もっと分かりやすい書き方があるはず*/
    {
      a = b;

      n = rand() % 2;

      if (n == 0)
      {
        a.first = 1;
      }
      /*空白が確率1/2で縦に移動する*/

      else if (n == 1)
      {
        a.second = 1;
      }
      /*空白が確率1/2で横に移動する*/

      swap(X[a.first][a.second], X[b.first][b.second]);
      /*空白と空白でないピースを交換する*/
    }

    else if (b.first == 1 && b.second == 1)
    /*空白が真ん中にある場合*/
    {
      a = b;

      n = rand() % 4;

      if (n == 0)
      {
        a.first++;
      }
      else if (n == 1)
      {
        a.first--;
      }
      else if (n == 2)
      {
        a.second++;
      }
      else if (n == 3)
      {
        a.second--;
      }
      /*空白が確率1/4で上下左右のいずれかに移動する*/

      swap(X[a.first][a.second], X[b.first][b.second]);
      /*空白と空白でないピースを交換する*/
    }

    else if ((b.first == 0 && b.second == 1) || (b.first == 1 && b.second == 0) || (b.first == 1 && b.second == 2) || (b.first == 2 && b.second == 1))
    /*それ以外の場合*/
    /*
    つまり
    010
    101
    010
    空白が上記の図の1のいずれかにある場合
    */

    /*
   以下では次のようにパターン分けをしている
   0A0
   B0B
   0A0
   */
    {
      n = rand() % 3;

      a = b;

      if (n == 0)
      {
        a.first = 1;
        a.second = 1;
      }
      /*確率1/3で真ん中へ*/

      else if (n == 1)
      /*確率1/3で…*/
      {
        if ((b.first == 0 && b.second == 1) || (b.first == 2 && b.second == 1))
        {
          a.second--;
          /*Aパターンの時は左に移動*/
        }
        else
        {
          a.first--;
          /*Bパターンの時は上に移動*/
        }
      }

      else if (n == 2)
      /*確率1/3で…*/
      {
        if ((b.first == 0 && b.second == 1) || (b.first == 2 && b.second == 1))
        {
          a.second++;
          /*Aパターンの時は右に移動*/
        }
        else
        {
          a.first++;
          /*Bパターンの時は下に移動*/
        }
      }

      swap(X[a.first][a.second], X[b.first][b.second]);
      /*空白と空白でないピースを交換する*/
    }

    b = a;
    /*空白の位置を更新する*/
  }
}

int main(void)
{
  int how_many_boards;

  cout << "何枚のボードを作成しますか?\n";
  cin >> how_many_boards;

  for (int count_ = 1; count_ <= how_many_boards; count_++)
  {
    Board board;

    board.shuffle(count_);

    cout << "Board # " << count_ << endl;
    board.show();
    cout << endl
         << endl;
  }
  return 0;
}
