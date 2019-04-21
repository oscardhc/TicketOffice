var errorDialog = new mdui.Dialog('#errorDialog');

function openError(msg) {
    document.querySelector('#errorMsg').textContent = msg
    errorDialog.open()
    document.querySelector('#cancleBtn').focus()
}

function eraseBtnClicked () {
    var nm = document.querySelector('#emailinput').value
    var ps = document.querySelector('#passwdinput').value
    // alert(nm + ps)
    $.post('/login', {'user': nm, 'pswd': ps}, function (d) {
        // alert(d.status === '-1')
        if (d.status === '-1') {
            openError("用户名或密码错误")
        } else {
            location.reload()
        }
    })
    // window.navigate('/login')
}

function registerBtnClicked () {
    var nm = document.querySelector('#emailinputR').value
    var ps1 = document.querySelector('#passwdinputR1').value
    var ps2 = document.querySelector('#passwdinputR2').value

    if (ps1 !== ps2) {
        document.querySelector('#errorMsg').textContent = "两次输入密码不一致"
        errorDialog.open()
    }

    // $.post('/login', {'user': nm, 'pswd': ps}, function (d) {
    //     if (d.status == '-1') {
    //         alert('dismatch!')
    //     }
    //     location.reload()
    // })
    // window.navigate('/login')
}


var logoutbtn = document.querySelector('#logoutbtn')

if (logoutbtn) {
    logoutbtn.onclick = function () {
        $.post('/logout', {}, function (d) {
            location.reload()
        })
    }
}

function ClickUsername() {
    if (event.keyCode == 13) {
        var pswd = document.querySelector('#passwdinput')
        pswd.focus()
        event.returnValue = false
        return false
    }
}

function ClickPassword() {
    if (event.keyCode == 13) {
        // alert("enter pressed")
        eraseBtnClicked()
        event.returnValue = false
        return false
    }
}

var headicon = document.querySelector('#headicon')
if (headicon) {
    headicon.onclick = function () {
        location.href = "/account"
    }
}

var hitokoto = document.querySelector('#hitokoto')
if (hitokoto) {
    fetch('https://v1.hitokoto.cn')
        .then(function (res) {
            return res.json();
        })
        .then(function (data) {
            hitokoto.innerText = data.hitokoto;
            document.querySelector('#hitokotoSource').innerHTML = "—— " + data.from
        })
}


function createTable(){
    tableNode=document.querySelector('#tbody')
    var row = 5, cols = 7;
    //上面确定了 现在开始创建

    for(var x=0;x<row;x++){
        var trNode=tableNode.insertRow();
        for(var y=0;y<cols;y++){
            var tdNode=trNode.insertCell();
            tdNode.innerHTML="单元格"+(x+1)+"-"+(y+1);
        }
        var col = document.createElement('td')
        var btn = document.createElement('button')
        btn.setAttribute('class', 'mdui-btn mdui-ripple mdui-color-theme-accent')
        btn.innerText = '1'
        btn.setAttribute('mdui-dialog', "{target: '#detailDialogue'}")
        col.append(btn)
        trNode.append(col)
    }
}

var tablePlace = document.querySelector('#tbody')
if (tablePlace) {
    createTable()
}

