from flask import Flask
from flask import request, redirect
from flask import render_template
from flask import session, make_response, jsonify
from datetime import timedelta
from random import randint
import interaction
import captcha
import sys

BackEndPath = "/Users/oscar/Documents/SJTU/1819_Spring/Data_Structure/TicketOffice/"
# BackEndPath = "/home/oscar/dhc/TicketOffice/"

app = Flask("TTRS", template_folder = BackEndPath + "web/project/templates")
app.config['SEND_FILE_MAX_AGE_DEFAULT'] = timedelta(seconds=1)
app.jinja_env.auto_reload = True
app.secret_key = "19260817"
app.permanent_session_lifetime = timedelta(seconds=60*30)

con = interaction.intereaction(BackEndPath)

@app.route('/')
def hello():
    if 'user' not in session:
        return render_template('index.html', ses = session)
    else:
        return render_template('index.html', ses = session)


@app.route('/login', methods=['POST'])
def login():
    if request.method == 'POST':
        user = request.form['user']
        pswd = request.form['password']
        print('NAME IS ' + user)
        print(request, request.form)

        res = con.exeCmd(['login', user, pswd])

        print("RECEIVED " + res)

        if res == '1' :
            session['user'] = user
            session['headIcon'] = randint(1, 15)
            print(session)
            session.permanent = True
            return jsonify({'status': '0'})
        else :
            return jsonify({'status': '-1'})


@app.route('/logout', methods=['POST'])
def logout():
    if request.method == 'POST':
        print("LOGGING OUT....");
        del session['user']
        session.permanent = True
        return jsonify({'statsu': '0'})

@app.route('/account')
def account():
    return render_template('account.html', ses=session)

@app.route('/query')
def query():
    return render_template('query.html', ses=session)

@app.route('/register', methods=['POST'])
def reg():
    if request.method == 'POST' :
            res = con.exeCmd(['register', request.form['name'], request.form['password'], request.form['email'], request.form['phone']])
            id = int(res)
            return jsonify({'status': '0'})

@app.route('/manage')
def manage():
    return render_template('manage.html', ses=session)

@app.route('/exec', methods=['POST'])
def exec():
    if request.method == 'POST' :
        res = con.exeCmd([request.form['cmd']])
        return jsonify({'result': res})

@app.route('/captcha', methods=['POST'])
def capt():
    if request.method == 'POST' :
        a = captcha.generate()
        return jsonify({'origin':a[0], 'diff':a[1], 'val':str(a[2])})


if __name__ == '__main__':
    # con.init()
    if len(sys.argv) == 3:
        app.run(host=str(sys.argv[1]), port=int(sys.argv[2]))
    else:
        app.run(host="0.0.0.0", port=80)
