import sys

f = open('/home/oscar/tmp.txt','w')
sys.stdout = sys.stderr
f.write("2333333")
sys.path.append('/home/oscar/dhc/TicketOffice/web/project')

from main import app as application