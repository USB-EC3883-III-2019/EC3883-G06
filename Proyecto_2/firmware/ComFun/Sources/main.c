/* ###################################################################
**     Filename    : main.c
**     Project     : SOLINDAR
**     Processor   : MC9S08QE128CLK
**     Version     : Driver 01.12
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2019-10-09, 18:06, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.12
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "AS1.h"
#include "AS2.h"
#include "TI1.h"
#include "AD1.h"
#include "Cap1.h"
#include "Bits1.h"
#include "TI2.h"
#include "Bit1.h"
#include "FC321.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
#include "functions.h"


//Externs
bool tick_motor = 0;
bool tick_sensors = 0;
unsigned short sonar_measure = 0;
bool sonar_value_done = 0;
bool config_en = 0; //Flag read from PC

void main(void)
{
  /* Write your local variable definition here */

  //Motor variables-------------------------------------------------------------	
  bool dir = 1;
  int counter = 0; //motor position as well
  int max = 63;
  int seq_index = 0;
  bool sequence[8][4] = { {1,0,1,0},
                          {1,0,0,0},
                          {1,0,0,1},
                          {0,0,0,1},
                          {0,1,0,1},
                          {0,1,0,0},
                          {0,1,1,0},
                          {0,0,1,0}};
  bool is_step_done = 0;
  unsigned short ref_zone = 1; //Initial zone for motor reference
  unsigned short set_zone = 2; // Desired zone for motor to move
  unsigned short steps_per_zone = 10;
  unsigned short adjust_steps = 0;
  

  //End of motor variables-------------------------------------------------------------
  //Lidar variables--------------------------------------------------------------------  
  unsigned short lidar_measure = 0; 
  bool lidar_ADC_read_done = 0;
  bool lidar_value_done = 0;
  
  //End of Lidar variables-------------------------------------------------------------
  //Sonar variables--------------------------------------------------------------------  
  unsigned short sonar_UStimer = 0;
  
  
  //End of Sonar variables-------------------------------------------------------------  
  //Other variables--------------------------------------------------------------------
  int i=0 ,j=0; //used in for statements
  int filter_order=10; 
  bool filter_en = 0;
  
  
  //End of other variables--------------------------------------------------------------------
  //Comunication variables--------------------------------------------------------------------
  //Bytes from PC
   byte packet[4] = {0}; //packet from IR
   byte packet_PC[4] = {0}; //packet from PC
   //Info from PC
   char msg = 0; //msg sent/received from IR   
   char msg_PC = 0; //msg received from PC   
   bool is_master = 0;   
   char zones[5] = {0}; 
   char zones_PC[5] = {0};
   unsigned long data=0;
   word Sent; //data block sent
   byte err; 
   bool is_send = 0;
  //End of Comunication variables-------------------------------------------------------------
  //FSM variables
   const unsigned short IDLE_state = 0;
   const unsigned short ReadConfigPC_state = 1;
   const unsigned short SetZone_state = 2;
   const unsigned short AdjustZone_state = 3;
   const unsigned short SensorsCheck_state = 4;
   const unsigned short SendIR_state = 5;
   const unsigned short ReceiveIR_state = 6;
   const unsigned short SendMsgToPC_state = 7;

   unsigned short current_state=0;

  //End of FSM variables---------------------------------------------------------------------

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  while(1){  
	  
	  	if(0){ //Read config from PC
			do {                                                   
				err = AS1_RecvChar(&packet_PC[0]);                             
			  } while((err != ERR_OK) && ((packet_PC[0] & 0x80) == 0));    	
			for(i=1;i<4;i++){
				do {                                                   
					err = AS1_RecvChar(&packet_PC[i]);                             
				  } while(err != ERR_OK);
			}
			//Decode
			msg_PC = ((packet_PC[0] & 0x0F) << 4) | (packet_PC[1] & 0x0F);
			is_master =  (packet_PC[0] & 0x10) > 0;
			zones_PC[0] = (packet_PC[1] & 0x70) >> 4;
			zones_PC[1] = (packet_PC[2] & 0x38) >> 3;
			zones_PC[2] = (packet_PC[2] & 0x07);
			zones_PC[3] = (packet_PC[3] & 0x38) >> 3;
			zones_PC[4] = (packet_PC[3] & 0x07);
			
			config_en = 0;

			packet_PC[0] = packet_PC[0] & 0xEF;
			packet_PC[1] = packet_PC[1] & 0x8F;
	  	 }

  		if(0){ //Send msg to PC
  			 for(i=0;i<100;i++){
				 do
					 err=AS1_SendChar(msg);
				 while(err!=ERR_OK);
			} 
		}

		if(0){ //Send IR
			if (is_master == 1)	{		
				for(i=0;i<4;i++){
					do{
						err=AS2_SendChar(packet_PC[i]);
					}
					while(err!=ERR_OK);
				}
			} 
			if (is_master == 0)	{	
				for(i=0;i<4;i++){
					do{
						err=AS2_SendChar(packet[i]);
					}
					while(err!=ERR_OK);
				} 
			}		 
		}				

		if(0){ //Receive IR
			//Receive
			do {                                                   
				err = AS2_RecvChar(&packet[0]);                             
			  } while((err != ERR_OK) && ((packet[0] & 0x80) == 0));    	
			for(i=1;i<4;i++){
				do {                                                   
					err = AS2_RecvChar(&packet[i]);                             
				  } while(err != ERR_OK);
			}
			//Decode
			msg = ((packet[0] & 0x0F) << 4) | (packet[1] & 0x0F);
			is_master =  (packet[0] & 0x10) > 0;
			zones[1] = (packet[2] & 0x38) >> 3;
			zones[2] = (packet[2] & 0x07);
			zones[3] = (packet[3] & 0x38) >> 3;
			zones[4] = (packet[3] & 0x07);

			if (zones[1] != 0){
				set_zone = zones[1];
				packet[2] = packet[2] & 0xC7;
			}
			else if (zones[2] != 0){
				set_zone = zones[2];
				packet[2] = packet[2] & 0xF8;
			}
			else if (zones[3] != 0){
				set_zone = zones[3];
				packet[3] = packet[3] & 0xC7;
			}
			else if (zones[4] != 0){
				set_zone = zones[4];
				packet[3] = packet[3] & 0xF8;
			}
		}
	 
		  
		if(0){ //Set zone
			max=set_zone-ref_zone;
			dir = 1;
			counter=0;
			if(max<0)
				max+=8;
			if(max>4){
				max=8-max;
				dir=0;
				counter=max;
			}
			max*= steps_per_zone;
				
			do{
				if(dir)
					counter++;
				else
					counter--;
				seq_index= counter%8;
				for(i=0;i<4;i++)
					Bits1_PutBit(i,sequence[seq_index][i]);
			 //Put some delays...100ms
				//..
			} while(counter>0 && counter<max);
			
			ref_zone = set_zone; //Update ref zone			
			
		}
		  
        if(0){ //Adjust zone
			  
        	counter = 0;
			  //Motor routine   
        	  for(i=0;i<adjust_steps;i++){
				  if(dir)
					counter++;
				  else
					counter--;				
				  seq_index= counter%8;
				  for(j=0;j<4;j++)
					Bits1_PutBit(i,sequence[seq_index][i]);	
        	  }				  
				
		}      
        
		if(0){ //Sensors check
		  tick_sensors=0;
		  
		  //Sonar trigger 
		  Bit1_PutVal(1);
		  FC321_Enable();
		  FC321_Reset();
		  while(sonar_UStimer<15){
		  do
			err=FC321_GetTimeUS(&sonar_UStimer);
		  while(err!=ERR_OK);
		  }
		  FC321_Disable();
		  sonar_UStimer=0;
		  Bit1_PutVal(0); 
		  
		  //Lidar      
		  AD1_MeasureChan(1,0); 
		  AD1_GetChanValue(0,&lidar_measure);		  
	 
		}

		//Define next state


  }
  /* For example: for(;;) { } */

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
