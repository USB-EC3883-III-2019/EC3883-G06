#include "ComDriver.h"

void make_packet(unsigned short position,unsigned short sonar,unsigned short lidar, unsigned short filtro_en,unsigned long* data){

	*data=  0x00808080;
	*data|= (0x007F & (unsigned long) lidar);
	*data|= (0x0F80 & (unsigned long) lidar) << 1;
	*data|= (0x0003 & (unsigned long) sonar) << 13;
	*data|= (0x01FC & (unsigned long) sonar) << 14;
	*data|= (0x003F & (unsigned long) position) << 24;
	if(filtro_en)
		*data|= 0x40000000;

}
