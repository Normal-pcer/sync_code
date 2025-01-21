// ==UserScript==
// @name         qiaoqiao2008
// @namespace    http://tampermonkey.net/
// @version      2025-01-09
// @description  try to take over the world!
// @author       sjy
// @match        https://www.luogu.com.cn/
// @match        https://www.luogu.com.cn/*
// @match        https://www.luogu.com/
// @match        https://www.luogu.com/*
// @icon         https://www.google.com/s2/favicons?sz=64&domain=luogu.com
// @grant        none
// ==/UserScript==

let old_usernames=[];
let new_usernames=[];
let uid_usernames=[];

let tags=[];
let username_tags=[];
let uid_tags=[];

function sleep(ms,callback){
    setTimeout(callback,ms)
}

function new_person(){
    let tar=document.getElementById("qq2008-name");
    let old_name=tar.children[1].value,new_name=tar.children[2].value;
    let xml=new XMLHttpRequest();
    xml.open("GET","/api/user/search?keyword="+old_name);
    xml.addEventListener("loadend",()=>{
        let tmp=JSON.parse(xml.response);
        if (tmp.users.length==0) alert("用户未找到!");
        else{
            let id=uid_usernames.indexOf(tmp.users[0].uid.toString());
            if (id==-1){
                uid_usernames.push(tmp.users[0].uid.toString());
                old_usernames.push(tmp.users[0].name);
                new_usernames.push(new_name);
            }else{
                uid_usernames[id]=tmp.users[0].uid.toString();
                old_usernames[id]=tmp.users[0].name;
                new_usernames[id]=new_name;
            }
            alert("添加成功");
            localStorage.setItem("old_usernames",JSON.stringify(old_usernames));
            localStorage.setItem("new_usernames",JSON.stringify(new_usernames));
            localStorage.setItem("uid_usernames",JSON.stringify(uid_usernames));
        }
    });
    xml.send();
}

function del_person(){
    let tar=document.getElementById("qq2008-name");
    let old_name=tar.children[5].value;
    let id1=old_usernames.indexOf(old_name),id2=new_usernames.indexOf(old_name),id3=uid_usernames.indexOf(old_name);
    if (id1==-1 && id2==-1 && id3==-1){
        alert("用户未找到!");
    }else{
        if (id1!=-1){
            uid_usernames.splice(id1,1);
            old_usernames.splice(id1,1);
            new_usernames.splice(id1,1);
        }else if (id2!=-1){
            uid_usernames.splice(id2,1);
            old_usernames.splice(id2,1);
            new_usernames.splice(id2,1);
        }else if (id3!=-1){
            uid_usernames.splice(id3,1);
            old_usernames.splice(id3,1);
            new_usernames.splice(id3,1);
        }
        alert("删除成功");
        localStorage.setItem("old_usernames",JSON.stringify(old_usernames));
        localStorage.setItem("new_usernames",JSON.stringify(new_usernames));
        localStorage.setItem("uid_usernames",JSON.stringify(uid_usernames));
    }
}

function new_tag(){
    let tar=document.getElementById("qq2008-tag");
    let name=tar.children[1].value,tag=tar.children[2].value;
    let xml=new XMLHttpRequest();
    xml.open("GET","/api/user/search?keyword="+name);
    xml.addEventListener("loadend",()=>{
        let tmp=JSON.parse(xml.response);
        if (tmp.users.length==0) alert("用户未找到!");
        else{
            let id=uid_tags.indexOf(tmp.users[0].uid.toString());
            if (id==-1){
                tags.push(tag);
                username_tags.push(tmp.users[0].name);
                uid_tags.push(tmp.users[0].uid.toString());
            }else{
                tags[id]=tag;
                username_tags[id]=tmp.users[0].name;
                uid_tags[id]=tmp.users[0].uid.toString();
            }
            alert("添加成功");
            localStorage.setItem("tags",JSON.stringify(tags));
            localStorage.setItem("username_tags",JSON.stringify(username_tags));
            localStorage.setItem("uid_tags",JSON.stringify(uid_tags));
        }
    });
    xml.send();
}

