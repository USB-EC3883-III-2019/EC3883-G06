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
#include "TI1.h"
#include "AD1.h"
#include "Cap1.h"
#include "FC81.h"
#include "Bits1.h"
#include "TI2.h"
#include "Bit1.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
#include "ComDriver.h"
#include "functions.h"

//Externs
bool tick_motor = 0;
bool tick_sensors = 0;

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

  //End of motor variables-------------------------------------------------------------
  //Lidar variables--------------------------------------------------------------------  
  unsigned short lidar_measure = 0; 
  unsigned short lidar_value = 0;
  
  //End of Lidar variables-------------------------------------------------------------
  //Sonar variables--------------------------------------------------------------------  
  unsigned short sonar_measure = 0;     
  unsigned short sonar_value = 0;
  unsigned short sonar_UStimer = 0;
  
  
  //End of Sonar variables-------------------------------------------------------------  
  //Other variables--------------------------------------------------------------------
  int i = 0; //used in for statements
  unsigned short max_measures = 10; //number of measures to take at each point
  unsigned short counter_measures = 0;  //counter of measures done at a particular point
   
  
  
  //End of other variables--------------------------------------------------------------------
  //Comunication variables--------------------------------------------------------------------
   unsigned long data=0;
   word Sent; //data block sent
   byte err; 
   bool is_send = 0;
  //End of Comunication variables-------------------------------------------------------------

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  while(1){
        if(tick_motor){
          tick_motor=0;       
          //Motor routine
          seq_index=motor_run(&dir,&counter,&max); //Get motor index sequence
          for(i=0;i<4;i++)
            Bits1_PutBit(0,sequence[seq_index][i]);

          //For testing comunications
          is_send=1;
    } 
    
    if(tick_sensors){
      tick_sensors=0; 
      
      if(counter_measures<max_measures)
        counter_measures++;
      else
        counter_measures = 0;
      
      //Lidar
      AD1_MeasureChan(0,0);           
      
      //Sonar
      
      //trigger
      Bit1_PutVal(1);
      FC81_Reset();
      while(sonar_UStimer<15){
      do
        err=FC81_GetTimeUS(&sonar_UStimer);
      while(err!=ERR_OK);
      }
      sonar_UStimer=0;
      Bit1_PutVal(0);     
      //Capture   
      Cap1_Reset();   
      
    }   
    //Comunication      
  
    if(is_send){
      is_send=0;
      //Make packet
      make_packet(counter,sonar_value,lidar_value,&data);
      //Send data
      do
        err=AS1_SendBlock((byte*) &data,sizeof(data),&Sent);
      while(err!=ERR_OK);
    }

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
