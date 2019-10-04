#include "ComDriver.h"

void make_packet(unsigned short position,unsigned short sonar,unsigned short lidar,unsigned long* data){

	*data=  0x00808080;
	*data|= (0x003F & (unsigned long) position) << 24;
	*data|= (0x007F & (unsigned long) sonar) << 2;
  *data|= (0x0FC0 & (unsigned long) lidar) << 2;

}
