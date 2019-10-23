import matplotlib
matplotlib.use('Qt5Agg')
import sys
from PyQt4 import QtCore, QtGui, uic
from solindar_lib import ComDriver, SolindarGUI, testCom
import qdarkstyle

#Implementation
ts=300 #sampling time in ms
len_fifo = 63
n_block = 5
Port = "COM3"
# con = TestCom.testCom(len_fifo,n_block)
con = ComDriver(Port,len_fifo,n_block)

#The following lines open the gui app
if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)
    app.setStyleSheet(qdarkstyle.load_stylesheet_pyqt())
    window = SolindarGUI(con,ts)
    window.show()
    sys.exit(app.exec_())
