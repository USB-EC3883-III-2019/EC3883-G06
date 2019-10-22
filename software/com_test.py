import sys
import serial
import numpy as np
import time
from solindar_lib import ComDriver, process_data

#Parameters
Port = "COM3"
len_fifo=10
scale_factor=1
n_block=10

con = ComDriver(Port,len_fifo,scale_factor,n_block)

while True:
	con.update_fifos()
	sonar,lidar = process_data(con.sonar_fifo,con.lidar_fifo)	
	print('Position: ',con.position_fifo)
	print('Lidar: ', lidar)
	print('Sonar: ', sonar)
	time.sleep(1)