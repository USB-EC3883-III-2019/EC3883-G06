import numpy as np

def process_data(sonar,lidar): #Return array of distance in cm

	#Lidar curve distance=(a/value)+b
	a=55632
	b=-12.37
	try:
		p_lidar=(a/lidar)+b
	except:
		p_lidar=np.zeros(len(lidar), dtype=np.uint8) 


	#Sonar curve
	c=1.0468
	p_sonar=c*sonar

	return p_sonar,p_lidar
