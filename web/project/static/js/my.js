var errorDialog = new mdui.Dialog('#errorDialog');
document.querySelector('#errorDialog').addEventListener('opened.mdui.dialog', function () {
    document.querySelector('#cancelBtn').focus()
    // document.querySelector('#errorMsg').textContent = document.activeElement.id
})

function openError(msg) {
    // mdui.alert(msg, 'ERROR');
    document.querySelector('#errorMsg').textContent = msg
    errorDialog.open()
}

var loginDialogue = new mdui.Dialog('#exampleDialog')


function openLogin() {
    loginDialogue.open()
}


var processDialogue = new mdui.Dialog('#processDialogue')

function eraseBtnClicked () {
    var nm = document.querySelector('#emailinput').value
    var ps = document.querySelector('#passwdinput').value

    // alert(location.href)

    // alert(nm + ps)
    loginDialogue.close()
    processDialogue.open()
    // document.querySelector('#baseDiv').setAttribute('class', 'disabled')
    var classVal = document.querySelector("body").getAttribute("class")
    document.querySelector("body").setAttribute('class', classVal + ' disabled')

    $.post('/login', {'user': nm, 'pswd': ps}, function (d) {
        // alert(d.status === '-1')
        processDialogue.close()
        document.querySelector("body").setAttribute('class', classVal)

        if (d.status === '-1') {
            openError('用户名或密码错误')
        } else {
            // document.execCommand('Refresh')
            var url = location.href.replace('#mdui-dialog','')
            // alert(url)
            location.href = url
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
            // alert('logged out!')
            // location.reload()
            location.href = "/"
            // alert('reloaded!')
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
    fetch('https://v1.hitokoto.cn/?c=a ')
        .then(function (res) {
            return res.json();
        })
        .then(function (data) {
            hitokoto.innerHTML = "&nbsp &nbsp &nbsp &nbsp" +  data.hitokoto;
            document.querySelector('#hitokotoSource').innerHTML = "—— " + data.from
        })
}

var data = [
    ['c100','2018-03-28 08:00','2018-03-28 08:23',[['一等座',2000,765.50],['二等座',2000,765.49],['三等座',2000,765.48]],'c','name1'],
    ['c200','2018-03-28 10:00','2018-03-28 10:23',[['一等座',2000,965.50],['二等座',2000,965.49],['三等座',2000,965.48]],'c','name2'],
    ['c300','2018-03-29 10:00','2018-03-29 10:23',[['一等座',2000,2265.50],['二等座',2000,265.49],['三等座',2000,265.48]],'c','name3']
]

var tagname = [
    '车次    ：','发车时间：','到达时间：','座位情况：'
]

function createPanel() {
    var plc = document.querySelector('#panelPlace')
    var row = data.length

    for (var i=0;i<row;++i) {
        var pnl = document.createElement('div')
        pnl.setAttribute('class', 'mdui-panel-item mdui-hoverable')
        pnl.setAttribute('id', 'panel' + i)
        var hed = document.createElement('div')
        hed.setAttribute('class', 'mdui-panel-item-header')
        hed.innerText = data[i][0]
        hed.innerHTML += '<i class="mdui-panel-item-arrow mdui-icon material-icons">keyboard_arrow_down</i>'
        var bod = document.createElement('div')
        bod.setAttribute('class', 'mdui-panel-item-body')
        // bod.innerText = data[i]

        var lst = document.createElement('ul')
        lst.setAttribute('class', 'mdui-list')

        for (var j=0;j<4;j++) {
            var c = document.createElement('li')
            c.setAttribute('class', 'mdui-list-item mdui-ripple')
            c.innerText = tagname[j] + data[i][j]
            lst.append(c)
        }

        var btn = document.createElement('button')
        btn.setAttribute('class', 'mdui-btn mdui-ripple mdui-color-theme-accent')
        btn.innerText = "购买"

        bod.setAttribute('align', 'right')

        bod.append(lst)
        bod.append(btn)

        pnl.append(hed)
        pnl.append(bod)
        plc.append(pnl)
    }
}

if (document.querySelector('#panelPlace')) {
    createPanel()
}

var detailDialog = new mdui.Dialog('#detailDialogue');

function clickedAtRow(x) {
    var tx = document.createElement('div')
    tx.setAttribute('class', 'mdui-typo')
    var len = data[x][3].length
    for(var i=0;i<len;i++) {
        tx.innerHTML += "<h1 class='mdui-text-color-theme' style='display: inline;'>" + data[x][3][i][0] + "</h1>"
        tx.innerHTML += "<p style='display: inline;'>" + ": 余" + data[x][3][i][1] + "张" + "</p> <br/>"
    }
    document.querySelector('#detailMsg').innerHTML = ""
    document.querySelector('#detailMsg').append(tx)


    detailDialog.open()
    document.querySelector('#detailCancleBtn').focus()
    // alert(row)
}