"use strict";
// ==UserScript==
// @name         Customized LUOGU UI
// @namespace    http://tampermonkey.net/
// @version      2024-09-19
// @description  test
// @author       You
// @match        https://www.luogu.com.cn/*
// @match        https://www.luogu.com/*
// @icon         https://www.google.com/s2/favicons?sz=64&domain=luogu.com.cn
// @grant        none
// ==/UserScript==
const backgroundURL = "https://cdn.luogu.com.cn/upload/image_hosting/n1yvs9t8.png";
const blankBackgroundStyle = "linear-gradient(156deg, rgb(51, 51, 51), rgb(71, 176, 223), rgb(244, 226, 250), rgb(227, 245, 255)); filter: blur(3px) brightness(87%);";
function modifyMainPage() {
    var _a, _b, _c;
    (_a = document
        .querySelector("#app-old > div.lg-index-content.am-center > div:nth-child(1) > div > div > div > div.am-u-md-8")) === null || _a === void 0 ? void 0 : _a.remove();
    let rightBar = document.querySelector("#app-old > div.lg-index-content.am-center > div:nth-child(3) > div.am-u-lg-3.am-u-md-4.lg-right");
    let luck = document.querySelector("#app-old > div.lg-index-content.am-center > div:nth-child(1) > div > div > div > div");
    (_b = luck.parentElement) === null || _b === void 0 ? void 0 : _b.removeChild(luck);
    rightBar.insertBefore(luck, rightBar.firstChild);
    luck.classList.add("lg-article");
    luck.style.width = "100%";
    luck.style.zIndex = "1";
    (_c = document.querySelector("#app-old > div.lg-index-content.am-center > div:nth-child(1)")) === null || _c === void 0 ? void 0 : _c.remove();
    let background = document.createElement("img");
    background.src = backgroundURL;
    background.alt = "background";
    background.style.position = "fixed";
    background.style.top = "4em";
    background.style.height = "120vh";
    let upBar = document.querySelector("#app > div.main-container > div.wrapper.wrapped.lfe-body.header-layout.tiny > div.container");
    upBar.style.position = "sticky";
    upBar.style.zIndex = "1";
    upBar.style.background = blankBackgroundStyle;
    let container = document.querySelector("#app-old");
    container === null || container === void 0 ? void 0 : container.insertBefore(background, container.firstChild);
}
(function () {
    "use strict";
    window.onload = () => {
        modifyMainPage();
    };
    // Your code here...
})();
