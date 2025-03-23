"use strict";
// ==UserScript==
// @name         Luogu Mark Problem
// @namespace    http://tampermonkey.net/
// @version      2024-12-17
// @description  try to take over the world!
// @author       You
// @match        https://www.luogu.com.cn/training*
// @match        https://www.luogu.com.cn/problems*
// @icon         https://www.google.com/s2/favicons?sz=64&domain=luogu.com.cn
// @grant        none
// ==/UserScript==
(function () {
    'use strict';
    var _a;
    var Flags = (_a = (function (s) {
        if (!s)
            return null;
        try {
            return JSON.parse(s);
        }
        catch (_) {
            return null;
        }
    })(window.localStorage.getItem("LuoguMarkProblem"))) !== null && _a !== void 0 ? _a : {};
    console.log(Flags);
    var enableFor = function (problemId) {
        if (problemId.startsWith("CF"))
            return true;
        return false;
    };
    // 对勾图标的 HTML
    var checkColor = "rgb(82, 196, 26)";
    var checkHtml = "<path data-v-1b44b3e6=\"\" fill=\"currentColor\" d=\"M438.6 105.4c12.5 12.5 12.5 32.8 0 45.3l-256 256c-12.5 12.5-32.8 12.5-45.3 0l-128-128c-12.5-12.5-12.5-32.8 0-45.3s32.8-12.5 45.3 0L160 338.7 393.4 105.4c12.5-12.5 32.8-12.5 45.3 0z\" class=\"\"></path>";
    // 叉号图标的 HTML
    var crossColor = "rgb(231, 76, 60)";
    var crossHtml = "<path data-v-1b44b3e6=\"\" fill=\"currentColor\" d=\"M342.6 150.6c12.5-12.5 12.5-32.8 0-45.3s-32.8-12.5-45.3 0L192 210.7 86.6 105.4c-12.5-12.5-32.8-12.5-45.3 0s-12.5 32.8 0 45.3L146.7 256 41.4 361.4c-12.5 12.5-12.5 32.8 0 45.3s32.8 12.5 45.3 0L192 301.3 297.4 406.6c12.5 12.5 32.8 12.5 45.3 0s12.5-32.8 0-45.3L237.3 256 342.6 150.6z\" class=\"\"></path>";
    var problemListPage = function () {
        var ListClassName = "row-wrap";
        var lists = document.getElementsByClassName(ListClassName);
        if (lists.length !== 1)
            return;
        var children = lists[0].children;
        var _loop_1 = function (i) {
            var problem = children[i];
            var span_list = problem.getElementsByTagName("span");
            var status_icon = span_list[0];
            var problem_id = span_list[1].innerText;
            if (enableFor(problem_id)) {
                var display_1 = function () {
                    var _a;
                    var flag = (_a = Flags[problem_id]) !== null && _a !== void 0 ? _a : null;
                    console.log(problem_id, flag);
                    var svg = status_icon.querySelector("svg");
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
                display_1();
                status_icon.addEventListener("click", function (event) {
                    var _a;
                    var flag = (_a = Flags[problem_id]) !== null && _a !== void 0 ? _a : null;
                    var new_flag = !flag;
                    Flags[problem_id] = new_flag;
                    console.log(Flags);
                    console.log(JSON.stringify(Flags));
                    localStorage.setItem("LuoguMarkProblem", JSON.stringify(Flags));
                    display_1();
                    event.preventDefault();
                });
                status_icon.addEventListener("contextmenu", function (event) {
                    var _a;
                    var flag = (_a = Flags[problem_id]) !== null && _a !== void 0 ? _a : null;
                    var new_flag = (function () {
                        if (flag === null)
                            return true;
                        else
                            return null;
                    })();
                    Flags[problem_id] = new_flag;
                    localStorage.setItem("LuoguMarkProblem", JSON.stringify(Flags));
                    display_1();
                    event.preventDefault();
                });
            }
        };
        for (var i = 0; i < children.length; i++) {
            _loop_1(i);
        }
    };
    window.onload = function () {
        if (window.location.href.includes("training") || window.location.href.includes("problems")) {
            setTimeout(problemListPage, 0);
        }
    };
})();
