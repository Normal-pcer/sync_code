"use strict";
// ==UserScript==
// @name         闪击题解
// @namespace    http://tampermonkey.net/
// @version      2024-09-24
// @description  Accepted!
// @author       You
// @match        https://www.luogu.com.cn/problem/list*
// @match        https://www.luogu.com.cn/problem/solution*
// @match        https://www.luogu.com.cn/problem/*
// @match        https://www.luogu.com.cn/training/*
// @icon         https://www.google.com/s2/favicons?sz=64&domain=luogu.com.cn
// @grant        none
// ==/UserScript==
var copiedFlag = false;
function logic(x, y) {
    return (x & y) ^ ((x ^ y) | (~x & y));
}
function problemsIndex() {
    if (!(window.location.href.includes("list") || window.location.href.includes("training")))
        return;
    document.querySelectorAll(".status").forEach((element) => {
        const link = element.querySelector("a");
        const accepted = element.querySelector(".fa-check");
        if (accepted === null && link) {
            const unaccepted = element.querySelector(".fa-xmark");
            if (unaccepted)
                link.style.color = "rgb(231, 76, 60)";
            console.log(link.href);
            const pid = link.href.replace(/.*pid=(.*)&.*/g, "$1");
            console.log(pid ? pid : "null");
            link.innerHTML = "闪";
            link.style.fontWeight = "bold";
            link.href = "https://www.luogu.com.cn/problem/solution/" + pid;
        }
    });
}
function jumpTo(element) {
    console.log(element);
    let root = document.body;
    let height = 0;
    while (element !== root && element.parentElement) {
        height += element.offsetTop;
        element = element.parentElement;
    }
    window.scrollTo({
        top: height,
        behavior: "smooth"
    });
}
function readingSolution() {
    if (!window.location.href.includes("solution"))
        return;
    const pid = window.location.href.replace(/.*solution\/(.*)/g, "$1");
    const codes = document.querySelectorAll("code");
    codes.forEach((element) => {
        element.addEventListener("click", (event) => {
            if (copiedFlag)
                return;
            console.log(element);
            navigator.clipboard.writeText(element.innerText);
            console.log(element.innerText);
            alert("复制成功！跳转至提交界面");
            setTimeout(() => window.location.href = `https://www.luogu.com.cn/problem/${pid}#submit`, 100);
            event.preventDefault();
            copiedFlag = true;
        });
    });
    codes[0].scrollIntoView();
}
(function () {
    'use strict';
    console.log("闪击题解");
    setInterval(() => {
        problemsIndex();
        readingSolution();
    }, 3000);
    // Your code here...
})();
