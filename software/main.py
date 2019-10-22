import matplotlib
matplotlib.use('Qt5Agg')
import sys
from PyQt4 import QtCore, QtGui, uic
from solindar_lib import ComDriver, SolindarGUI, TestCom
import qdarkstyle

#Implementation
ts=1000 #sampling time in ms
refresh_time=1000 #time for updating graph in ms
len_fifo = 63
n_block = 10
Port = "COM3"
# con = TestCom.testCom(len_fifo,n_block)
con = ComDriver(Port,len_fifo,n_block)

#The following lines open the gui app
if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)
    app.setStyleSheet(qdarkstyle.load_stylesheet_pyqt())
    window = SolindarGUI(con,ts,refresh_time)
    window.show()
    sys.exit(app.exec_()) 