function del_tag(){
    let tar=document.getElementById("qq2008-tag");
    let name=tar.children[5].value;
    let id1=tags.indexOf(name),id2=username_tags.indexOf(name),id3=uid_tags.indexOf(name);
    if (id1==-1 && id2==-1 && id3==-1){
        alert("用户未找到!");
    }else{
        if (id1!=-1){
            tags.splice(id1,1);
            username_tags.splice(id1,1);
            uid_tags.splice(id1,1);
        }else if (id2!=-1){
            tags.splice(id2,1);
            username_tags.splice(id2,1);
            uid_tags.splice(id2,1);
        }else if (id3!=-1){
            tags.splice(id3,1);
            username_tags.splice(id3,1);
            uid_tags.splice(id3,1);
        }
        alert("删除成功");
        localStorage.setItem("tags",JSON.stringify(tags));
        localStorage.setItem("username_tags",JSON.stringify(username_tags));
        localStorage.setItem("uid_tags",JSON.stringify(uid_tags));
    }
}

function create_name(fa){
    let title1=document.createElement("h2");
    title1.innerHTML="添加外号";
    let box=document.createElement("div");
    box.setAttribute("class","lg-article");
    box.setAttribute("id","qq2008-name");
    let uid1=document.createElement("input");
    uid1.setAttribute("placeholder","输入要修改人的昵称或uid");
    let name=document.createElement("input");
    name.setAttribute("placeholder","输入要修改的名称");
    let btn1=document.createElement("button");
    btn1.innerHTML="修改";
    btn1.onclick=new_person;
    let title2=document.createElement("h2");
    title2.innerHTML="删除外号";
    let uid2=document.createElement("input");
    uid2.setAttribute("placeholder","要删除的昵称/uid或原有的外号");
    let btn2=document.createElement("button");
    btn2.innerHTML="删除";
    btn2.onclick=del_person;
    box.appendChild(title1);
    box.appendChild(uid1);
    box.appendChild(name);
    box.appendChild(btn1);
    box.appendChild(title2);
    box.appendChild(uid2);
    box.appendChild(btn2);
    fa.insertBefore(box,fa.children[0]);
}

function create_tag(fa){
    let title1=document.createElement("h2");
    title1.innerHTML="添加标签";
    let box=document.createElement("div");
    box.setAttribute("class","lg-article");
    box.setAttribute("id","qq2008-tag");
    let uid1=document.createElement("input");
    uid1.setAttribute("placeholder","输入要修改人的昵称或uid");
    let name=document.createElement("input");
    name.setAttribute("placeholder","输入要添加的标签");
    let btn1=document.createElement("button");
    btn1.innerHTML="添加";
    btn1.onclick=new_tag;
    let title2=document.createElement("h2");
    title2.innerHTML="删除标签";
    let uid2=document.createElement("input");
    uid2.setAttribute("placeholder","要删除的昵称/uid或原有的外号");
    let btn2=document.createElement("button");
    btn2.innerHTML="删除";
    btn2.onclick=del_tag;
    box.appendChild(title1);
    box.appendChild(uid1);
    box.appendChild(name);
    box.appendChild(btn1);
    box.appendChild(title2);
    box.appendChild(uid2);
    box.appendChild(btn2);
    fa.insertBefore(box,fa.children[0]);
}

function create(){
    if (window.location.href.search("^https://www.luogu.com(.cn)?/?$")!=-1){
        let fa=document.getElementsByClassName("am-u-lg-3 am-u-md-4 lg-right")[0];
        create_tag(fa);
        create_name(fa);
    }
}

function init(){
    old_usernames=JSON.parse(localStorage.getItem("old_usernames"))??[];
    new_usernames=JSON.parse(localStorage.getItem("new_usernames"))??[];
    uid_usernames=JSON.parse(localStorage.getItem("uid_usernames"))??[];

    tags=JSON.parse(localStorage.getItem("tags"))??[];
    username_tags=JSON.parse(localStorage.getItem("username_tags"))??[];
    uid_tags=JSON.parse(localStorage.getItem("uid_tags"))??[];

    let id=uid_usernames.indexOf("612412");
    if (id==-1){
        uid_usernames.push("612412");
        old_usernames.push("Eibon");
        new_usernames.push("qiaoqiao2008");
    }
    create();
}

