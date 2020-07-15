#include <iostream>
#include <cstdio>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

const int Ntiles = 16;
const int Width = 4;
#define LIMIT 100
/*LIMITはdeepeningの限界(f=g+hのfの限界)を表します。*/

int manhattan_dist(int X[16])
{
  int count = 0;
  for (int i = 0; i < 16; i++)
  {
    {
      count += abs(X[i] / 4 - i / 4) + abs(X[i] % 4 - i % 4);
    }
  }
  return count;
}
/*マンハッタン距離を計算するプログラムです。*/
/*空白は0で表すことにしたので、空白でない数字と同じように扱います。*/

int input_;
/*入力の際に一時的に使う変数です。*/

/*パズルの盤面の状態を表すクラスです。*/
class State
{
private:
  int X[16];
  int blank;
  int h;
  /*Xがパズルの盤面を表す配列、blankが空白の位置を表す変数、hがマンハッタン距離を表す変数です。*/

public:
  State()
  {
    for (int i = 0; i < 16; i++)
    {
      X[i] = i;
    }
    blank = 0;
    h = 0;
  }
  /*コンストラクタです。*/

  void show(int x)
  {
    cout << "State " << x << " :";
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
  /*パズルの盤面を表示するメソッドです。*/

  void input(void)
  {
    for (int i = 0; i < 16; i++)
    {
      cin >> input_;
      if (input_ == 0)
        blank = i;
      X[i] = input_;
    }
    h = manhattan_dist(X);
    return;
  }
  /*パズルの初期状態を入力するメソッドです。blankもhも同時に入力します。*/

  bool isgoal(void)
  {
    for (int i = 0; i < 16; i++)
      if (X[i] != i)
        return false;
    return true;
  }
  /*パズルの盤面が正解かどうかをbool値で返す関数です。*/

  friend int NOPS(State);

  friend int NTHOP(State, int);

  friend State *APPLY(State &, int);

  friend int mdincr(int, int, int);

  friend void UNDO(State &, State *);

  friend bool dfs(State, int, int, int);

  friend bool search(State);
};

typedef class State *Undo;
/*論文で紹介されていたOpaque Pointerです。*/

/*以下のクラスは、論文の"Operator Pre-computation"の章に書かれていたoperator tableを表すクラスです。*/
class Optab : public State
{
private:
  vector<int> Y[16];
  /*Y[i]がblankがiの時にblankを交換させることのできるピースの位置を保存するvectorです。*/
  /*例えば、Y[0]の場合だと、blank=0は左上を指しますから、1と4が入っています。*/

public:
  Optab()
  {
    for (int i = 0; i < 16; i++)
    {
      if (i >= Width)
      {
        Y[i].push_back(i - Width);
      }
      if (i % Width > 0)
      {
        Y[i].push_back(i - 1);
      }
      if (i % Width < Width - 1)
      {
        Y[i].push_back(i + 1);
      }
      if (i < Ntiles - Width)
      {
        Y[i].push_back(i + Width);
      }
    }
  }
  /*論文の通りに実装したつもりです*/

  int number(int i)
  {
    return Y[i].size();
  }
  /*Y[i]に入っている数字の個数(=移動可能なパズルの状態の個数)を返すメソッドnumber*/

