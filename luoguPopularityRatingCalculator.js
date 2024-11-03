// ==UserScript==
// @name         网红指数计算
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

    
    // Enum method
    class ModeEnum {
        static Normal = 0;
        static Sqrt = 1;
        static Log2 = 2;
        static Ln = 3;
        static Log10 = 4;
        static size = 5;
    }

    class Mode2Enum {
        static Normal = 0;
        static Fold = 1;  // 折叠成 ...K / ...M / ...G / ...T 的形式
        static size = 2;
    }

    const levelChar = ' KMGTP'
    /**
     * 
     * @param {number} num 
     * @returns {string}
     */
    function fold(num, dep=0) {  // 保证前面的数整数部分 2~4 位，小数点后 2 位或没有
        // console.log("fold", num, dep);
        if (num >= 10000 && dep < levelChar.length - 1) {
            return fold(num / 1000, dep + 1);
        }
        if (dep == 0) {
            return num.toString();
        } else {
            return (num).toFixed(2) + levelChar[dep];
        }
    }
    
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

    function popularity(followers, submits, accepts, mode) {
        let res = parse(followers) * parse(submits) * parse(accepts);
        switch (mode) {
            case ModeEnum.Sqrt:     res = Math.round(Math.sqrt(res)); break;
            case ModeEnum.Log2:     res = Math.log2(res).toFixed(6); break;
            case ModeEnum.Ln:       res = Math.log(res).toFixed(6); break;
            case ModeEnum.Log10:    res = Math.log10(res).toFixed(6); break;
        }
        return res;
    }

    function getMode() {
        // console.log("getMode", window.localStorage.getItem('modePopularity'))
        return Math.round(window.localStorage.getItem('modePopularity') ?? ModeEnum.Normal);
    }

    function setMode(mode) {
        // console.log("setMode", mode)
        window.localStorage.setItem('modePopularity', mode);
    }

    function getMode2() {
        // console.log("getMode2", window.localStorage.getItem('mode2Popularity'))
        return Math.round(window.localStorage.getItem('mode2Popularity') ?? Mode2Enum.Fold);
    }

    function setMode2(mode) {
        // console.log("setMode2", mode)
        window.localStorage.setItem('mode2Popularity', mode);
    }

    function modifyPage() {
        const followersSel = "#app .user-stat-data.lfe-caption > div > div:nth-child(2) > a > span.value"
        const submitsSel = "#app .user-stat-data.lfe-caption > div > div:nth-child(3) > a > span.value"
        const acceptsSel = "#app .user-stat-data.lfe-caption > div > div:nth-child(4) > a > span.value"

        const followers = (document.querySelector(followersSel).textContent);
        const submits = (document.querySelector(submitsSel).textContent);
        const accepts = (document.querySelector(acceptsSel).textContent);

        // 清除以前的网红指数
        const oldPopularity = document.querySelector("#app .user-stat-data.lfe-caption > div > div.field[popularity]");
        if (oldPopularity)   oldPopularity.remove();

        const father = document.querySelector("#app .user-stat-data.lfe-caption > div");

        // 从父亲的最后一个 div 复制过来，保留属性和子节点
        const container = father.lastElementChild.cloneNode(true);
        container.setAttribute("popularity", getMode());
        const popularitySpan = container.querySelector(".value");
        popularitySpan.textContent = popularity(followers, submits, accepts, getMode());

        const popularityKey = container.querySelector(".key");
        popularityKey.textContent = "网红指数";
        
        switch (getMode()) {
            case ModeEnum.Sqrt:     popularityKey.textContent += "(sqrt)"; break;
            case ModeEnum.Log2:     popularityKey.textContent += "(log2)"; break;
            case ModeEnum.Ln:       popularityKey.textContent += "(ln)"; break;
            case ModeEnum.Log10:    popularityKey.textContent += "(lg)"; break;
        }

        if (getMode2() == Mode2Enum.Fold) {
            popularitySpan.textContent = fold(popularitySpan.textContent);
        }

        container.appendChild(popularityKey);
        container.appendChild(popularitySpan);
        father.appendChild(container);

        // 左键切换 mode，右键切换 mode2
        container.addEventListener("click", () => {
            // console.log("Switch mode", getMode())
            setMode((getMode() + 1) % ModeEnum.size);
            // console.log("to", (getMode() + 1) % ModeEnum.size)
            // console.log("With size", ModeEnum.size)
            modifyPage();
        })

        container.addEventListener("contextmenu", (event) => {
            event.preventDefault();
            // console.log("Switch mode2")
            setMode2((getMode2() + 1) % Mode2Enum.size);
            modifyPage();
        })
    }

    window.onload = () => {
        console.log("网红指数插件 正在运行")
        modifyPage();
    }

    // Your code here...
})();