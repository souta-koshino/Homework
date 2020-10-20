#include <iostream>
#include <cmath>
#include <random>
#include <cstdlib>
#include <algorithm>
using namespace std;

const double MIN = 1e-3;
const int DIMENSION = 3;
/*DIMENSIONをこれ以上大きくするとなぜかエラーとなるため、三次元空間について考えます。*/

class Individual
{
private:
  double *DNA;

public:
  Individual()
  {
    DNA = new double(DIMENSION);
    for (int i = 0; i < DIMENSION; i++)
    {
      DNA[i] = 0;
    }
  }

  Individual(double *x)
  {
    DNA = new double(DIMENSION);
    for (int i = 0; i < DIMENSION; i++)
    {
      DNA[i] = x[i];
    }
  }
  /*DNA[i]がx_i座標の値に相当します。三次元空間だと、DNA[0]がx_0座標、つまりx座標。DNA[1]がx_1座標、つまりy座標みたいな感じです。*/

  bool operator<(const Individual &other) const
  {
    return DNA[0] < other.DNA[0];
  }

  friend Individual Reproduce(const Individual &, const Individual &);

  void Mutate();

  void show();

  friend Individual GA(vector<pair<double, Individual>>);
};

double f7(double *x)
{
  double a = 0.0;

  for (int i = 0; i < DIMENSION; i++)
  {
    a += (x[i] * x[i]) / 4000;
  }

  double b = 1.0;

  for (int i = 0; i < DIMENSION; i++)
  {
    b *= cos(x[i] / sqrt(i + 1));
  }

  double tmp = a - b + 1.0;

  return tmp;
}
/*論文に掲載されていたGriewank's Functionです。*/

double gradf7(double *x)
{
  const double eps = 1e-6;

  double *g;
  g = new double(DIMENSION);

  double **e;
  e = new double *[DIMENSION];

  for (int i = 0; i < DIMENSION; i++)
  {
    e[i] = new double(DIMENSION);
  }

  for (int i = 0; i < DIMENSION; i++)
  {
    for (int j = 0; j < DIMENSION; j++)
    {
      if (i == j)
      {
        e[i][j] = x[j] + eps;
      }
      else
      {
        e[i][j] = x[j];
      }
    }
  }

  double **h;
  h = new double *[DIMENSION];

  for (int i = 0; i < DIMENSION; i++)
  {
    h[i] = new double(DIMENSION);
  }

  for (int i = 0; i < DIMENSION; i++)
  {
    for (int j = 0; j < DIMENSION; j++)
    {
      if (i == j)
      {
        h[i][j] = x[j] - eps;
      }
      else
      {
        h[i][j] = x[j];
      }
    }
  }

  for (int i = 0; i < DIMENSION; i++)
  {
    g[i] = (f7(e[i]) - f7(h[i])) / (2.0 * eps);
  }
  /*f7の勾配を計算します。この方法で誤差が生まれないか心配です。*/

  double t = 0;

  for (int i = 0; i < DIMENSION; i++)
  {
    t += g[i] * g[i];
  }

  t = sqrt(t);

  /*勾配の各座標の値についてユークリッドノルムを計算します。2Aで現在とってる数値解析の講義でこのような計算をしていた気がしたので、このような手法をとりました。*/

  return t;
}

Individual Reproduce(const Individual &X, const Individual &Y)
{
  int c = rand() % DIMENSION;

  double *dna;
  dna = new double(DIMENSION);

  for (int i = 0; i < c; i++)
  {
    dna[i] = X.DNA[i];
  }

  for (int j = c; j < DIMENSION; j++)
  {
    dna[j] = Y.DNA[j];
  }

  Individual child = Individual(dna);

  return child;
}
/*Russel&Norvingに掲載されている方法を用いました。*/

void Individual::Mutate()
{
  int c = rand() % DIMENSION;
  DNA[c] = (rand() % 200) - 100;
  return;
}
/*Mutateが起こるとDNAのどれか一つがランダムな値に設定されます。*/

Individual GA(vector<pair<double, Individual>> population)
{
  int count = 0;
  int weight = 30;
  /*一世代のpopulationを100に設定しているため、上位3割のみで交配を行ないます。*/

  while (1)
  {
    vector<pair<double, Individual>> population2;

    for (int i = 0; i < population.size(); i++)
    {
      Individual parent1 = population[rand() % weight].second;
      Individual parent2 = population[rand() % weight].second;

      Individual child = Reproduce(parent1, parent2);

      double mutate_rate = (double)rand() / RAND_MAX;

      if (mutate_rate < 0.1)
      {
        child.Mutate();
      }
      /*10%の確率で突然変異します。*/

      if (gradf7(child.DNA) < MIN)
      {
        return child;
      }
      /*停留点が得られたらそこで終了します。*/

      population2.push_back({gradf7(child.DNA), child});
    }

    count++;

    if (count > 1000)
    {
      long long MIN = 1001001001;
      Individual last;

      for (int i = 0; i < population2.size(); i++)
      {
        if (MIN > gradf7(population2[i].second.DNA))
        {
          MIN = gradf7(population2[i].second.DNA);
          last = population2[i].second;
        }
      }
      return last;
    }
    /*1000回を超えたら現在の世代で最強のものを代表とします。*/

    sort(population2.begin(), population2.end());

    population = population2;
  }
}

void Individual::show()
{
  cout << "パラメータは以下のようになった\n";

  for (int i = 0; i < DIMENSION; i++)
  {
    cout << i << "のパラメータは" << DNA[i] << "でした\n";
  }

  cout << "f7の値は" << f7(DNA) << "でした\n";
  cout << "f7の勾配の値は" << gradf7(DNA) << "でした\n";

  return;
}

int main(void)
{
  vector<pair<double, Individual>> population;

  for (int i = 0; i < 100; i++)
  {
    double *x;
    x = new double(DIMENSION);

    for (int i = 0; i < DIMENSION; i++)
    {
      x[i] = ((double)rand() / 200) - 100;
    }

    Individual p{x};
    population.push_back({gradf7(x), p});
  }

  Individual best = GA(population);
  best.show();

  return 0;
}
