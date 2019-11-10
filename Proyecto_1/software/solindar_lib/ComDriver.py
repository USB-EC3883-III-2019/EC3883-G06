import sys
import serial
import numpy as np

#This class allows receptions & decoding data from serial ports
class ComDriver(serial.Serial):

    def __init__(self,Port,len_fifo,n_block):
        super().__init__(port=Port,
                         baudrate=115200,
                         parity=serial.PARITY_NONE,
                         stopbits=serial.STOPBITS_ONE,
                         bytesize=serial.EIGHTBITS,
                         timeout=10)
        #Port init
        self.close()
        self.open()
        self.set_buffer_size(rx_size = 15000, tx_size = 0)

        #Internal variables
        self.currentPosition = 0
        self.currentLidar = 0
        self.currentSonar = 0
        self.filter_on =  np.zeros(len_fifo, dtype=np.uint8) == 0
        self.len_fifo=len_fifo
        self.n=n_block
        self.position_fifo = np.array(range(len_fifo))
        self.sonar_fifo = np.zeros(len_fifo, dtype=np.uint16)
        self.lidar_fifo = np.zeros(len_fifo, dtype=np.uint16)

    def update_fifos(self): #Updates data in fifos
        while True:
            ro=ord(self.read(1))
            if ro < 128:
                break
        r=list(self.read(4*self.n-1))
        data=[[ro,r[0],r[1],r[2]]]
        for i in range(self.n-1):
            data.append(r[3+4*i:3+4*(i+1)])
        #Convert to np
        data=np.array(data,dtype=np.uint16)
        #Check for error
        is_err= ((data[:,0] & 0x80) != 0) | ((data[:,1] & 0x80) == 0) | ((data[:,2] & 0x80) == 0) | ((data[:,3] & 0x80) == 0)
        #Decode
        filter_on = (0x40 & data[:,0]) > 0
        position = 0x3F & data[:,0]
        sonar = ((0x7F & data[:,1]) << 2) | ((data[:,2] & 0x60) >> 5)
        lidar = ((0x1F & data[:,2]) << 7) | (data[:,3] & 0x7F)
        #Mark errors
        position[is_err] = 0
        sonar[is_err] = 0
        lidar[is_err] = 0
        #Update fifos
        indexes=np.searchsorted(self.position_fifo, position)
        bool_new_indexes = indexes == len(self.position_fifo)
        old_indexes = indexes[~bool_new_indexes]
        self.filter_on[old_indexes] = filter_on[~bool_new_indexes]
        self.position_fifo[old_indexes] = position[~bool_new_indexes]
        self.sonar_fifo[old_indexes] = sonar[~bool_new_indexes]
        self.lidar_fifo[old_indexes] = lidar[~bool_new_indexes]
        #Current vals
        self.currentPosition = position[-1]
        self.position = position

    def set_fifo_len(self,len_fifo):  #Change fifo size
        self.len_fifo=len_fifo
        self.position_fifo=np.zeros(len_fifo, dtype=np.float)
        self.sonar_fifo=np.zeros(len_fifo, dtype=np.float)
        self.lidar_fifo=np.zeros(len_fifo, dtype=np.float)
