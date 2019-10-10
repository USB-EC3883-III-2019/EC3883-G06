from matplotlib.figure import Figure
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT as NavigationToolbar
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
import matplotlib.pyplot as plt
from PyQt4 import QtCore, QtGui, uic
import sys
from ..ComDriver import ComDriver
from .canvas import Canvas
import matplotlib
matplotlib.use('Qt5Agg')

qtCreatorFile = "solindar_lib/GUI/graph5.ui" # my Qt Designer file 

Ui_MainWindow, QtBaseClass = uic.loadUiType(qtCreatorFile)

class SolindarGUI(QtGui.QMainWindow, Ui_MainWindow):
    def __init__(self):
        QtGui.QMainWindow.__init__(self)
        Ui_MainWindow.__init__(self)
        self.setupUi(self)
        self.navigation = NavigationToolbar(self.canvas, self)
        self.hlayout.addWidget(self.navigation)

        self.gCheck.stateChanged.connect(self.Grid)
        self.gCheck.setStatusTip("Colocar Grid en la grafica")
        # self.tBox.activated[str].connect(self.changeTimeScale)
        # self.tBox.setStatusTip("Cambiar la escala temporal")
        # self.aBox.activated[str].connect(self.changeAmpScale)
        # self.aBox.setStatusTip("Cambiar la escala de Amplitud")

        # timer = QtCore.QTimer(self)
        # timer.timeout.connect(self.tick)
        # timer.start(1000)

        self.plot()

    def tick(self):
        print("Timeout")

    def Grid(self, state):
        if state == QtCore.Qt.Checked:
            print("Checked")  # Colocar Grid
        else:
            print("Unchecked")  # Quitar Grid

    def plot(self):
         # Colocar la funcion que comienza el plot
        x = [1, 2, 3, 4, 5]
        y = [2, 4, 6, 8, 10]
        ax = self.canvas.figure.add_subplot(111)
        ax.plot(x, y)
        self.canvas.draw()
