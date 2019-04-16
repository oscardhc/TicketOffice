var inst = new mdui.Drawer('#drawer')
var btn = document.querySelector('#menubtn')
var status = 1
// var col =

// method
btn.onclick = function () {
    if (status == 1) {
        inst.close()
    } else {
        inst.open()
    }
    status = 1 - status
}

function eraseBtnClicked () {
    var nm = document.querySelector('#emailinput').value
    var ps = document.querySelector('#passwdinput').value
    // alert(nm + ps)
    $.post('/login', {'user': nm, 'pswd': ps}, function (d) {
        if (d.status == '-1') {
            alert('dismatch!')
        }
        location.reload()
    })
    // window.navigate('/login')
}

var erabtn = document.querySelector('#loginbtn')

if (erabtn) {
    erabtn.onclick = eraseBtnClicked
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
