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
#include "AD1.h"
#include "Cap1.h"
#include "Bits1.h"
#include "Bit1.h"
#include "PWM1.h"
#include "TI1.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
#include "functions.h"


//Externs
unsigned short sonar_measure = 0;
bool sonar_value_done = 0;
bool config_en = 0; //Flag read from PC
bool is_RX_IR=0;  //Flag read from IR
bool tick = 0;

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
  unsigned short steps_per_zone = 16;
  unsigned short adjust_steps = 3;


  //End of motor variables-------------------------------------------------------------
  //Lidar variables--------------------------------------------------------------------
  unsigned short lidar_measure = 0;
  bool lidar_ADC_read_done = 0;
  bool lidar_value_done = 0;
  int lidar_value = 0;

  //End of Lidar variables-------------------------------------------------------------
  //Sonar variables--------------------------------------------------------------------
  unsigned short sonar_UStimer = 0;
  int sonar_value = 0;


  //End of Sonar variables-------------------------------------------------------------
  //Other variables--------------------------------------------------------------------
  int i=0 ,j=0, k=0; //used in for statements
  int distance_value = 0;
  int distance_previous = 0;
  int distance_diff = 0;

  //End of other variables--------------------------------------------------------------------
  //Comunication variables--------------------------------------------------------------------
  //Bytes from PC
   byte packet[4] = {0}; //packet from IR
   byte packet_PC[4] = {0}; //packet from PC
   //Info from PC
   char msg = 0; //msg sent/received from IR
   char msg_PC = 105; //msg received from PC
   bool is_master = 1;
   char zones[5] = {0};
   char zones_PC[5] = {1,5,4,2,6};
   unsigned long data=0;
   word Sent; //data block sent
   byte err;
   bool is_send = 0;
   int n_buffer = 0;
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

   //Other flags
   bool adjust_ok=0;
   bool msg_ok = 0; //Received msg is ok
   bool is_IR_send = 1; //send msg IR
   bool is_slave_end = 0; //Flag if msg is for me




  //End of FSM variables---------------------------------------------------------------------

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */


  while(1){

        if (config_en)
            current_state = ReadConfigPC_state;         
        else if (is_RX_IR)
            current_state = ReceiveIR_state;
        else if (current_state == ReceiveIR_state)
            current_state = SendMsgToPC_state; 
        else
            current_state = IDLE_state;


	  	if(current_state == ReadConfigPC_state){ //Read config from PC
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
			  set_zone = zones_PC[0];

  			packet_PC[0] = packet_PC[0] & 0xEF;
  			packet_PC[1] = packet_PC[1] & 0x8F;

	  	 }

  		if(current_state == SendMsgToPC_state){ //Send msg to PC 

  			for(i=0;i<100;i++){
                for (j = 0; j < 10; j++){
                    tick=0;
                    while(tick==0){}
                    tick=0;
                }
				do
					err=AS1_SendChar(msg);
				while(err!=ERR_OK);
			}            
		}

		if(current_state == SendIR_state){ //Send IR

            for (i = 0; i < 30; i++){
                tick=0;
                while(tick==0){}
                tick=0;
            }

			if (is_master){
				for(i=0;i<4;i++){
					do
						err=AS2_SendChar(packet_PC[i]);
					while(err!=ERR_OK);
				}
			}
			else{
				for(i=0;i<4;i++){
					do
						err=AS2_SendChar(packet[i]);
					while(err!=ERR_OK);
				}
			}
		}

		if(current_state == ReceiveIR_state){ //Receive IR
			//Receive
			i=0;
			do
				n_buffer = AS2_GetCharsInRxBuf();
			while(n_buffer < 4);
			do {
				err = AS2_RecvChar(&packet[0]);
				i++;				
				if(i>4)
					break;
			  } while((err != ERR_OK) && ((packet[0] & 0x80) == 0));
            if((packet[0] & 0x80) != 0){

    			for(i=1;i<4;i++){
    				do
    					err = AS2_RecvChar(&packet[i]);
    				while(err != ERR_OK);
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
                    is_slave_end = 0;
    			}
    			else if (zones[2] != 0){
    				set_zone = zones[2];
    				packet[2] = packet[2] & 0xF8;
                    is_slave_end = 0;
    			}
    			else if (zones[3] != 0){
    				set_zone = zones[3];
    				packet[3] = packet[3] & 0xC7;
                    is_slave_end = 0;
    			}
    			else if (zones[4] != 0){
    				set_zone = zones[4];
    				packet[3] = packet[3] & 0xF8;
                    is_slave_end = 1;
    			}
                //Check msg is ok
                msg_ok = msg == msg_PC;
            }
            else
                msg_ok = 0;
            is_RX_IR = 0;
		}

		if(current_state == SetZone_state){ //Set zone
			max=set_zone-ref_zone;
			dir = 1;
			if(max<0)
				max+=6;
			if(max>3){
				max=6-max;
				dir=0;
			}
			max*= steps_per_zone;			
			for(j=0;j<max;j++){
				if(dir)
					counter++;
				else
					counter--;
				if(counter<0)
					counter=79;
				seq_index= counter%8;
				for(i=0;i<4;i++)
					Bits1_PutBit(i,sequence[seq_index][i]);
                for (i = 0; i < 10; i++){
                    tick=0;
                    while(tick==0){}
                    tick=0;
                }				
			}
			ref_zone = set_zone; //Update ref zone

		}

    if(current_state == AdjustZone_state){ //Adjust zone	   
	  	for(i=0;i<adjust_steps;i++){
		  	if(dir)
			    counter++;
		  	else
			    counter--;
            if(counter<0)
				counter=79;
		  	seq_index= counter%8;
		  	for(j=0;j<4;j++)
				Bits1_PutBit(i,sequence[seq_index][i]);
            tick=0;
            while(tick==0){}
            tick=0;
	    }	
	}

	if(current_state == SensorsCheck_state){ //Sensors check

        //Sonar trigger
        Bit1_PutVal(1);
        tick=0;
        while(tick==0){}
        tick=0;
        Bit1_PutVal(0);

        //Wait 30ms
        for(i = 0; i < 30; i++){
            tick=0;
            while(tick==0){}
            tick=0;
        }

         //Make calculations
        sonar_value = (1047*sonar_measure)/100;
        distance_previous = distance_value;
        distance_value = sonar_value; //Distance in mm
        distance_diff = distance_previous - distance_value;
        if(distance_diff>0)
            dir = !dir;
        if(distance_diff<50 && distance_diff>=0)
            adjust_ok=1;
        else
            adjust_ok=0;
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