function dfs(curr,flag){
    flag=flag||(window.location.href.search("https://www.luogu.com(.cn)?/chat.*")!=-1 && (curr.className=="lfe-caption message" || curr.className=="history"));
    if (curr.children.length==0){
        for (let i=0;i<old_usernames.length;i++){
            if ((new_usernames[i]!="qiaoqiao2008" && flag)) continue;
            if (curr.innerHTML.search(old_usernames[i])!=-1){
                curr.innerHTML=curr.innerHTML.replace(old_usernames[i],new_usernames[i]);
            }
        }
    }else{
        for (let i=0;i<curr.children.length;i++){
            dfs(curr.children[i],flag);
        }
    }
}

var flag=false;

function render(){
    if (flag) return;
    if (window.location.href.search("https://www.luogu.com(.cn)?/paste.*")!=-1) return;
    if (window.location.href.search("https://www.luogu.com(.cn)?/chat.*")!=-1){
        var tar=document.getElementsByClassName("item");
        for (let i=0;i<tar.length;i++){
            tar[i].onclick=function(){
                try{
                    let tmp=document.getElementsByClassName("title");
                    if (this.children[1].children[0].children.length==0){
                        tmp[0].children[0].children[0].children[0].children[0].innerHTML=this.children[1].children[0].innerHTML;//搜索联系人
                    }else{
                        tmp[0].children[0].children[0].children[0].children[0].innerHTML=this.children[1].children[0].children[0].innerHTML;//边栏
                    }//私信标题改名
                }catch(err){
                }
            }
        }
    }//私信
    dfs(document.head,false);
    dfs(document.body,false);
}

const colors=["lg-fg-purple lg-bold","lg-fg-green","lg-fg-bluelight","lg-fg-red lg-bold","lg-fg-orange lg-bold","lg-fg-gray"];
const tag_colors=["lg-bg-purple lg-bold","lg-bg-green","lg-bg-bluelight","lg-bg-red lg-bold","lg-bg-orange lg-bold","lg-bg-gray"];

function render_tag(){
    for (var j=0;j<colors.length;j++){
        let classname=colors[j];
        let tar=document.getElementsByClassName(classname);
        for (let i=0; i<tar.length; i++){
            if (tar[i].attributes.href==undefined) continue;
            let tmp=tar[i].attributes.href.value;
            let pos=tmp.search("/user/");
            if (pos!=-1){
                for (let k=0;k<tags.length;k++){
                    if (tmp.substring(pos+6)==uid_tags[k]){
                        if (tar[i].parentElement.getElementsByClassName("qq2008-newtag").length==0){
                            tar[i].parentElement.innerHTML+="&nbsp;<span class=\"am-badge am-radius "+tag_colors[j]+" qq2008-newtag\">"+tags[k]+"</span>";
                        }
                        break;
                    }
                }
            }
        }
    }//主页
    /*
    if (window.location.href.search("https://www.luogu.com.cn/user.*")!=-1){
        let tar=document.getElementsByClassName("user-name");
        for (let i=0; i<tar.length; i++){
            let tmp=tar[i].children[0];
            let id=new_usernames.index(tmp);
            if (id!=-1){
                tmp=old_usernames[id];
            }
            for (let j=0;j<tags.length;j++){
                if (tmp==username_tags[j]){
                    tar[i].children[0].innerHTML+="&nbsp;<span class=\"am-badge am-radius "+tag_colors[j]+" qq2008-newtag\">"+tags[k]+"</span>";
                }
            }
        }
    }//用户主页
    */
}

function jump(){
    if (!flag){
        if (window.location.href.search("https://www.luogu.com(.cn)?/record.*")!=-1){
            for (let i=0;i<new_usernames.length;i++){
                if (window.location.href.search(new_usernames[i])!=-1){
                    flag=true;
                    window.location.href=window.location.href.replace(new_usernames[i],uid_usernames[i]);
                    return;
                }
            }
        }
    }
}

var lasturl=window.location.href;

function reload(){
    if (window.location.href!=lasturl){
        lasturl=window.location.href;
        if (window.location.href.search("https://www.luogu.com(.cn)?/record.*")!=-1) window.location.reload();
    }
}

(function() {
    'use strict';
    window.onload=init;
    setInterval(jump,300);
    setInterval(render,200);
    setInterval(render_tag,200)
    //sleep(1000,render_tag);
    setInterval(reload,200);
})();