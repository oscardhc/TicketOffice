import os
import time

write_path = "/tmp/pipe.in"
read_path = "/tmp/pipe.out"

wf = os.open(write_path, os.O_SYNC | os.O_CREAT | os.O_RDWR)
rf = None

for i in range(1, 11):
    msg = "hhh " + str(i)
    len_send = os.write(wf, msg.encode())
    print("sent msg: %s" % msg)

    if rf is None:
        rf = os.open(read_path, os.O_RDONLY)

    s = os.read(rf, 1024).decode()
    if len(s) == 0:
        break
    print("received msg: %s" % s)

    time.sleep(1)

os.write(wf, 'exit'.encode())

os.close(rf)
os.close(wf)
