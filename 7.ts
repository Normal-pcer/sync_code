namespace _7 {
    // ==UserScript==
// @name         Find Prime
// @namespace    http://tampermonkey.net/
// @version      2024-09-25
// @description  try to take over the world!
// @author       You
// @match        http://*/*
// @match        https://*/*
// @icon         data:image/gif;base64,R0lGODlhAQABAAAAACH5BAEKAAEALAAAAAABAAEAAAICTAEAOw==
// @grant        none
// ==/UserScript==

const S = 16;
const ZERO = BigInt("0");
const ONE = BigInt("1");
const TWO = BigInt("2");

function quickPow(a: bigint, b: bigint, p: bigint) {
    let res = ONE;
    for (; b != ZERO; b /= TWO, a = (a * a) % p) if (b % TWO == ONE) res = (res * a) % p;
    return res;
}

function equals(a: any, b: any) {
    return a == b;
}

class PrimeChecking {
    static check(a: bigint, n: bigint, x: bigint, t: number) {
        let res = quickPow(a, x, n);
        let lst = res;
        for (let i = 0; i < t; i++) {
            res = (res * res) % n;
            if (res == ONE && lst != ONE && lst != n - ONE) return true;
            lst = res;
        }
        return res != ONE;
    }

    static isPrime(n: bigint) {
        if (n < TWO) return false;
        if (n == TWO) return true;
        if (n % TWO == ZERO) return false;
        let x = BigInt(n - ONE);
        let t = 0;
        while (x % TWO == ZERO) {
            x /= TWO;
            t++;
        }
        for (let i = 0; i < S; i++) {
            let a = (BigInt(Math.floor(Math.random() * 1e6) + 1) % (n - TWO)) + TWO;
            if (PrimeChecking.check(a, n, x, t)) return false;
        }
        return true;
    }
}

function main() {
    // 找几个略小于 1e18 的素数
    let i = BigInt("1000000000000000000");
    for( ; ; i--) {
        if (PrimeChecking.isPrime(i)) {
            console.log(i);
        }
    }
}

main();

}