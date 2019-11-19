import matplotlib
matplotlib.use('Qt5Agg')
import sys
from PyQt4 import QtCore, QtGui, uic
from solindar_lib import ComDriver, SolindarGUI, testCom
import qdarkstyle

# #Implementation
#Parameters
Port = "COM3"
len_fifo=10
n_block=10

con = ComDriver(Port,len_fifo,n_block)
# con=1
#The following lines open the gui app
if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)
    app.setStyleSheet(qdarkstyle.load_stylesheet_pyqt())
    window = SolindarGUI(con)
    window.show()
    sys.exit(app.exec_())
