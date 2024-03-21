import * as readline from 'node:readline/promises';

const DELTA = 1e-10;

function main() {
    console.log('Enter source, transition, and destination probabilities separated by spaces');
    const r = parseInt(readline());
    const s = parseInt(readline());
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
            p_yx[i][j] = parseFloat(readline());
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

    console.log('Optimal source probabilities');
    for (let e of p_x) {
        console.log(e);
    }
    console.log('Capacity');
    console.log(c);
    console.log('(bits per character)');
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

if (require.main === module) {
    main();
}