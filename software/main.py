import sys
from PyQt4 import QtCore, QtGui, uic
from solindar_lib import ComDriver, SolindarGUI

#Implementation

#The following lines open the gui app
if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)
    window = SolindarGUI()
    window.show()
    sys.exit(app.exec_()) 
