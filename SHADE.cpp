#include <iostream>
#include <random>
using namespace std;

const int G_MAX = 10000;
/*世代のマックス*/
const int D = 3;
/*問題の次元*/
const int N = 100;
/*一世代あたりの個体数*/
const int H = 20;
/*論文で紹介されていたHパラメータ*/

double f7(vector<double> x)
{
  double a = 0.0;

  for (int i = 0; i < D; i++)
  {
    a += (x[i] * x[i]) / 4000;
  }

  double b = 1.0;

  for (int i = 0; i < D; i++)
  {
    b *= cos(x[i] / sqrt(i + 1));
  }

  double tmp = a - b + 1.0;

  return tmp;
}
/*コンテストで紹介されていた初等的な関数f7、Griewank's Functionです。*/
* /

    void shade(void)
{
  int G = 0;

  vector<vector<vector<double>>> P(N, vector<vector<double>>(G_MAX, vector<double>(D)));
  /*全ての個体を保存する多次元配列*/

  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < D; j++)
    {
      P[i][0][j] = rand() % 200 - 100;
    }
  }
  /*[-100,100]の間に保管*/

  vector<vector<vector<double>>> V(N, vector<vector<double>>(G_MAX, vector<double>(D)));
  /*mutationを保存する多次元配列*/

  vector<vector<double>> MCR(H, vector<double>(G_MAX, 0.5));
  vector<vector<double>> MF(H, vector<double>(G_MAX, 0.5));

  vector<vector<vector<double>>> A(G_MAX, vector<vector<double>>(D));

  int k = 1;

  vector<vector<double>> CR(N, vector<double>(G_MAX));
  vector<vector<double>> F(N, vector<double>(G_MAX));
  vector<vector<double>> p(N, vector<double>(G_MAX));

  while (true)
  {
    vector<double> SCR;
    vector<double> SF;

    for (int i = 0; i < N; i++)
    {
      std::random_device seed_gen;
      std::default_random_engine engine(seed_gen());

      int r = rand() % H;

      std::normal_distribution<> dist1(MCR[r][G], 0.1);
      CR[i][G] = dist1(engine);

      std::cauchy_distribution<> dist2(MF[r][G], 0.1);
      F[i][G] = dist2(engine);

      std::uniform_real_distribution<> dist3(2 / N, 0.2);
      p[i][G] = dist3(engine);

      for (int j = 0; j < D; j++)
      {
        int r1 = rand() % N;
        int r2 = rand() % N;

        if (G == 0 || A[G - 1][j].size() == 0)
        {
          V[i][G][j] = P[i][G][j] + F[i][G] * (P[r1][G][j] - P[r2][G][j]);
        }
        else
        {
          int u = A[G - 1][j].size();
          int y = rand() % u;
          int t = A[G - 1][j][y];

          V[i][G][j] = P[i][G][j] + F[i][G] * (t - P[i][G][j]) + F[i][G] * (P[r1][G][j] - P[r2][G][j]);
        }
        /*ここの部分は論文の擬似コードと変えてあるためあっているか不明です*/
      }
    }

    for (int i = 0; i < N; i++)
    {
      if (f7(V[i][G]) <= f7(P[i][G]))
      {
        P[i][G + 1] = V[i][G];
      }
      else
      {
        P[i][G + 1] = P[i][G];
      }

      if (f7(V[i][G]) < f7(P[i][G]))
      {
        for (int j = 0; j < D; j++)
        {
          A[G][j].push_back(P[i][G][j]);
        }
        SCR.push_back(CR[i][G]);
        SF.push_back(F[i][G]);
      }
    }

    /*「Archiveのサイズを超えたらArchiveから個体を削除」と論文の擬似コードには書いてありましたが、Archiveを動的配列としたため、そのようなことはおこらないので無視しています*/
    if (SCR.size() != 0 && SF.size() != 0)
    {
      if (SCR.size() == 0)
      {
        MCR[k][G + 1] = MCR[k][G];
      }
      else
      {
        vector<double> w(SCR.size());
        double Sum = 0;
        for (int i = 0; i < SCR.size(); i++)
        {
          Sum += abs(f7(V[i][G]) - f7(P[i][G]));
        }

        for (int i = 0; i < SCR.size(); i++)
        {
          w[i] = abs(f7(V[i][G]) - f7(P[i][G])) / Sum;
        }

        double mean_SCR = 0.0;

        for (int i = 0; i < SCR.size(); i++)
        {
          mean_SCR += w[i] * SCR[i];
        }

        MCR[k][G + 1] = mean_SCR;
      }

      if (SF.size() == 0)
      {
        MF[k][G + 1] = MF[k][G];
      }
      else
      {
        vector<double> w(SF.size());
        double Sum = 0;

        for (int i = 0; i < SF.size(); i++)
        {
          Sum += abs(f7(V[i][G]) - f7(P[i][G]));
        }

        for (int i = 0; i < SF.size(); i++)
        {
          w[i] = abs(f7(V[i][G]) - f7(P[i][G])) / Sum;
        }

        double mean_SF = 0.0;

        for (int i = 0; i < SF.size(); i++)
        {
          mean_SF += (w[i] * SF[i] * SF[i]) / (w[i] * SF[i]);
        }

        MCR[k][G + 1] = mean_SF;
      }

      k++;

      if (k >= H)
      {
        k = 1;
      }
    }

    G++;

    if (G == G_MAX - 1)
    {
      G = G_MAX - 1;
      double Min = 1001001001;
      int t = 0;

      for (int i = 0; i < N; i++)
      {
        if (f7(P[i][G]) < Min)
        {
          t = i;
          Min = f7(P[i][G]);
        }
      }

      cout << "最小値は" << Min << "でした\n";

      cout << "最小化元は" << endl;

      for (int j = 0; j < D; j++)
      {
        cout << j << "番目の変数は" << P[t][G][j] << "でした\n";
      }
      cout << endl;
      break;
    }
    /*論文にはwhile文の終わり方が掲載されていなかったため、自分で追加しました。最後の世代で一番成績の良いものを選びます。*/
  }
}

int main(void)
{
  shade();
  return 0;
}
