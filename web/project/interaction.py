# import os
# import time
#
# write_path = "/tmp/pipe.in"
# read_path = "/tmp/pipe.out"
#
# wf = os.open(write_path, os.O_SYNC | os.O_CREAT | os.O_RDWR)
# rf = None
#
# for i in range(1, 11):
#     msg = "msg " + str(i)
#     len_send = os.write(wf, msg.encode())
#     print("sent msg: %s" % msg)
#
#     if rf is None:
#         rf = os.open(read_path, os.O_RDONLY)
#
#     s = os.read(rf, 1024).decode()
#     if len(s) == 0:
#         break
#     print("received msg: %s" % s)
#
#     time.sleep(1)
#
# os.write(wf, 'exit'.encode())
#
# # os.close(rf)
# # os.close(wf)

import os
import subprocess

class intereaction:

    write_path = "/tmp/pipe.in"
    read_path = "/tmp/pipe.out"
    wf = 0
    rf = 0

    def __init__(self, path):
        print("start exec")
        subprocess.call('cd '+ path + 'backend ; ./init.sh', shell=True)
        print("end exec")
        self.wf = os.open(self.write_path, os.O_SYNC | os.O_CREAT | os.O_RDWR)
        self.rf = None
        subprocess.Popen('cd ' + path + 'backend ; ./run.sh', shell=True)

    def exeCmd(self, _msg):

        msg = ' '.join(_msg)

        len_send = os.write(self.wf, msg.encode())
        print("sent msg: [%s]" % msg)

        if self.rf is None:
            self.rf = os.open(self.read_path, os.O_RDONLY)

        while True:
            s = os.read(self.rf, 1024).decode()
            if s == "":
                continue
            print("received msg: [%s]" % s)
            break

        print("msg is " + s)
        return s

    def close(self):
        os.close(self.rf)
        os.close(self.wf)
