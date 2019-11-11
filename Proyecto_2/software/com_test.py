import sys
import serial
import numpy as np
import time
from solindar_lib import ComDriver, process_data, logger

#Parameters
Port = "COM3"
len_fifo=10
n_block=10

con = ComDriver(Port,len_fifo,n_block)
log = logger()
dict_data = {
			 'msg': 0xA8,
			 'is_master':True,
			 'zones': [4,1,6,7,2]
}

packet = con.send_data(dict_data)

