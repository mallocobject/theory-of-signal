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