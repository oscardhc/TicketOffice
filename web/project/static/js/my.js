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

// alert('1wed1qwex')

var erabtn = document.querySelector('#loginbtn')

if (erabtn) {
    erabtn.onclick = function () {
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
}

var logoutbtn = document.querySelector('#logoutbtn')

if (logoutbtn) {
    logoutbtn.onclick = function () {
        $.post('/logout', {}, function (d) {
            location.reload()
        })
    }
}
