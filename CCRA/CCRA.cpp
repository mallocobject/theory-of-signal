#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <numeric>

const double DELTA = 1e-10; // 信道容量相对误差门限

/**
 *  @brief  迭代计算最佳信源分布、信道容量
 *  @param  p_x  信源分布
 *  @param  p_yx  信道转移概率矩阵
 *  @return  信道容量
 */
double func(std::vector<double> &, const std::vector<std::vector<double>> &);

int main()
{
    // <--------------------------------1 ： initial-------------------------------->
    int r; // 信源符号个数
    int s; // 信宿符号个数
    std::cout << "依次输入信源、新宿符号个数（可用空格、回车隔开）" << std::endl;
    std::cin >> r >> s;
    // test 1
    // r = 4;
    // s = 4
    // test 2
    // r = 2;
    // s = 3;
    std::vector<double> p_x(r, 1.0 / r);                              // 信源分布
    std::vector<std::vector<double>> p_yx(r, std::vector<double>(s)); // 信道转移概率矩阵
    std::cout << "依次信道转移概率矩阵元素（从左到右、自上而下，可用空格、回车隔开）" << std::endl;
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < s; j++)
        {
            std::cin >> p_yx[i][j];
        }
    }
    // test 1
    // p_yx = {{0.5, 0.25, 0, 0.25},
    //         {0, 1, 0, 0},
    //         {0, 0, 1, 0},
    //         {0.25, 0, 0.25, 0.5}};
    // test 2
    // p_yx = {{0.5, 0.3, 0.2},
    //         {0.3, 0.5, 0.2}};
    double new_c = 0.0;                               // 用于接收迭代结果
    double c = std::numeric_limits<double>::lowest(); // 初始化为“负无穷”，用于保存上一次迭代结果
    // <--------------------------------6 : recure-------------------------------->
    do
    {
        // <--------------------------------2 ~ 4 : calculate-------------------------------->
        new_c = func(p_x, p_yx);         // 接收迭代结果
        std::swap(c, new_c);             // 保存迭代结果，以备下次比较
        new_c = std::abs(c - new_c) / c; // 计算相对变化值
    } while (new_c > DELTA);
    // <--------------------------------5 : compare-------------------------------->

    // <--------------------------------7 : output-------------------------------->
    std::cout << "最佳信源分布" << std::endl;
    for (int i = 0; i < r; i++)
    {
        std::cout << p_x[i] << std::endl;
    }
    std::cout << "信道容量" << std::endl;
    std::cout << c << " nat per charactor" << std::endl;
    c *= std::log2(std::exp(1)); // net / charactor -> bit / charactor
    std::cout << c << " bit per charactor" << std::endl;
    // <--------------------------------8 : end-------------------------------->
    system("pause");
    return 0;
}

double func(std::vector<double> &p_x, const std::vector<std::vector<double>> &p_yx)
{
    int r = p_yx.size();    // 获取转移矩阵行数
    int s = p_yx[0].size(); // 获取列数
    // <--------------------------------2-------------------------------->
    std::vector<double> mult(s);
    for (int j = 0; j < s; j++)
    {
        double mult_sum = 0.0;
        for (int i = 0; i < r; i++)
        {
            mult_sum += p_yx[i][j] * p_x[i];
        }
        mult[j] = mult_sum;
    }
    std::vector<std::vector<double>> p_xy(p_yx); // 条件概率 p(x|y)
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < s; j++)
        {
            p_xy[i][j] = p_yx[i][j] * p_x[i] / mult[j];
        }
    }
    // <--------------------------------3-------------------------------->
    std::vector<double> exp_sum(r);
    for (int i = 0; i < r; i++)
    {
        double sum = 0.0;
        for (int j = 0; j < s; j++)
        {
            // if x -> 0 then xlnx -> 0
            // but here what computer provide is NaN
            // so we need take some measures such as followd
            if (p_yx[i][j] != 0)
            {
                sum += p_yx[i][j] * std::log(p_xy[i][j]);
            }
        }
        exp_sum[i] = std::exp(sum);
    }
    double sum_mult_sum = std::accumulate(exp_sum.begin(), exp_sum.end(), 0.0);
    for (int i = 0; i < r; i++)
    {
        p_x[i] = exp_sum[i] / sum_mult_sum;
    }
    // <--------------------------------4-------------------------------->
    double new_c = std::log(sum_mult_sum);
    return new_c;
}