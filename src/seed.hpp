#ifndef SEED_HPP
#define SEED_HPP

#include <iostream>
#include <vector>
#include <utility>

#include <cmath>

using namespace std;

#ifdef M_PI
#undef M_PI
#endif
#define M_PI 3.1415926535f

constexpr int DATA = 997;
constexpr long long B = 314159239;
constexpr long long C = 453806245;
constexpr int init = 36;
constexpr double mean = 0.25;
constexpr double standard_deviation = 0.05;


void distribution_normal(double random, vector<int> &wastes, vector<double> &pseudo_random, vector<pair<double, double>> &coordinates)
{
    double x;
    double y;

    for (int i = 1; i < DATA - 1; ++i)
    {
        int waste = (B * wastes[i - 1] + C) % DATA;
        double random_p = (static_cast<double>(waste) / static_cast<double>(DATA));

        if (pseudo_random[i - 1] <= 0)
        {
            pseudo_random[i - 1] = 1;
        }

        x = sqrt(-2 * log(pseudo_random[i - 1])) * cos(2 * M_PI * random_p);
        y = mean + standard_deviation * x;

        coordinates.push_back(make_pair(x, y));

        wastes.push_back(waste);
        pseudo_random.push_back(random_p);
    }

    if (pseudo_random[wastes.size() - 1] <= 0)
    {
        pseudo_random[wastes.size() - 1] = 1;
    }

    x = sqrt(-2 * log(pseudo_random[wastes.size() - 1])) * cos(2 * M_PI * 0);
    y = mean + standard_deviation * x;

    coordinates.push_back(make_pair(x, y));
}

void distribution_uniform(const double a, const double b, vector<int> &wastes, vector<double> &pseudo_random, vector<double> &d_uniform)
{
    double y;

    for (int i = 1; i < DATA; ++i)
    {
        int waste = (B * wastes[i - 1] + C) % DATA;
        double random_p = (static_cast<double>(waste) / static_cast<double>(DATA));

        y = (a - b) * random_p + b;

        wastes.push_back(waste);
        pseudo_random.push_back(random_p);
        d_uniform.push_back(y);
    }
}

/*int main()
{
    const double a = 1.5;
    const double b = 0.5;
    double random = (static_cast<double>(init) / static_cast<double>(DATA));
    double y_prim = ((a - b) * random) + b;

    vector<int> wastes = {36};
    vector<double> pseudo_random = {random};
    vector<pair<double, double>> coordinates;

    vector<double> d_uniform = {y_prim};

    // distribution_normal(random, wastes, pseudo_random, coordinates);

    // distribution_uniform(a, b, wastes, pseudo_random, d_uniform);

    for (int i = 0; i < wastes.size(); ++i)
    {
        cout << wastes[i] << " " << pseudo_random[i] << " " << d_uniform[i] << endl;
    }

    return 0;
}*/

#endif