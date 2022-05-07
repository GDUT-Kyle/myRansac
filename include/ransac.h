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
    Ransac(/* args */);
    ~Ransac();
    void estimate_ransac(const vector<float>& data_x, const vector<float>& data_y, float& k, float& b);
};