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
        self.rx_msg = 0        


    def send_data(self,dict_data): #Sends data, returns True if correctly send, False otherwise
        #Extract from dict
        msg = dict_data['msg']
        is_master = dict_data['is_master']
        zones = dict_data['zones']
        #Encode
        packet = 0x80000000
        packet |= (msg & 0xF0) << 20
        packet |= (msg & 0x0F) << 16
        shift = [20,11,8,3,0]
        for i in range(len(shift)):            
            packet |= (zones[i] & 0x7) << shift[i]
        if is_master:
            packet |= 0x10000000            
        #send
        packet = packet.to_bytes(4,'little')
        is_sent = self.write(packet)

        if is_sent > 0:
            return True
        else:
            return False

    def get_msg(self):
        return ord(self.read())
  