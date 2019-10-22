import numpy as np

def process_data(sonar,lidar): #Return array of distance in cm

	#Lidar curve distance=(a/value)+b
	a=55632
	b=-12.37
	p_lidar=(a/lidar)+b

	#Sonar curve
	c=1.0468
	p_sonar=c*sonar

	return p_sonar,p_lidar