  int op(int i, int j)
  {
    return Y[i][j];
  }
  /*Y[i]のj番目に入っている数字を返すメソッドop*/
};

Optab optab;
/*一度Optabのインスタンスを作成して、それを使い回せば、いちいち4つのif文の処理をしていないことになります。*/

int NOPS(State s)
{
  return optab.number(s.blank);
}
/*論文のFigure 3で書かれていた関数のNOPSです*/

int NTHOP(State s, int n)
{
  return optab.op(s.blank, n);
}
/*論文のFigure 3で書かれていた関数のNTHOPSです*/

int mdincr(int tile, int newblank, int blank)
{
  return (abs(newblank % 4) + abs(newblank / 4) + abs(tile / 4 - blank / 4) + abs(tile % 4 - blank % 4) - abs(blank % 4) - abs(blank / 4) - abs(tile % 4 - newblank % 4) - abs(tile / 4 - newblank / 4));
}
/*論文のFigure 3で書かれていた関数のmdincrです。これを使うことで、交換されたピースのみマンハッタン距離を計算することができ、いちいち全ての盤面についてマンハッタン距離を計算する必要がなくなります。*/

State *APPLY(State &x, int newblank)
{
  Undo u = new State;

  u->h = x.h;
  u->blank = x.blank;

  for (int j = 0; j < 16; j++)
  {
    u->X[j] = x.X[j];
  }

  swap(x.X[x.blank], x.X[newblank]);
  /*ここでsegmentation faultが起こっているみたいです。*/

  x.h += mdincr(x.X[x.blank], newblank, x.blank);
  x.blank = newblank;

  return u;
}
/*論文のFigure 3で書かれていた関数のAPPLYです。StateのポインタであるUndoを作成して、それが指すStateにxを保存します。そして、xにピースの交換の操作を施した後、Undoを返す仕様になっています。*/

void UNDO(State &x, Undo u)
{
  for (int i = 0; i < 16; i++)
  {
    x.X[i] = u->X[i];
  }

  x.h = u->h;
  x.blank = u->blank;

  delete u;
}
/*論文のFigure 3で書かれていた関数のUNDOです。xをUndo uの状態と同じにする、つまり交換された状態を元に戻します。そして、使い終わったUndo uをdeleteで削除します。*/

int nops;
vector<State> vec;
int tmp;
/*nopsとtmpは以下のdfsで使う変数です、vecは成功した場合のパスとして、交換過程を保存するvector<State>です。*/

bool dfs(State x, int depth, int limit, int prev)
{
  if (x.isgoal())
  {
    return true;
  }
  /*もらったState xが正しい状態ならtrueを返して終了です。*/

  if (depth + x.h > limit)
  {
    return false;
  }
  /*xのf=g+hがlimitを超えたらその状態はダメなのでfalseを返します。*/

  nops = NOPS(x);
  /*State xから一回の交換によっていける状態の数をNOPSを使って計算します。それをnopsに入れます。*/

  for (int i = 0; i < nops; i++)
  {
    Undo u;
    /*xがもしダメだった場合に戻るためのUndo uをここで定義します。*/

    tmp = NTHOP(x, i);
    /*newblank、つまり、現在のxのblankと交換してこれからblankになる場所を表す数字をtmpに入れます。*/

    u = APPLY(x, tmp);
    /*APPLYを使って交換します。xがダメだった時に戻れるように、uにxの交換前の状態を入れておきます。*/

    if (x.blank == prev)
    {
      UNDO(x, u);
      continue;
      /*prevは、xの前のblankを表します。もしこれが今のxのblankと同じだったら、論文でいう2-Cycleの状態になりますので、非効率です。なのでそれを抑制します。*/
    }

    if (dfs(x, depth + 1, limit, u->blank))
    {
      /*depthには、現在のdepthに１足したもの、prevには、交換前のblankの位置、つまりu->blankを入れて再帰の計算をします。もし成功したら、vecに入れて、true(=深さ優先探索が成功したことを意味する)を返します。*/
      vec.push_back(x);
      return true;
    }

    UNDO(x, u);
    /*この交換(iで表されている)の先の全ての探索が失敗したら、xを元に戻します。*/
  }
  return false;
  /*もし、全ての交換についての探索が失敗したら、深さ優先探索が失敗したことを表すfalseを返します。*/
}

bool search(State x)
{
  for (int limit = x.h; limit <= LIMIT; limit++)
  /*limitを1ずつインクリメントしてlimitをdeepeningします。本当は1ずつのインクリメントである必要はなかったように思いますが、どのように実装するべきかわからなかったので、このような仕様にしました。*/
  {
    if (dfs(x, 0, limit, -1))
    /*dfsにdepthは0に、prevは存在しないので-1にして深さ優先探索をします。*/
    {
      return true;
    }
    else
    {
      continue;
    }
  }
  return false;
  /*全ての深さ優先探索が失敗したらfalseで終了します。*/
}

int t;

int main(void)
{
  State x;
  x.input();

  if (search(x))
  {
    int count = 0;

    reverse(vec.begin(), vec.end());

    for (vector<State>::iterator ite = vec.begin(); ite != vec.end(); ite++)
    {
      count++;
      (*ite).show(count);
    }
    /*もしIDAsearchがうまくいったら、vecの中身をプリントして、交換過程を表示します。*/
  }
  else
    cout << "fail";

  return 0;
}
