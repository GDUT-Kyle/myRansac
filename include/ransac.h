#include <iostream>
#include <vector>
#include "Eigen/Core"
#include "Eigen/Dense"

using namespace std;

class Ransac
{
private:
    int pickNums; // `n`: Minimum number of data points to estimate parameters
    int maxIter; // `k`: Maximum iterations allowed
    float threshold; // `t`: Threshold value to determine if points are fit well

    float best_fit;
    float best_err;
public:
    Ransac();
    ~Ransac(){};
    void estimate_ransac(const vector<float>& data_x, const vector<float>& data_y, float& k, float& b);
};

Ransac::Ransac()
{
    pickNums = 2;
    maxIter = 500;
    threshold = 0.05;

    best_fit = 0;
    best_err = __FLT_MAX__;
}