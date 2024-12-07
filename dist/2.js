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
    if (window.location.href !== "https://www.luogu.com.cn/")
        return;
    document
        .querySelector("#app-old > div.lg-index-content.am-center > div:nth-child(1) > div > div > div > div.am-u-md-8")
        ?.remove();
    let rightBar = document.querySelector("#app-old > div.lg-index-content.am-center > div:nth-child(3) > div.am-u-lg-3.am-u-md-4.lg-right");
    let luck = document.querySelector("#app-old > div.lg-index-content.am-center > div:nth-child(1) > div > div > div > div");
    luck.parentElement?.removeChild(luck);
    rightBar.insertBefore(luck, rightBar.firstChild);
    luck.classList.add("lg-article");
    luck.style.width = "100%";
    luck.style.zIndex = "1";
    document.querySelector("#app-old > div.lg-index-content.am-center > div:nth-child(1)")?.remove();
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
    container?.insertBefore(background, container.firstChild);
}
(function () {
    "use strict";
    window.onload = () => {
        modifyMainPage();
    };
    // Your code here...
})();
