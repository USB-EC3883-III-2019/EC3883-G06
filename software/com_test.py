import sys
import serial
import numpy as np
import time
from solindar_lib import ComDriver

#Parameters
Port = "COM3"
len_fifo=10
scale_factor=1
n_block=5

con = ComDriver(Port,len_fifo,scale_factor,n_block)

while True:
	con.update_fifos()	
	print('Position: ',con.position_fifo[0])
	print('Lidar: ',con.lidar_fifo[0])
	print('Sonar: ',con.sonar_fifo[0])
	time.sleep(2)
