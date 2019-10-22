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

while True:
	con.update_fifos()
	sonar,lidar = process_data(con.sonar_fifo,con.lidar_fifo)
	print('Filter on: ', con.filter_on)
	print('Position: ',con.position_fifo)
	print('Lidar: ', lidar)
	print('Sonar: ', sonar)
	log.info('Filter on: '+  str(con.filter_on))
	log.info('Position: '+ str(con.position_fifo))
	log.info('Lidar: '+  str(lidar))
	log.info('Sonar: '+  str(sonar))
	time.sleep(1)
