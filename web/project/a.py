from flask import Flask
from flask import request, redirect
from flask import render_template
from flask import session, make_response, jsonify
from datetime import timedelta
from lg import Database

app = Flask("Test")
app.config['SEND_FILE_MAX_AGE_DEFAULT'] = timedelta(seconds=1)
app.jinja_env.auto_reload = True
app.secret_key = "19260817"
app.permanent_session_lifetime = timedelta(seconds=300)


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
        pswd = request.form['pswd']
        print('NAME IS', user)
        print(request, request.form)

        if pswd == user + '1' :
            session['user'] = user
            session.permanent = True
            return jsonify({'status': '0'})
        else :
            return jsonify({'status': '-1'})


@app.route('/logout', methods=['POST'])
def logout():
    if request.method == 'POST':
        del session['user']
        session.permanent = True
        return jsonify({'statsu': '0'})


if __name__ == '__main__':
    app.run(port=9999)
