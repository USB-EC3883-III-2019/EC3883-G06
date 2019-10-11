import sys
from PyQt4 import QtCore, QtGui, uic
from solindar_lib import ComDriver, SolindarGUI, TestCom

#Implementation
ts=100 #sampling time in ms
refresh_time=100 #time for updating graph in ms
len_fifo = 63
n_block = 10
con = TestCom.testCom(len_fifo,n_block)

#The following lines open the gui app
if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)
    window = SolindarGUI(con,ts,refresh_time)
    window.show()
    sys.exit(app.exec_()) 
