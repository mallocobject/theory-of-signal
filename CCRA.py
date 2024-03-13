import numpy as np

DELTA = 1e-10

def main():
    print('依次输入信源、新宿符号个数（回车隔开）')
    r = int(input())
    s = int(input())
    p_x = np.full(r, 1.0 / r, dtype=float)
    p_yx = np.zeros((r, s), dtype=float)
    print('依次信道转移概率矩阵元素（从左到右、自上而下，回车隔开）')
    for i in range(len(p_yx)):
        for j in range(len(p_yx[0])):
            p_yx[i][j] = float(input())

    c = -np.inf
    new_c = func(p_x, p_yx)
    c, new_c = new_c, c
    new_c = np.abs(c - new_c) / c
    while (new_c > DELTA):
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
    r = len(p_yx)
    s = len(p_yx[0])
    mult = np.zeros(s, dtype=float)
    for j in range(s):
        mult_sum = 0.0
        for i in range(r):
            mult_sum += p_yx[i][j] * p_x[i]
        mult[j] = mult_sum
    
    p_xy = np.zeros((r, s), dtype=float)
    for i in range(r):
        for j in range(s):
            p_xy[i][j] = p_yx[i][j] * p_x[i] / mult[j]
    
    exp_sum = np.zeros(r, dtype=float)
    for i in range(r):
        sum = 0.0
        for j in range(s):
            if (p_yx[i][j] != 0):
                sum += p_yx[i][j] * np.log(p_xy[i][j])
        exp_sum[i] = np.exp(sum)
    sum_mult_sum = np.sum(exp_sum)

    for i in range(r):
        p_x[i] = exp_sum[i] / sum_mult_sum
    
    new_c = np.log(sum_mult_sum)
    return new_c

if __name__ == '__main__':
    main()