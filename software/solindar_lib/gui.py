from matplotlib.figure import Figure
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT as NavigationToolbar
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
import matplotlib.pyplot as plt
from PyQt4 import QtCore, QtGui, uic
import sys
from . ComDriver import ComDriver
from . TestCom import testCom
import matplotlib
matplotlib.use('Qt5Agg')
import sys
import serial
import numpy as np

qtCreatorFile = "solindar_lib/graph6.ui" # my Qt Designer file 

Ui_MainWindow, QtBaseClass = uic.loadUiType(qtCreatorFile)

class SolindarGUI(QtGui.QMainWindow, Ui_MainWindow):
    def __init__(self,con,ts,refresh_time):
        QtGui.QMainWindow.__init__(self)
        Ui_MainWindow.__init__(self)
        self.setupUi(self)
        self.navigation = NavigationToolbar(self.canvas, self)
        self.hlayout.addWidget(self.navigation)
        self.gCheck.stateChanged.connect(self.Grid) #If grid button is checked executes Grid function
        self.gCheck.setStatusTip("Colocar Grid en la grafica")
        self.fCheck.stateChanged.connect(self.filter)
        self.fCheck.setStatusTip("Activar el filtro")
        self.sonBox.stateChanged.connect(self.ChannelSon)
        self.sonBox.setStatusTip("Activar la Grafica del Sonar")
        self.lidBox.stateChanged.connect(self.ChannelLid)
        self.lidBox.setStatusTip("Activar la Grafica del Lidar")
        self.fusBox.stateChanged.connect(self.ChannelFus)
        self.fusBox.setStatusTip("Activar la Grafica del Fusion")
        self.con=con
        self.Ch_state=[False, False, False]
        self.Ch_index=[]
        self.Ch_colors=['b','g','y','r']
        self.pos_conv = 2 * np.pi / 84
        self.ax = self.canvas.figure.add_subplot(111, projection = 'polar')
        #self.ax.set_xticklabels([]) #deletes labels from X-axes
        self.lines = self.ax.plot()
        self.set_grid=False
        plt.ion()


        #Timer 1
        timer1=QtCore.QTimer(self)
        timer1.timeout.connect(self.con.update)
        timer1.start(ts)

        #Timer 2
        timer2=QtCore.QTimer(self)
        timer2.timeout.connect(self.refresh_image)
        timer2.start(refresh_time)



        #con.set_buffer_size(rx_size = 15000, tx_size = 0)
        self.plot_init();

    def ChannelSon (self,state):
        if state == QtCore.Qt.Checked:
            self.Ch_state[0] = True
        else:
            self.Ch_state[0] = False

    def ChannelLid (self,state):
        if state == QtCore.Qt.Checked:
            self.Ch_state[1] = True
        else:
            self.Ch_state[1] = False

    def ChannelFus (self,state):
        if state == QtCore.Qt.Checked:
            self.Ch_state[2] = True
        else:
            self.Ch_state[2] = False

    def filter (self,state):
        if state == QtCore.Qt.Checked:
            pass                
        else:
            pass


    def Grid (self,state): #If Grid button is checked shows the Grid. Otherwise, it hides it
        if state == QtCore.Qt.Checked:
            self.set_grid=True                  
        else:
            self.set_grid=False            
        self.plot_init()

    def plot_init(self):
        self.ax = self.canvas.figure.clear()
        self.ax = self.canvas.figure.add_subplot(111, projection='polar')

        if self.set_grid:
            self.ax.grid()
        
        if self.Ch_state[0]:
            self.lines = self.ax.plot(self.con.position_fifo[:] * self.pos_conv,self.con.sonar_fifo[:],self.Ch_colors[0])
            self.Ch_index.append(0)
        if self.Ch_state[1]:
            self.lines = self.ax.plot(self.con.position_fifo[:] * self.pos_conv,self.con.lidar_fifo[:],self.Ch_colors[1])
            self.Ch_index.append(1) 

    def refresh_image(self):
        #print('Refresh')
        self.ax.clear()
        if self.Ch_state[0]:
            self.lines = self.ax.plot(self.con.position_fifo[:] * self.pos_conv,self.con.sonar_fifo[:],self.Ch_colors[0])
        if self.Ch_state[1]:            
            self.lines = self.ax.plot(self.con.position_fifo[:] * self.pos_conv,self.con.lidar_fifo[:],self.Ch_colors[1])
        # self.lines[0].set_ydata(self.con.sonar_fifo[:]) #Plot antiguo
        # self.lines[1].set_ydata(self.con.lidar_fifo[:])
    