const readline = require('readline-sync');
const math = require('mathjs');

const DELTA = 1e-10;

function main() {
    let r, s;

    while (isNaN(r) || r === null) {
        r = parseInt(readline.question('请输入信源个数：'));
        if (isNaN(r)) {
            console.log('输入无效，请输入一个整数。');
        }
    }

    while (isNaN(s) || s === null) {
        s = parseInt(readline.question('请输入新宿符号个数：'));
        if (isNaN(s)) {
            console.log('输入无效，请输入一个整数。');
        }
    }
    // const r = 2;
    // const s = 3;

    let p_x = Array(r).fill(1.0 / r);
    // let p_yx = Array.from({ length: r }, () => Array(s).fill(0));
    let p_yx = [[0.5, 0.3, 0.2], [0.3, 0.5, 0.2]];

    // console.log('依次信道转移概率矩阵元素（从左到右、自上而下，回车隔开）');
    // for (let i = 0; i < r; i++) {
    //     for (let j = 0; j < s; j++) {
    //         p_yx[i][j] = parseFloat(readline.question());
    //     }
    // }

    let c = -Infinity;
    let new_c = func(p_x, p_yx);
    [c, new_c] = [new_c, c];
    new_c = Math.abs(c - new_c) / c;
    while (new_c > DELTA) {
        new_c = func(p_x, p_yx);
        [c, new_c] = [new_c, c];
        new_c = Math.abs(c - new_c) / c;
    }

    console.log('最佳信源分布');
    for (let e of p_x) {
        console.log(e);
    }
    console.log('信道容量');
    console.log(c + ' (net per charactor)');
    c *= Math.log2(Math.exp(1));
    console.log(c + ' (bit per charactor)');
}

function func(p_x, p_yx) {
    const r = p_yx.length;
    const s = p_yx[0].length;
    let mult = Array(s).fill(0);

    for (let j = 0; j < s; j++) {
        let mult_sum = 0.0;
        for (let i = 0; i < r; i++) {
            mult_sum += p_yx[i][j] * p_x[i];
        }
        mult[j] = mult_sum;
    }

    let p_xy = Array.from({ length: r }, () => Array(s).fill(0));
    for (let i = 0; i < r; i++) {
        for (let j = 0; j < s; j++) {
            p_xy[i][j] = p_yx[i][j] * p_x[i] / mult[j];
        }
    }

    let exp_sum = Array(r).fill(0);
    for (let i = 0; i < r; i++) {
        let sum = 0.0;
        for (let j = 0; j < s; j++) {
            if (p_yx[i][j] != 0) {
                sum += p_yx[i][j] * Math.log(p_xy[i][j]);
            }
        }
        exp_sum[i] = Math.exp(sum);
    }
    let sum_mult_sum = math.sum(exp_sum);

    for (let i = 0; i < r; i++) {
        p_x[i] = exp_sum[i] / sum_mult_sum;
    }

    let new_c = Math.log(sum_mult_sum);
    return new_c;
}

main();