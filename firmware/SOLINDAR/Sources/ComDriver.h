#ifndef COMDRIVER_H_
#define COMDRIVER_H_

void make_packet(unsigned short position,unsigned short sonar,unsigned short lidar, unsigned short filtro_en,unsigned long* data);

#endif /* COMDRIVER_H_ */
