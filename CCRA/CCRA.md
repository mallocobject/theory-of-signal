# Procedures

1. $$
   initial
   $$

2. $$
   \varphi_{ji}^{(k)} = \frac{p_{ij}p_i^{(k)}}{\sum_{i = 1}^r}	i = 1, ..., r; j = 1, ..., s
   $$

3. $$
   p_i^{(k+1)} = \frac{exp[\sum_{j = 1}^sp_{ij}ln\varphi_{ji}^{(k)}]}{\sum_{i = 1}^r\{exp[\sum_{j = 1}^sp_{ij}ln\varphi_{ji}^{(k)}]\}}	i = 1, ..., r
   $$

4. $$
   C^{k + 1} = ln{\sum_{i = 1}^r\{exp[\sum_{j = 1}^sp_{ij}ln\varphi_{ji}^{(k)}]\}}
   $$

5. $$
   if	\frac{|C^{k + 1} - C^k|}{C^{k + 1}} \leqslant \delta: \ goto \ step \ 7 \\
   else: continue
   $$

6. $$
   remap \ \ k = k + 1，goto \ step \ 2
   $$

7. $$
   output \ \  p_i^{(k + 1)} \ and \ \  C^{(k + 1)}
   $$

8. $$
   stop
   $$

   

# C++ source code

```c++
#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <numeric>

const double DELTA = 1e-10; // 信道容量相对误差门限

/**
 *  @brief  迭代计算最佳信源分布，旧的信道容量
 *  @param  p_x  信源分布
 *  @param  p_yx  信道转移概率矩阵
 *  @return  新的信道容量
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
```

# Python source code

```python
import numpy as np

DELTA = 1e-10

def main():
    """
    主函数，用于接收用户输入并计算信道容量
    """
    print('依次输入信源、新宿符号个数（回车隔开）')
    r = int(input())  # 信源符号个数
    s = int(input())  # 新宿符号个数
    p_x = np.full(r, 1.0 / r, dtype=float)  # 初始化信源分布
    p_yx = np.zeros((r, s), dtype=float)  # 初始化信道转移概率矩阵
    print('依次信道转移概率矩阵元素（从左到右、自上而下，回车隔开）')
    for i in range(len(p_yx)):
        for j in range(len(p_yx[0])):
            p_yx[i][j] = float(input())  # 输入信道转移概率矩阵元素

    c = -np.inf  # 初始化信道容量
    new_c = func(p_x, p_yx)  # 计算新的信道容量
    c, new_c = new_c, c
    new_c = np.abs(c - new_c) / c
    while (new_c > DELTA):  # 当新旧信道容量之差大于阈值时，继续迭代
        new_c = func(p_x, p_yx)
        c, new_c = new_c, c
        new_c = np.abs(c - new_c) / c
    
    print('最佳信源分布')
    for e in p_x:
        print(e)
    print('信道容量')
    print(c, end=' ')
    print('(net per charactor)')
    c *= np.log2(np.exp(1))
    print(c, end=' ')
    print('(bit per charactor)')

def func(p_x, p_yx):
    """
    计算新的信道容量和信源分布

    参数:
    p_x -- 信源分布
    p_yx -- 信道转移概率矩阵

    返回:
    new_c -- 新的信道容量
    """
    r = len(p_yx)  # 信源符号个数
    s = len(p_yx[0])  # 新宿符号个数
    mult = np.zeros(s, dtype=float)  # 初始化乘积数组
    for j in range(s):
        mult_sum = 0.0
        for i in range(r):
            mult_sum += p_yx[i][j] * p_x[i]  # 计算乘积
        mult[j] = mult_sum
    
    p_xy = np.zeros((r, s), dtype=float)  # 初始化联合概率矩阵
    for i in range(r):
        for j in range(s):
            p_xy[i][j] = p_yx[i][j] * p_x[i] / mult[j]  # 计算联合概率
    
    exp_sum = np.zeros(r, dtype=float)  # 初始化指数和数组
    for i in range(r):
        sum = 0.0
        for j in range(s):
            if (p_yx[i][j] != 0):
                sum += p_yx[i][j] * np.log(p_xy[i][j])  # 计算指数和
        exp_sum[i] = np.exp(sum)
    sum_mult_sum = np.sum(exp_sum)

    for i in range(r):
        p_x[i] = exp_sum[i] / sum_mult_sum  # 更新信源分布
    
    new_c = np.log(sum_mult_sum)  # 计算新的信道容量
    return new_c

if __name__ == '__main__':
    main()
```

# HTML source code

```html

<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>

<body>
    <script>
        const DELTA = 1e-10;

        function main() {
            // console.log('Enter source, transition, and destination probabilities separated by spaces');
            const r = parseInt(prompt('行数'));
            const s = parseInt(prompt('列数'));
            // const r = 2;
            // const s = 3;
            const p_x = new Array(r);
            for (let i = 0; i < p_x.length; i++) {
                p_x[i] = 1 / r;
            }

            const p_yx = new Array(r);
            for (let i = 0; i < r; i++) {
                p_yx[i] = new Array(s);
                for (let j = 0; j < s; j++) {
                    p_yx[i][j] = parseFloat(prompt());
                }
            }
            // const p_yx = [[0.5, 0.3, 0.2], [0.3, 0.5, 0.2]];

            let new_c = 0.0;
            let c = -Infinity;
            do {
                new_c = func(p_x, p_yx);
                [c, new_c] = [new_c, c];
                new_c = Math.abs(c - new_c) / c;
            } while (new_c > DELTA);

            // console.log('Optimal source probabilities');
            document.write('Optimal source probabilities');
            document.write('</br>');
            for (let e of p_x) {
                document.write(`<strong>${e}</strong>&nbsp;&nbsp;`);
                // document.write('</br>');
            }
            document.write('</br>');
            document.write('Capacity');
            document.write('</br>');
            document.write(`<strong>${c}</strong>`);
            document.write('&nbsp;(bits per character)');
        }

        function func(p_x, p_yx) {
            const r = p_yx.length;
            const s = p_yx[0].length;
            const mult = new Array(s);
            for (let j = 0; j < s; j++) {
                let mult_sum = 0;
                for (let i = 0; i < r; i++) {
                    mult_sum += p_yx[i][j] * p_x[i];
                }
                mult[j] = mult_sum;
            }

            const p_xy = new Array(r);
            for (let i = 0; i < r; i++) {
                p_xy[i] = new Array(s);
                for (let j = 0; j < s; j++) {
                    p_xy[i][j] = p_yx[i][j] * p_x[i] / mult[j];
                }
            }

            const exp_sum = new Array(s);
            for (let i = 0; i < r; i++) {
                let sum = 0;
                for (let j = 0; j < s; j++) {
                    if (p_yx[i][j] !== 0) {
                        sum += p_yx[i][j] * Math.log(p_xy[i][j]);
                    }
                }
                exp_sum[i] = Math.exp(sum);
            }
            const sum_mult_sum = exp_sum.reduce(function (total, value) {
                return total + value;
            }, 0);

            for (let i = 0; i < r; i++) {
                p_x[i] = exp_sum[i] / sum_mult_sum;
            }

            const new_c = Math.log(sum_mult_sum);
            return new_c;
        }

        main();

    </script>
</body>

</html>
```

