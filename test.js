"use strict";
// ==UserScript==
// @name         禁止抄题解
// @namespace    http://tampermonkey.net/
// @version      2025-03-19
// @description  try to take over the world!
// @author       You
// @match        https://www.luogu.com.cn/problem/*
// @match        https://www.luogu.com.cn/training/*
// @icon         https://www.google.com/s2/favicons?sz=64&domain=luogu.com.cn
// @grant        none
// ==/UserScript==
(function () {
    "use strict";
    let href = window.location.href;
    if (href.indexOf("problem/solution/") !== -1) {
        window.location.href = "about:blank";
    }
    let cnt = 0;
    setInterval(() => {
        if (cnt > 20) { return; }
        const selector = "#app > div.main-container.lside-bar > main > div > div > div.side > div:nth-child(1) > a:nth-child(6)";
        let element = document.querySelector(selector);
        if (element !== null) {
            element.parentElement?.removeChild(element);
        }
        const s2 = ".expand-indicator";
        let e2 = document.querySelector(s2);
        if (e2 !== null) {
            e2.parentElement?.removeChild(e2);
        }

        // 删除题目难度标签
        const s3 = "a[href*=\"list?difficulty=\"]";
        let e3 = document.querySelector(s3);
        if (e3 !== null) {
            e3.innerHTML = `<span> ? </span>`;
            if (e3 instanceof HTMLAnchorElement) e3.href = "about:blank";
        }

        const s4 = ".difficulty a";
        let e4s = document.querySelectorAll(s4);
        for (let e4 of e4s) {
            e4.innerHTML = `<span style = "color: black !important;"> ? </span>`;
            if (e4 instanceof HTMLAnchorElement) e4.href = "about:blank";
        }
        cnt++;
    }, 50);
    // Your code here...
})();
