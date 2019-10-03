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
        self.data_read=[0] * 4
        self.len_fifo=len_fifo
        self.data_fifo=np.zeros([len_fifo,4], dtype=np.float)
        self.scale_factor=scale_factor
        self.n=n_block

    def get(self): #Reads four byte convetions & stores it in self.data_read
        while True:
            r=ord(self.read(1))
            if r < 128:
                break
        self.data_read=[r]
        r=  self.read(3)
        for i in r:
            self.data_read.append(i)

    def get_block(self): #Reads four byte convetions & stores it in self.data_read
        while True:
            r=ord(self.read(1))
            if r < 128:
                break
        self.data_read=[r]
        r=self.read(4*self.n-1)
        for i in r:
            self.data_read.append(i)

    def decode(self,data): #Returns array with four decoded channels from data in self.data_read
        #Check for error
        is_err= (data[0] & 0xC0) != 0 or (data[1] & 0x80) == 0 or (data[2] & 0x80) == 0 or (data[3] & 0x80) == 0
        if(is_err):
            return 0, 0, 100, 100 #identify error
        else:
            #analog channels
            analogCh1= ((0x3F & data[0]) << 6) | (data[1] & 0x3F)
            analogCh2= ((0x3F & data[2]) << 6) | (data[3] & 0x3F)

            #digital channels
            digitalCh1, digitalCh2 = 0, 0
            if (data[2] & 0x40) != 0:
                digitalCh1=1
            if (data[3] & 0x40) != 0:
                digitalCh2=1


            analogCh1, analogCh2 = self.angles(self.scale_factor*analogCh1, self.scale_factor*analogCh2 )

            return [analogCh1, analogCh2, digitalCh1, digitalCh2]

    def update_all(self): #Updata self.data_read, decode such data and stores it in self.data_fifo
        self.get()
        self.data_fifo=np.vstack((self.decode(self.data_read),self.data_fifo[:-1]))

    def update_all_block(self):
        self.get_block()
        for i in range(self.n):
            r=self.decode(self.data_read[4*i:4*i+4])
            self.data_fifo=np.vstack((r,self.data_fifo[:-1]))

    def set_fifo_len(self,len_fifo):  #Change fifo size
        self.len_fifo=len_fifo
        self.data_fifo=np.zeros([len_fifo,4], dtype=np.float)
