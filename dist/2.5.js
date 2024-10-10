"use strict";
// ==UserScript==
// @name         New Userscript
// @namespace    http://tampermonkey.net/
// @version      2024-09-12
// @description  try to take over the world!
// @author       You
// @match        https://www.luogu.com.cn/*
// @match        https://www.luogu.com/*
// @icon         https://www.google.com/s2/favicons?sz=64&domain=luogu.com.cn
// @grant        none
// ==/UserScript==
(function () {
    "use strict";
    let href = window.location.href;
    if (href.indexOf("problem/solution/") !== -1) {
        window.location.href = "https://ys.mihoyo.com/";
    }
    window.onload = () => {
        const selector = "#app > div.main-container > main > div > section.side > div:nth-child(1) > a:nth-child(3)";
        let element = document.querySelector(selector);
        if (element !== null) {
            element.parentElement?.removeChild(element);
        }
    };
    // Your code here...
})();
