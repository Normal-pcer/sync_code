"use strict";
// ==UserScript==
// @name         Luogu Mark Problem
// @namespace    http://tampermonkey.net/
// @version      2024-12-17
// @description  try to take over the world!
// @author       You
// @match        https://www.luogu.com.cn/training*
// @match        https://www.luogu.com.cn/problem*
// @icon         https://www.google.com/s2/favicons?sz=64&domain=luogu.com.cn
// @grant        none
// ==/UserScript==
(function () {
    'use strict';
    const Flags = ((s) => {
        if (!s)
            return null;
        try {
            return JSON.parse(s);
        }
        catch (_) {
            return null;
        }
    })(window.localStorage.getItem("LuoguMarkProblem")) ?? {};
    console.log(Flags);
    const enableFor = (problemId) => {
        if (problemId.startsWith("CF"))
            return true;
        return false;
    };
    // 对勾图标的 HTML
    const checkColor = "rgb(82, 196, 26)";
    const checkHtml = `<path data-v-1b44b3e6="" fill="currentColor" d="M438.6 105.4c12.5 12.5 12.5 32.8 0 45.3l-256 256c-12.5 12.5-32.8 12.5-45.3 0l-128-128c-12.5-12.5-12.5-32.8 0-45.3s32.8-12.5 45.3 0L160 338.7 393.4 105.4c12.5-12.5 32.8-12.5 45.3 0z" class=""></path>`;
    // 叉号图标的 HTML
    const crossColor = "rgb(231, 76, 60)";
    const crossHtml = `<path data-v-1b44b3e6="" fill="currentColor" d="M342.6 150.6c12.5-12.5 12.5-32.8 0-45.3s-32.8-12.5-45.3 0L192 210.7 86.6 105.4c-12.5-12.5-32.8-12.5-45.3 0s-12.5 32.8 0 45.3L146.7 256 41.4 361.4c-12.5 12.5-12.5 32.8 0 45.3s32.8 12.5 45.3 0L192 301.3 297.4 406.6c12.5 12.5 32.8 12.5 45.3 0s12.5-32.8 0-45.3L237.3 256 342.6 150.6z" class=""></path>`;
    const problemListPage = () => {
        const ListClassName = "row-wrap";
        const lists = document.getElementsByClassName(ListClassName);
        if (lists.length !== 1)
            return;
        const children = lists[0].children;
        for (let i = 0; i < children.length; i++) {
            const problem = children[i];
            const span_list = problem.getElementsByTagName("span");
            const status_icon = span_list[0];
            const problem_id = span_list[1].innerText;
            console.log("Found problem", problem_id);
            if (enableFor(problem_id)) {
                const display = () => {
                    const flag = Flags[problem_id] ?? null;
                    console.log(problem_id, flag);
                    const svg = status_icon.querySelector("svg");
                    if (!svg)
                        return;
                    if (flag === null)
                        return;
                    if (flag === true) {
                        svg.style.color = checkColor;
                        svg.innerHTML = checkHtml;
                    }
                    else {
                        svg.style.color = crossColor;
                        svg.innerHTML = crossHtml;
                    }
                };
                display();
                status_icon.addEventListener("click", (event) => {
                    const flag = Flags[problem_id] ?? null;
                    const new_flag = !flag;
                    Flags[problem_id] = new_flag;
                    console.log(Flags);
                    console.log(JSON.stringify(Flags));
                    localStorage.setItem("LuoguMarkProblem", JSON.stringify(Flags));
                    display();
                    event.preventDefault();
                });
                status_icon.addEventListener("contextmenu", (event) => {
                    const flag = Flags[problem_id] ?? null;
                    const new_flag = (() => {
                        if (flag === null)
                            return true;
                        else
                            return null;
                    })();
                    Flags[problem_id] = new_flag;
                    localStorage.setItem("LuoguMarkProblem", JSON.stringify(Flags));
                    display();
                    event.preventDefault();
                });
            }
        }
    };
    window.onload = () => {
        setTimeout(problemListPage, 500);
    };
})();
