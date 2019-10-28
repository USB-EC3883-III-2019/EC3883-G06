import matplotlib
matplotlib.use('Qt4Agg')
from matplotlib.figure import Figure
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT as NavigationToolbar
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
import matplotlib.pyplot as plt
from PyQt4 import QtCore, QtGui, uic
import sys
from ..TestCom import testCom
from ..ComDriver import ComDriver
from .canvas import Canvas
import sys
import serial
import numpy as np
from ..process_data import process_data
from ..logger import logger
from ..process_fusion import process_fusion

qtCreatorFile = "solindar_lib/GUI/graph8.ui" # my Qt Designer file

Ui_MainWindow, QtBaseClass = uic.loadUiType(qtCreatorFile)

class SolindarGUI(QtGui.QMainWindow, Ui_MainWindow):
    def __init__(self,con,ts):
        QtGui.QMainWindow.__init__(self)
        Ui_MainWindow.__init__(self)
        self.setupUi(self)
        self.navigation = NavigationToolbar(self.canvas, self)
        self.hlayout.addWidget(self.navigation)
        self.sonBox.stateChanged.connect(self.ChannelSon)
        self.sonBox.setStatusTip("Activar la Grafica del Sonar")
        self.lidBox.stateChanged.connect(self.ChannelLid)
        self.lidBox.setStatusTip("Activar la Grafica del Lidar")
        self.fusBox.stateChanged.connect(self.ChannelFus)
        self.fusBox.setStatusTip("Activar la Grafica del Fusion")
        self.con=con
        self.Ch_state=[False, False, False]
        self.Ch_index=[]
        self.Ch_colors=['bo','go','yo','ro','co','mo','wo']
        self.pos_conv = 2 * np.pi / 84
        self.ax = self.canvas.figure.add_subplot(111, projection = 'polar')
        self.axesCom.activated[str].connect(self.changeDistanceScale)
        self.axesCom.setStatusTip("Cambiar la escala de distancia")
        self.axeslim = 80
        #self.ax.set_xticklabels([]) #deletes labels from X-axes
        self.lines = self.ax.plot()
        self.set_grid=False
        #Fifos
        self.fusion_fifo = np.zeros((self.con.len_fifo))
        self.sonarproc = np.zeros((self.con.len_fifo))
        self.lidarproc = np.zeros((self.con.len_fifo))
        #Logger
        self.log = logger()
        #Plot
        plt.ion()

        #Timer
        timer2=QtCore.QTimer(self)
        timer2.timeout.connect(self.run)
        timer2.start(ts)

        #Start graph
        self.ax = self.canvas.figure.clear()
        self.ax = self.canvas.figure.add_subplot(111, projection='polar')
        self.make_graph()

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

    def filter (self):
        if int(self.con.filter_on[0]):
            self.filPro.setValue(100)
        else:
            self.filPro.setValue(0)

    def PosicionLcd(self):
        self.posLcd.display(int(((-1)*self.con.currentPosition*self.pos_conv*(180/np.pi))))

    def SonarLcd(self):
        self.sonLcd.display(int(self.sonarproc[self.con.currentPosition]))

    def LidarLcd(self):
        self.lidLcd.display(int(self.lidarproc[self.con.currentPosition]))

    def FusionLcd(self):
        self.fusLcd.display(int(self.fusion_fifo[self.con.currentPosition]))

    def logFun(self):
        for i in range(self.con.n):
            if self.con.currentPosition+i >= self.con.len_fifo:
                n= 2*self.con.len_fifo - self.con.currentPosition - i -1
            else:
                n= self.con.currentPosition+i
            self.log.info(str(self.con.filter_on[n]) + ',' + \
            str((-1)*self.con.position_fifo[n] * self.pos_conv*(180/np.pi)) + ','  +\
            str(self.lidarproc[n]) + ',' + \
            str(self.sonarproc[n]) + ',')

    def make_graph(self):
        self.sonarproc,self.lidarproc = process_data(self.con.sonar_fifo,self.con.lidar_fifo)
        self.fusion_fifo = process_fusion(self.sonarproc,self.lidarproc)
        currentPosition = (-1)*self.con.position_fifo * self.pos_conv

        self.lines = self.ax.plot([(-1)*self.con.currentPosition*self.pos_conv,(-1)*self.con.currentPosition*self.pos_conv],[0,300],'b',alpha=0.35)
        if self.Ch_state[0]:
            self.lines = self.ax.plot(currentPosition[:],self.sonarproc[:],self.Ch_colors[0])
            self.lines = self.ax.plot(currentPosition[self.con.currentPosition],self.sonarproc[self.con.currentPosition],self.Ch_colors[4])
        if self.Ch_state[1]:
            self.lines = self.ax.plot(currentPosition[:],self.lidarproc[:],self.Ch_colors[3])
            self.lines = self.ax.plot(currentPosition[self.con.currentPosition],self.lidarproc[self.con.currentPosition],self.Ch_colors[5])
        if self.Ch_state[2]:
            self.lines = self.ax.plot(currentPosition[:], self.fusion_fifo[:],self.Ch_colors[2])
            self.lines = self.ax.plot(currentPosition[self.con.currentPosition], self.fusion_fifo[self.con.currentPosition],self.Ch_colors[1])
        self.ax.set_rlim(bottom=0,top=self.axeslim)

    #Repeat all over again
    def run(self):
        self.con.update_fifos() #Update fifo
        self.ax.clear()
        self.make_graph() #Graph
        self.filter() #Check filter flag
        self.PosicionLcd() #Print motor position
        self.logFun()  #Log
        self.SonarLcd()
        self.LidarLcd()
        self.FusionLcd()

    def changeDistanceScale(self,text):
        if text == "MaxDistance = 80cm":
            print("MaxDistance = 80cm") #Colocar cambio de escala
            self.ax.set_rlim(bottom=0,top=80)
            self.axeslim = 80
        else:
            print("MaxDistance = 300cm") #Colocar cambio de escala
            self.ax.set_rlim(bottom=0,top=300)
            self.axeslim = 300
