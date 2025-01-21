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
    uid: number;
    username: string;
    constructor(uid: number, username: string) {
        this.uid = uid;
        this.username = username;
    }
}
let uidToIdentifier: Map<number, UserIdentifier> = new Map();
let usernameToIdentifier: Map<string, UserIdentifier> = new Map();

class UsernameAlias {
    id: UserIdentifier;
    newName: string;
    constructor(id: UserIdentifier, newName: string) {
        this.id = id;
        this.newName = newName;
    }

    /**
     * 在当前文档中应用别名
     */
    apply(): void {
        function dfs(p: HTMLElement, alias: UsernameAlias): void {
            if (p.children.length == 0) {
                // 到达叶子节点，进行替换
                p.innerText = p.innerText.replaceAll(alias.id.username, alias.newName);
            } else {
                for (let element of p.children) {
                    if (element instanceof HTMLElement) {
                        dfs(element, alias);
                    }
                }
            }
        }
        dfs(document.body, this);
    }
};
let aliasList: Array<UsernameAlias> = new Array();

abstract class SettingBoxItem {
    abstract createElement(): HTMLElement;
    abstract getValue(): string;
};
class SettingBoxItemText implements SettingBoxItem {
    element: HTMLElement | null = null;
    placeholder: string;
    constructor(placeholder: string) {
        this.placeholder = placeholder;
    }
    createElement(): HTMLElement {
        alert("111")
        if (this.element !== null) {
            throw "SettingBoxItemText::createElement(): this.element is not null.";
        }
        let new_element = document.createElement("input");
        new_element.placeholder = this.placeholder;
        this.element = new_element;
        return new_element;
    }
    getValue(): string {
        if (this.element instanceof HTMLInputElement) {
            return this.element.value;
        } else {
            throw "SettingBoxItemText::getValue(): this.element is not HTMLInputElement.";
            return "";
        }
    }
}
/**
 * 位于主页的设置块
 */
class SettingBox {
    title: string
    items: Array<SettingBoxItem> = [];

    constructor(title: string) {
        this.title = title;
    }
};


(function () {
    'use strict';

    // Your code here...
    setInterval(() => {
        const sel = "#app-old > div.lg-index-content.am-center > div:nth-child(1) > div > div > div > div.am-u-md-4.lg-punch.am-text-center";
        let element = document.querySelector(sel) as HTMLDivElement;
        let txt = new SettingBoxItemText("test");
        element.appendChild(txt.createElement());
    }, 2000);
})();