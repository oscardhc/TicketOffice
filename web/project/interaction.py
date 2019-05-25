
import subprocess
import zmq
import time

class intereaction:


    def __init__(self, path):
        global socket
        print("start exec")
        subprocess.call('cd '+ path + 'backend ; ./init.sh', shell=True)
        print("end exec")
        subprocess.Popen('cd ' + path + 'backend ; ./run.sh', shell=True)
        time.sleep(5)
        context = zmq.Context()
        print('connect to hello world server')
        socket = context.socket(zmq.REQ)
        socket.connect('tcp://localhost:5555')

    def exeCmd(self, _msg):
        msg = ' '.join(_msg)
        print('exec ' + msg)
        socket.send_string(msg)
        message = socket.recv_string()
        print('received ' + message)
        return message

    def close(self):
        pass