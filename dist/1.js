"use strict";
// ==UserScript==
// @name         Favor-words and Encryption For LUOGU
// @namespace    http://tampermonkey.net/
// @version      2024-09-11
// @description  Favor-words and Encryption For LUOGU
// @author       You
// @match        https://www.luogu.com.cn
// @match        https://www.luogu.com.cn/*
// @icon         https://www.google.com/s2/favicons?sz=64&domain=luogu.com.cn
// @grant        none
// ==/UserScript==
var _1;
(function (_1) {
    const base64 = Array.from("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
    const blankChars = ["\u200b", "\u200d", "\u200e", "\u202a", "\u202b", "\u202c", "\u202d", "喵"];
    const wordsList = JSON.parse(window.localStorage.getItem("fav-words") ?? "[]");
    let fastSendButton = null;
    function getUserId() {
        const userAElementSelector = "#app > div.main-container > main > div > div.card.wrapper.padding-none > div.main > div > div.top-container > div.title > span > span > a";
        const userAElement = document.querySelector(userAElementSelector);
        if (!userAElement) {
            console.log("获取用户信息失败！");
            return 0;
        }
        const link = userAElement.href; // 类似"https://www.luogu.com.cn/user/114514"
        const uid = link.substring(link.length - 6);
        return Number(uid);
    }
    ;
    class WordIndexManager {
        static _currentWordIndex = 0;
        static set currentWordIndex(value) {
            WordIndexManager._currentWordIndex = value % wordsList.length;
            if (fastSendButton) {
                fastSendButton.innerHTML = wordsList[this._currentWordIndex];
            }
        }
        static get currentWordIndex() {
            return WordIndexManager._currentWordIndex;
        }
    }
    function encryptMessage(msg) {
        let result = msg;
        let arr = Array.from(window.btoa(window.encodeURI(msg))).filter((element) => element != '=');
        arr = arr.map((element) => {
            const index = base64.indexOf(element);
            return blankChars[index >>> 3] + blankChars[index % 8];
        });
        result = blankChars[0] + blankChars[0] + blankChars[0] + blankChars[7] + arr.reduce((a, b) => a + b); // 放入0007表示由本插件生成
        return result;
    }
    function decryptMessage(msg) {
        let result = "";
        for (let i = 0; i < msg.length; i += 2)
            result += base64[(blankChars.indexOf(msg[i]) << 3) + (blankChars.indexOf(msg[i + 1]))];
        if (result.substring(0, 2) !== "AH")
            return false;
        result = result.substring(2);
        return window.decodeURI(window.atob(result));
    }
    function sendMessage(msg) {
        const httpRequest = new XMLHttpRequest();
        httpRequest.open("POST", "https://www.luogu.com.cn/api/chat/new", true);
        httpRequest.setRequestHeader("Content-type", "application/json");
        //// httpRequest.setRequestHeader("referer", "https://www.luogu.com.cn/chat");
        httpRequest.setRequestHeader("x-csrf-token", document.querySelector("head > meta[name=csrf-token]").content);
        httpRequest.send(JSON.stringify({
            user: getUserId(),
            content: msg,
        }));
    }
    (function () {
        "use strict";
        if (wordsList.length === 0)
            wordsList.push("喵~");
        const onPageModifyCallback = () => {
            const sendButtonSelector = "#app > div.main-container > main > div > div.card.wrapper.padding-none > div.main > div > div.editor > div > button";
            const sendButton = document.querySelector(sendButtonSelector);
            const textAreaSelector = "#app > div.main-container > main > div > div.card.wrapper.padding-none > div.main > div > div.editor > textarea";
            const textArea = document.querySelector(textAreaSelector);
            if (sendButton && sendButton.getAttribute("modified") !== "true") {
                // 存在“发送”按钮，判定为处于私信界面
                sendButton.setAttribute("modified", "true");
                fastSendButton = sendButton.cloneNode();
                const addFavButton = sendButton.cloneNode();
                fastSendButton.innerHTML = wordsList[WordIndexManager.currentWordIndex];
                sendButton.parentElement?.insertBefore(fastSendButton, sendButton);
                fastSendButton.addEventListener("click", () => {
                    sendMessage(wordsList[WordIndexManager.currentWordIndex]);
                });
                fastSendButton.addEventListener("contextmenu", (event) => {
                    // 下一个收藏项
                    WordIndexManager.currentWordIndex = 1 + WordIndexManager.currentWordIndex;
                    event.preventDefault(); // 阻止弹出右键菜单
                });
                addFavButton.innerHTML = "收藏文字";
                addFavButton.addEventListener("click", () => {
                    let text = textArea?.value;
                    if (text && text.length > 0) {
                        if (wordsList.includes(text)) {
                            alert(`您已经收藏过 "${text}" 了！`);
                        }
                        wordsList.push(text);
                        localStorage.setItem("fav-words", JSON.stringify(wordsList));
                    }
                });
                addFavButton.addEventListener("contextmenu", (event) => {
                    alert('删除常用语 "' + wordsList.splice(WordIndexManager.currentWordIndex, 1)[0] + '"');
                    if (wordsList.length === 0) {
                        wordsList.push("喵~");
                        alert("所有常用语均已删除，恢复默认状态。");
                    }
                    WordIndexManager.currentWordIndex = 0;
                    localStorage.setItem("fav-words", JSON.stringify(wordsList));
                    event.preventDefault();
                });
                sendButton.parentElement?.insertBefore(addFavButton, fastSendButton);
                sendButton.addEventListener("contextmenu", (event) => {
                    const inputText = (textArea?.value);
                    console.log(inputText);
                    if (inputText) {
                        sendMessage(encryptMessage(inputText));
                        event.preventDefault();
                    }
                });
            }
        };
        const intervalCallback = () => {
            document.querySelectorAll(".message").forEach((element) => {
                if (element instanceof HTMLElement /* && element.getAttribute("decryptionPrepared") !== "1"*/) {
                    element.addEventListener("click", () => {
                        let decrypted = decryptMessage(element.innerText);
                        if (decrypted) {
                            element.innerText = decrypted;
                            element.setAttribute("decryptionPrepared", "1");
                        }
                        else if (element.getAttribute("decryptionPrepared") === "1") {
                            element.innerText = encryptMessage(element.innerText);
                            element.setAttribute("decryptionPrepared", "0");
                        }
                    });
                }
            });
        };
        window.onload = () => {
            const observer = new MutationObserver(onPageModifyCallback);
            const targetNode = document.body;
            observer.observe(targetNode, { childList: true, subtree: true, characterData: true });
            setInterval(intervalCallback, 500);
        };
    })();
})(_1 || (_1 = {}));
