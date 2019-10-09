import sys
import serial
import numpy as np

Port = "COM3"
con = serial.Serial( port=Port,
	                 baudrate=115200,
	                 parity=serial.PARITY_NONE,
	                 stopbits=serial.STOPBITS_ONE,
	                 bytesize=serial.EIGHTBITS,
	                 timeout=1)