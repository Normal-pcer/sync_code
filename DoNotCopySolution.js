"use strict";
// ==UserScript==
// @name         禁止抄题解
// @namespace    http://tampermonkey.net/
// @version      2025-03-25
// @description  try to take over the world!
// @author       You
// @match        https://www.luogu.com.cn/problem/*
// @match        https://www.luogu.com.cn/*
// @match        https://www.luogu.com.cn/training/*
// @icon         https://www.google.com/s2/favicons?sz=64&domain=luogu.com.cn
// @grant        none
// ==/UserScript==
/**
 * 创建一个随机的密码。
 * 由 DeepSeek R1 生成
 * @returns string
 */
function generatePassword(length = 12) {
    const uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const lowercase = "abcdefghijklmnopqrstuvwxyz";
    const numbers = "0123456789";
    const allChars = uppercase + lowercase + numbers;

    // 确保密码长度至少为 3
    if (length < 3) length = 3;

    let password = [];
    // 强制包含至少一个大写、小写和数字
    password.push(uppercase[Math.floor(Math.random() * uppercase.length)]);
    password.push(lowercase[Math.floor(Math.random() * lowercase.length)]);
    password.push(numbers[Math.floor(Math.random() * numbers.length)]);

    // 填充剩余字符
    for (let i = 3; i < length; i++) {
        password.push(allChars[Math.floor(Math.random() * allChars.length)]);
    }

    // Fisher-Yates 洗牌算法打乱顺序
    for (let i = password.length - 1; i > 0; i--) {
        const j = Math.floor(Math.random() * (i + 1));
        [password[i], password[j]] = [password[j], password[i]];
    }

    return password.join("");
}

(function () {
    "use strict";
    let href = window.location.href;
    if (href.indexOf("problem/solution/") !== -1) {
        const match = href.match(/[?&]password=([\w\d]*)/);
        if (match !== null && match[1] !== undefined) {
            let correct = sessionStorage.getItem("solution-password");
            correct === match[1];
            return;
        }
        window.location.href = "about:blank";
    }
    let cnt = 0;
    const do_replace = () => {
        console.log("do replace");
        // 尝试读取当前分数
        if (href.indexOf("problem/") !== -1 && href.indexOf("list") === -1) {
            const scoreSelector = `a[href *= "record"] span`;
            let possible = document.querySelectorAll(scoreSelector);
            let score = (() => {
                for (const e of possible) {
                    if (e instanceof HTMLSpanElement) {
                        let num = parseInt(e.innerText);
                        if (!isNaN(num)) {
                            return num;
                        }
                    }
                }
                return null;
            })();

            if (score !== null && score >= 60) {
                const lis = document.getElementsByClassName("hide-solution");
                for (const e of lis) {
                    if (e instanceof HTMLElement) {
                        e.style.display = e.getAttribute("origin-display") ?? "block";
                    }
                }
                // 允许查看题解
                // 创建一个新的查看题解密码
                const nextPassword = generatePassword();
                sessionStorage.setItem("solution-password", nextPassword);

                const selector =
                    "#app > div.main-container.lside-bar > main > div > div > div.side > div:nth-child(1) > a:nth-child(6)";
                let element = document.querySelector(selector);
                if (element !== null && element instanceof HTMLAnchorElement && !element.href.includes("password=")) {
                    if (element.href.includes("?")) {
                        element.href += `&password=${nextPassword}`;
                    } else {
                        element.href += `?password=${nextPassword}`;
                    }
                }
                return;
            }
        }

        const selector =
            "#app > div.main-container.lside-bar > main > div > div > div.side > div:nth-child(1) > a:nth-child(6)";
        let element = document.querySelector(selector);
        if (element !== null && element instanceof HTMLElement) {
            // element.parentElement?.removeChild(element);
            if (element.style.display !== "none") {
                element.setAttribute("origin-display", element.style.display);
                element.style.display = "none";
                element.classList.add("hide-solution");
            }
        }
        const s2 = ".expand-indicator";
        let e2Lis = document.querySelectorAll(s2);
        for (const e2 of e2Lis) {
            // e2.parentElement?.removeChild(e2);
            if (e2 instanceof HTMLElement && e2.style.display !== "none") {
                e2.setAttribute("origin-display", e2.style.display);
                e2.style.display = "none";
                e2.classList.add("hide-solution");
            }
        }

        // 删除题目难度标签
        const s3 = 'a[href*="list?difficulty="]';
        let e3 = document.querySelector(s3);
        if (e3 !== null) {
            e3.innerHTML = `<span> ? </span>`;
            if (e3 instanceof HTMLAnchorElement) e3.href = "about:blank";
        }

        const rowsSelector = ".row";
        const rows = document.querySelectorAll(rowsSelector);
        for (const row of rows) {
            const statSelector = `span.status`;
            let stat = row.querySelector(statSelector);
            if (stat !== null) {
                if (stat.innerHTML.includes("check")) {
                    // console.log("skip", row);
                    continue;
                }
            }
            const s4 = ".difficulty a";
            let e4s = row.querySelectorAll(s4);
            for (let e4 of e4s) {
                // console.log("row: ", row, "e4: ", e4);
                e4.innerHTML = `<span style = "color: black !important;"> ? </span>`;
                if (e4 instanceof HTMLAnchorElement) e4.href = "about:blank";
            }
        }
    };

    let doing_replace = false;
    const observer = new MutationObserver((_) => {
        // console.log("observe");
        if (doing_replace) {
            return;
        }
        doing_replace = true;
        do_replace();
        setTimeout(() => {
            doing_replace = false;
        }, 50);
    });

    setTimeout(() => {
        // 监听元素及其后代的所有类型变化
        observer.observe(document.body, {
            childList: true,
            attributes: true,
            characterData: true,
            subtree: true,
            attributeOldValue: true, // 记录旧属性值
            characterDataOldValue: true, // 记录旧文本值
        });
    }, 800);
    setInterval(() => {
        if (cnt >= 20) {
            return;
        }
        do_replace();
        cnt++;
    }, 50);
})();
