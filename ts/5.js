"use strict";
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
function quickPow(a, b, p) {
    let res = ONE;
    for (; b != ZERO; b /= TWO, a = a * a % p)
        if (b % TWO == ONE)
            res = res * a % p;
    return res;
}
class PrimeChecking {
    static check(a, n, x, t) {
        let res = quickPow(a, x, n);
        let lst = res;
        for (let i = 0; i < t; i++) {
            res = res * res % n;
            if (res == ONE && lst != ONE && lst != n - ONE)
                return true;
            lst = res;
        }
        return res != ONE;
    }
    static isPrime(n) {
        if (n < TWO)
            return false;
        if (n == TWO)
            return true;
        if (n % TWO == ZERO)
            return false;
        let x = BigInt(n - ONE);
        let t = 0;
        while (x % TWO == ZERO) {
            x /= TWO;
            t++;
        }
        for (let i = 0; i < S; i++) {
            let a = BigInt(Math.floor(Math.random() * 1e6) + 1) % (n - TWO) + TWO;
            if (PrimeChecking.check(a, n, x, t))
                return false;
        }
        return true;
    }
}
function dfs(element) {
    if (element.children.length == 0) {
        if (element.parentElement?.getAttribute("marked-prime") === "1") {
            return;
        }
        if (!(element instanceof HTMLElement))
            return;
        element.innerText = element.innerText.replaceAll(/[0-9]*([0-9]+.{0,1})*[0-9]/g, (s) => {
            const num = BigInt(Array
                .from(s)
                .filter(e => (48 <= e.charCodeAt(0) && e.charCodeAt(0) < 58))
                .reduce((a, b) => a + b));
            console.log(num);
            if (PrimeChecking.isPrime(num)) {
                element.setAttribute("marked-prime", "1");
                return `<prime>${s}</prime>`;
            }
            else {
                return s;
            }
        });
        element.innerHTML = element.innerHTML.replaceAll(/&lt;prime&gt;(.*?)&lt;\/prime&gt;/g, `<span style="color:red">$1</span>`);
    }
    else {
        for (let i = 0; i < element.children.length; i++) {
            dfs(element.children[i]);
        }
    }
}
(function () {
    'use strict';
    // window.onload = () => 
    //     document
    //         .body
    //         .innerText
    //         .match(/([0-9]+.{0,1})+/g)
    //        ?.forEach( (s) => {
    //             const num = Number(
    //                 Array
    //                     .from(s)
    //                     .filter(  e => ( 48<=e.charCodeAt(0) && e.charCodeAt(0)<58 )  )
    //                     .reduce( (a,b) => a + b )
    //             )
    //             console.log(num);
    //             if ( PrimeChecking.isPrime( BigInt(num) ) )
    //                 console.log(`${num} 是质数！原始数据：${s}`)
    //         } )
    setInterval(async () => dfs(document.body), 3000);
    // Your code here...
})();
