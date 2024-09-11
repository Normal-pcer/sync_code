// ==UserScript==
// @name         New User Script
// @namespace    http://tampermonkey.net/
// @version      2024-09-11
// @description  try to take over the world!
// @author       You
// @match        https://www.luogu.com.cn
// @match        https://www.luogu.com.cn/*
// @icon         https://www.google.com/s2/favicons?sz=64&domain=luogu.com.cn
// @grant        none
// ==/UserScript==
var _a;
var wordsList = JSON.parse((_a = window.localStorage.getItem("fav-words")) !== null && _a !== void 0 ? _a : "[]");
var fastSendButton = null;
var WordIndexManager = /** @class */ (function () {
    function WordIndexManager() {
    }
    Object.defineProperty(WordIndexManager, "currentWordIndex", {
        get: function () {
            return WordIndexManager._currentWordIndex;
        },
        set: function (value) {
            WordIndexManager._currentWordIndex = value % wordsList.length;
            if (fastSendButton) {
                fastSendButton.innerHTML = wordsList[this._currentWordIndex];
            }
        },
        enumerable: false,
        configurable: true
    });
    WordIndexManager._currentWordIndex = 0;
    return WordIndexManager;
}());
(function () {
    "use strict";
    if (wordsList.length === 0)
        wordsList.push("喵~");
    var getUserId = function () {
        var userAElementSelector = "#app > div.main-container > main > div > div.card.wrapper.padding-none > div.main > div > div.top-container > div.title > span > span > a";
        var userAElement = document.querySelector(userAElementSelector);
        if (!userAElement) {
            console.log("获取用户信息失败！");
            return 0;
        }
        var link = userAElement.href; // 类似"https://www.luogu.com.cn/user/114514"
        var uid = link.substring(link.length - 6);
        return Number(uid);
    };
    var onPageModifyCallback = function () {
        var _a, _b;
        console.log("test");
        var sendButtonSelector = "#app > div.main-container > main > div > div.card.wrapper.padding-none > div.main > div > div.editor > div > button";
        var sendButton = document.querySelector(sendButtonSelector);
        var textAreaSelector = "#app > div.main-container > main > div > div.card.wrapper.padding-none > div.main > div > div.editor > textarea";
        var textArea = document.querySelector(textAreaSelector);
        if (sendButton && sendButton.getAttribute("modified") !== "true") {
            // 存在“发送”按钮，判定为处于私信界面
            sendButton.setAttribute("modified", "true");
            fastSendButton = sendButton.cloneNode();
            var addFavButton = sendButton.cloneNode();
            fastSendButton.innerHTML = wordsList[WordIndexManager.currentWordIndex];
            (_a = sendButton.parentElement) === null || _a === void 0 ? void 0 : _a.insertBefore(fastSendButton, sendButton);
            fastSendButton.addEventListener("click", function () {
                var httpRequest = new XMLHttpRequest();
                httpRequest.open("POST", "https://www.luogu.com.cn/api/chat/new", true);
                httpRequest.setRequestHeader("Content-type", "application/json");
                //// httpRequest.setRequestHeader("referer", "https://www.luogu.com.cn/chat");
                httpRequest.setRequestHeader("x-csrf-token", document.querySelector("head > meta[name=csrf-token]").content);
                httpRequest.send(JSON.stringify({ user: getUserId(), content: wordsList[WordIndexManager.currentWordIndex] }));
            });
            fastSendButton.addEventListener("contextmenu", function (event) {
                // 下一个收藏项
                WordIndexManager.currentWordIndex = (1 + WordIndexManager.currentWordIndex);
                event.preventDefault(); // 阻止弹出右键菜单
            });
            addFavButton.innerHTML = "收藏文字";
            addFavButton.addEventListener("click", function () {
                var text = textArea === null || textArea === void 0 ? void 0 : textArea.value;
                if (text && text.length > 0) {
                    wordsList.push(text);
                    localStorage.setItem("fav-words", JSON.stringify(wordsList));
                }
            });
            addFavButton.addEventListener("contextmenu", function (event) {
                if (wordsList.length === 1)
                    return;
                alert('删除常用语 "' + wordsList.splice(WordIndexManager.currentWordIndex, 1)[0] + '"');
                WordIndexManager.currentWordIndex = 0;
                event.preventDefault();
            });
            (_b = sendButton.parentElement) === null || _b === void 0 ? void 0 : _b.insertBefore(addFavButton, fastSendButton);
        }
    };
    window.onload = function () {
        var observer = new MutationObserver(onPageModifyCallback);
        var targetNode = document.body;
        observer.observe(targetNode, { childList: true, subtree: true, characterData: true });
    };
})();
