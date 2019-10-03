import sys
import serial
import numpy as np

#This class allows receptions & decoding data from serial ports
class ComDriver(serial.Serial):

    def __init__(self,Port,len_fifo,scale_factor,n_block):
        super().__init__(self,
                         port=Port,
                         baudrate=115200,
                         parity=serial.PARITY_NONE,
                         stopbits=serial.STOPBITS_ONE,
                         bytesize=serial.EIGHTBITS,
                         timeout=1)
        #Port init
        self.close()
        self.open()
        self.set_buffer_size(rx_size = 15000, tx_size = 0)

        #Internal variables
        self.len_fifo=len_fifo
        self.n=n_block
        self.position_fifo = np.zeros(len_fifo, dtype=np.float)
        self.sonar_fifo = np.zeros(len_fifo, dtype=np.float)
        self.lidar_fifo = np.zeros(len_fifo, dtype=np.float)

    def update_fifos(self): #Updates data in fifos
        while True:
            ro=ord(self.read(1))
            if ro < 128:
                break
        r=self.read(4*self.n-1)
        data=[[ro,*r[:3]]]
        for i in range(self.n):
            data.append(r[3+4*i:3+4*(i+1)])
        #Convert to np
        data=np.array(data, dtype=np.int16)
        #Check for error
        is_err= ((data[:,0] & 0xC0) != 0) | ((data[:,1] & 0x80) == 0) | ((data[:,2] & 0x80) == 0) | ((data[:,3] & 0x80) == 0)
        #Decode
        position = (0x3F & data[:,0]) << 6
        sonar = ((0x7F & data[:,0]) << 1) | ((data[:,1] & 0x20) >> 6)
        lidar = ((0x1F & data[:,2]) << 6) | (data[:,3] & 0x7F)
        #Mark errors
        position[is_err] = 0
        sonar[is_err] = 0
        lidar[is_err] = 0
        #Update fifos
        self.position_fifo=np.vstack((position,self.position_fifo[:-self.n]))
        self.sonar_fifo=np.vstack((position,self.position_fifo[:-self.n]))
        self.lidar_fifo=np.vstack((position,self.position_fifo[:-self.n]))

    def set_fifo_len(self,len_fifo):  #Change fifo size
        self.len_fifo=len_fifo
        self.position_fifo=np.zeros(len_fifo, dtype=np.float)
        self.sonar_fifo=np.zeros(len_fifo, dtype=np.float)
        self.lidar_fifo=np.zeros(len_fifo, dtype=np.float)
