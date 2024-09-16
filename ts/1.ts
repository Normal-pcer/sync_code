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

const wordsList: string[] = JSON.parse(window.localStorage.getItem("fav-words") ?? "[]");
let fastSendButton: HTMLElement | null = null;

class WordIndexManager {
    static _currentWordIndex = 0;

    static set currentWordIndex(value: number) {
        WordIndexManager._currentWordIndex = value % wordsList.length;
        if (fastSendButton) {
            fastSendButton.innerHTML = wordsList[this._currentWordIndex];
        }
    }
    static get currentWordIndex() {
        return WordIndexManager._currentWordIndex;
    }
}

(function () {
    "use strict";

    if (wordsList.length === 0) wordsList.push("喵~");

    const getUserId = () => {
        const userAElementSelector =
            "#app > div.main-container > main > div > div.card.wrapper.padding-none > div.main > div > div.top-container > div.title > span > span > a";
        const userAElement = document.querySelector(userAElementSelector) as undefined | HTMLAnchorElement;
        if (!userAElement) {
            console.log("获取用户信息失败！");
            return 0;
        }
        const link = userAElement.href; // 类似"https://www.luogu.com.cn/user/114514"
        const uid = link.substring(link.length - 6);
        return Number(uid);
    };

    const onPageModifyCallback = () => {
        console.log("test");
        const sendButtonSelector =
            "#app > div.main-container > main > div > div.card.wrapper.padding-none > div.main > div > div.editor > div > button";
        const sendButton = document.querySelector(sendButtonSelector);
        const textAreaSelector =
            "#app > div.main-container > main > div > div.card.wrapper.padding-none > div.main > div > div.editor > textarea";
        const textArea = document.querySelector(textAreaSelector) as undefined | HTMLTextAreaElement;
        if (sendButton && sendButton.getAttribute("modified") !== "true") {
            // 存在“发送”按钮，判定为处于私信界面
            sendButton.setAttribute("modified", "true");
            fastSendButton = sendButton.cloneNode() as HTMLElement;
            const addFavButton = sendButton.cloneNode() as HTMLElement;
            fastSendButton.innerHTML = wordsList[WordIndexManager.currentWordIndex];
            sendButton.parentElement?.insertBefore(fastSendButton, sendButton);
            fastSendButton.addEventListener("click", () => {
                const httpRequest = new XMLHttpRequest();
                httpRequest.open("POST", "https://www.luogu.com.cn/api/chat/new", true);
                httpRequest.setRequestHeader("Content-type", "application/json");
                //// httpRequest.setRequestHeader("referer", "https://www.luogu.com.cn/chat");
                httpRequest.setRequestHeader(
                    "x-csrf-token",
                    (document.querySelector("head > meta[name=csrf-token]") as HTMLMetaElement).content
                );
                httpRequest.send(
                    JSON.stringify({
                        user: getUserId(),
                        content: wordsList[WordIndexManager.currentWordIndex],
                    })
                );
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
        }
    };

    window.onload = () => {
        const observer = new MutationObserver(onPageModifyCallback);
        const targetNode = document.body;

        observer.observe(targetNode, { childList: true, subtree: true, characterData: true });
    };
})();
