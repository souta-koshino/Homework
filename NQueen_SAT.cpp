#include <iostream>
#include <stack>
#include <vector>
using namespace std;

int main(void)
{
  int N;
  cout << "パズルのサイズを入力してください: ";
  cin >> N;

  int numVars = N * N;
  vector<int> vars;
  vector<vector<int>> cnf;

  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < N; j++)
    {
      vars.push_back(i * N + j + 1);
    }

    cnf.push_back(vars);

    for (int i = 0; i < vars.size() - 1; i++)
    {
      for (int j = i + 1; j < vars.size(); j++)
      {
        vector<int> p{-vars[i], -vars[j]};
        cnf.push_back(p);
      }
    }

    vars.clear();
  }

  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < N; j++)
    {
      vars.push_back(j * N + i + 1);
    }

    cnf.push_back(vars);

    for (int i = 0; i < vars.size() - 1; i++)
    {
      for (int j = i + 1; j < vars.size(); j++)
      {
        vector<int> p{-vars[i], -vars[j]};
        cnf.push_back(p);
      }
    }

    vars.clear();
  }

  for (int i = 0; i < N - 1; i++)
  {
    for (int j = 0; j < N - i; j++)
    {
      vars.push_back(j * N + i + j + 1);
    }

    for (int i = 0; i < vars.size() - 1; i++)
    {
      for (int j = i + 1; j < vars.size(); j++)
      {
        vector<int> p{-vars[i], -vars[j]};
        cnf.push_back(p);
      }
    }

    vars.clear();
  }

  for (int i = 1; i < N - 1; i++)
  {
    for (int j = 0; j < N - i; j++)
    {
      vars.push_back((j + i) * N + j + 1);
    }

    for (int i = 0; i < vars.size() - 1; i++)
    {
      for (int j = i + 1; j < vars.size(); j++)
      {
        vector<int> p{-vars[i], -vars[j]};
        cnf.push_back(p);
      }
    }

    vars.clear();
  }

  for (int i = 0; i < N - 1; i++)
  {
    for (int j = 0; j < N - i; j++)
    {
      vars.push_back(j * N + N - 1 - (i + j) + 1);
    }

    for (int i = 0; i < vars.size() - 1; i++)
    {
      for (int j = i + 1; j < vars.size(); j++)
      {
        vector<int> p{-vars[i], -vars[j]};
        cnf.push_back(p);
      }
    }

    vars.clear();
  }

  for (int i = 1; i < N - 1; i++)
  {
    for (int j = 0; j < N - i; j++)
    {
      vars.push_back((j + i) * N + N - j);
    }

    for (int i = 0; i < vars.size() - 1; i++)
    {
      for (int j = i + 1; j < vars.size(); j++)
      {
        vector<int> p{-vars[i], -vars[j]};
        cnf.push_back(p);
      }
    }

    vars.clear();
  }

  cout << "p cnf " << numVars << " " << cnf.size() << endl;

  for (int i = 0; i < cnf.size(); i++)
  {
    for (int j = 0; j < cnf[i].size(); j++)
    {
      cout << cnf[i][j] << " ";
    }
    cout << " 0" << endl;
  }

  return 0;
}
