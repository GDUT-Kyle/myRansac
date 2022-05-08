#include "ransac.h"

using namespace std;

void Ransac::estimate_ransac(const vector<float>& data_x, const vector<float>& data_y, float& k_, float& b_)
{
    if(data_x.size() != data_y.size())
        return;
    int randNums[pickNums];
    vector<bool> inliers(data_x.size(), false);
    for(int iter=0; iter<maxIter; iter++)
    {
        for(int i=0; i<pickNums; i++) // generate random numbers, and pick points
        {
            float frand = (float)rand()/(float)RAND_MAX;
            randNums[i] = (int)(100*frand);
        }
        // estimate a line
        Eigen::Matrix<float, 2, 1> X = Eigen::Matrix<float, 2, 1>::Zero(); // k, b
        const float& point1_x = data_x[randNums[0]];
        const float& point1_y = data_y[randNums[0]];
        const float& point2_x = data_x[randNums[1]];
        const float& point2_y = data_y[randNums[1]];
        X(0, 0) = (point1_y - point2_y)/(point1_x - point2_x);
        X(1, 0) = (point1_x*point2_y - point2_x*point1_y)/(point1_x - point2_x);

        if(X.hasNaN() || abs(X(0,0))>1e2 || abs(X(1,0))>1e2)
            continue;

        // Get number of inlier
        int inlier = 0;
        float inliers_dis = 0;
        vector<bool> inliers_tmp(data_x.size(), false);
        for(int i=0; i<data_x.size(); i++)
        {
            const float& x = data_x[i];
            const float& y = data_y[i];
            float& k = X(0, 0);
            float& b = X(1, 0);
            float dis = abs(k*x-y+b)/sqrt(1+k*k);
            if(dis < threshold)
            {
                inlier++;
                inliers_dis += dis;
                inliers_tmp[i] = true;
            }
            else
            {
                inliers_tmp[i] = false;
            }
        }

        // cout<<"X ="<<X.transpose()<<endl;
        // cout<<"inlier = "<<inlier<<endl;

        if(inlier > best_fit)
        {
            best_fit = inlier;
            inliers.swap(inliers_tmp);
        }

        if(inlier > 2*data_x.size()/3)
        {
            std::cout<<"iterate num = "<<iter<<endl;
            break;
        }
    }
    std::cout<<"points size = "<<data_x.size()<<", best_fit = "<<best_fit<<endl;

    // 将内点进行一次最小二乘法拟合
    Eigen::Matrix<float, 100, 2> A = Eigen::Matrix<float, 100, 2>::Zero();
    Eigen::Matrix<float, 2, 1> X = Eigen::Matrix<float, 2, 1>::Zero();
    Eigen::Matrix<float, 100, 1> B = Eigen::Matrix<float, 100, 1>::Zero();
    for(int i=0; i<data_x.size(); i++)
    {
        if(!inliers[i])
            continue;
        A(i, 0) = data_x[i];
        A(i, 1) = 1;
        B(i, 0) = data_y[i];
    }
    X = A.colPivHouseholderQr().solve(B);
    X = (A.transpose()*A).inverse()*A.transpose()*B;
    k_ = X(0, 0);
    b_ = X(1, 0);
}

int main(int, char**) {
    vector<float> data_x = {-0.848,-0.800,-0.704,-0.632,-0.488,-0.472,-0.368,-0.336,-0.280,-0.200,-0.00800,-0.0840,0.0240,0.100,0.124,0.148,0.232,0.236,0.324,0.356,0.368,0.440,0.512,0.548,0.660,0.640,0.712,0.752,0.776,0.880,0.920,0.944,-0.108,-0.168,-0.720,-0.784,-0.224,-0.604,-0.740,-0.0440,0.388,-0.0200,0.752,0.416,-0.0800,-0.348,0.988,0.776,0.680,0.880,-0.816,-0.424,-0.932,0.272,-0.556,-0.568,-0.600,-0.716,-0.796,-0.880,-0.972,-0.916,0.816,0.892,0.956,0.980,0.988,0.992,0.00400};
    vector<float> data_y = {-0.917,-0.833,-0.801,-0.665,-0.605,-0.545,-0.509,-0.433,-0.397,-0.281,-0.205,-0.169,-0.0531,-0.0651,0.0349,0.0829,0.0589,0.175,0.179,0.191,0.259,0.287,0.359,0.395,0.483,0.539,0.543,0.603,0.667,0.679,0.751,0.803,-0.265,-0.341,0.111,-0.113,0.547,0.791,0.551,0.347,0.975,0.943,-0.249,-0.769,-0.625,-0.861,-0.749,-0.945,-0.493,0.163,-0.469,0.0669,0.891,0.623,-0.609,-0.677,-0.721,-0.745,-0.885,-0.897,-0.969,-0.949,0.707,0.783,0.859,0.979,0.811,0.891,-0.137};

    Ransac estimator;
    float k_, b_;
    
    estimator.estimate_ransac(data_x, data_y, k_, b_);

    // Estimated linear equation
    cout<<"y = ("<<k_<<") * x + ("<<b_<<")"<<endl;

    return 0;
}
