// ==UserScript==
// @name         通过率计算
// @namespace    http://tampermonkey.net/
// @version      2024-11-03
// @description  try to take over the world!
// @author       You
// @match        *://www.luogu.com.cn/user/*
// @match        *://www.luogu.com/user/*
// @icon         https://www.google.com/s2/favicons?sz=64&domain=luogu.com.cn
// @grant        none
// ==/UserScript==

(function() {
    'use strict';
    
    function parse(str) {
        // 可能是 ...k/K ...m/M 的形式，前面可能是浮点数或整数
        if (str.endsWith('k') || str.endsWith('K')) {
            return parseFloat(str.slice(0, -1)) * 1000;
        }
        if (str.endsWith('m') || str.endsWith('M')) {
            return parseFloat(str.slice(0, -1)) * 1000000;
        }
        return parseInt(str);
    }

    function rate(submits, accepts) {
        return (parse(accepts) / parse(submits) * 100).toFixed(3) + '%';
    }

    let cnt = 0;
    let a = 0;
    function modifyPage() {
        if (cnt > 3)  {
            clearInterval(a);
            return;
        }
        const submitsSel = "#app .user-stat-data.lfe-caption > div > div:nth-child(3) > a > span.value"
        const acceptsSel = "#app .user-stat-data.lfe-caption > div > div:nth-child(4) > a > span.value"

        const submits = (document.querySelector(submitsSel).textContent);
        const accepts = (document.querySelector(acceptsSel).textContent);

        document.querySelector(acceptsSel).setAttribute("title",  "通过率" + rate(submits, accepts));
        // console.log(document.querySelector(acceptsSel).getAttribute("title"));
        cnt++;
    }

    window.onload = () => {
        console.log("通过率计算器插件 正在运行")
        a = setInterval(modifyPage, 500);
    }

    // Your code here...
})();