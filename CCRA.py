import numpy as np

DELTA = 1e-10

def main():
    r = int(input())
    s = int(input())
    p_x = np.full(r, 1.0 / r, dtype=float)
    p_yx = np.zeros((r, s), dtype=float)
    for i in range(len(p_yx)):
        for j in range(len(p_yx[0])):
            p_yx[i][j] = int(input())

    c = -np.inf
    new_c = func()
    c, new_c = new_c, c
    new_c = np.abs(c - new_c) / c
    while (new_c > DELTA):
        new_c = func()
        c, new_c = new_c, c
        new_c = np.abs(c - new_c) / c
    for e in p_x:
        print(e)
    print(c)

def func():
    pass
    new_c = 0.0
    return new_c

if __name__ == '__main__':
    main()