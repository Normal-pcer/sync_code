"use strict";
// ==UserScript==
// @name         Luogu Alias And Customize Tags
// @namespace    http://tampermonkey.net/
// @version      2025-01-21
// @description  try to take over the world!
// @author       normalpcer
// @match        https://www.luogu.com.cn/*
// @match        https://www.luogu.com/*
// @icon         https://www.google.com/s2/favicons?sz=64&domain=luogu.com.cn
// @grant        none
// ==/UserScript==
/**
 * 用于确定一个用户
 */
class UserIdentifier {
    uid;
    username;
    constructor(uid, username) {
        this.uid = uid;
        this.username = username;
    }
}
let uidToIdentifier = new Map();
let usernameToIdentifier = new Map();
class UsernameAlias {
    id;
    newName;
    constructor(id, newName) {
        this.id = id;
        this.newName = newName;
    }
    /**
     * 在当前文档中应用别名
     */
    apply() {
        function dfs(p, alias) {
            if (p.children.length == 0) {
                // 到达叶子节点，进行替换
                p.innerText = p.innerText.replaceAll(alias.id.username, alias.newName);
            }
            else {
                for (let element of p.children) {
                    if (element instanceof HTMLElement) {
                        dfs(element, alias);
                    }
                }
            }
        }
        dfs(document.body, this);
    }
}
;
let aliasList = new Array();
class SettingBoxItem {
}
;
class SettingBoxItemText {
    element = null;
    placeholder;
    constructor(placeholder) {
        this.placeholder = placeholder;
    }
    createElement() {
        alert("111");
        if (this.element !== null) {
            throw "SettingBoxItemText::createElement(): this.element is not null.";
        }
        let new_element = document.createElement("input");
        new_element.placeholder = this.placeholder;
        this.element = new_element;
        return new_element;
    }
    getValue() {
        if (this.element instanceof HTMLInputElement) {
            return this.element.value;
        }
        else {
            throw "SettingBoxItemText::getValue(): this.element is not HTMLInputElement.";
            return "";
        }
    }
}
/**
 * 位于主页的设置块
 */
class SettingBox {
    title;
    items = [];
    constructor(title) {
        this.title = title;
    }
}
;
(function () {
    'use strict';
    // Your code here...
    setInterval(() => {
        const sel = "#app-old > div.lg-index-content.am-center > div:nth-child(1) > div > div > div > div.am-u-md-4.lg-punch.am-text-center";
        let element = document.querySelector(sel);
        let txt = new SettingBoxItemText("test");
        element.appendChild(txt.createElement());
    }, 2000);
})();
