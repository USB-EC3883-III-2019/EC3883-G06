/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : FC321.h
**     Project     : ComFun
**     Processor   : MC9S08QE128CLK
**     Component   : FreeCntr32
**     Version     : Component 02.070, Driver 01.22, CPU db: 3.00.067
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2019-11-11, 12:16, # CodeGen: 0
**     Abstract    :
**         This device "FreeCntr32" implements 32-bit Free Running Counter
**     Settings    :
**         Timer name                  : TPM1 (16-bit)
**         Compare name                : TPM11
**         Counter shared              : Yes
**
**         High speed mode
**             Prescaler               : divide-by-16
**             Clock                   : 1048576 Hz
**           Resolution of timer
**             microseconds            : 10
**             seconds (real)          : 0.000009536743
**             Hz                      : 104858
**             kHz                     : 105
**
**         Initialization:
**              Timer                  : Enabled
**
**         Timer registers
**              Counter                : TPM1CNT   [$0041]
**              Mode                   : TPM1SC    [$0040]
**              Run                    : TPM1SC    [$0040]
**              Prescaler              : TPM1SC    [$0040]
**
**         Compare registers
**              Compare                : TPM1C1V   [$0049]
**     Contents    :
**         Enable    - byte FC321_Enable(void);
**         Disable   - byte FC321_Disable(void);
**         Reset     - byte FC321_Reset(void);
**         GetTimeUS - byte FC321_GetTimeUS(word *Time);
**         GetTimeMS - byte FC321_GetTimeMS(word *Time);
**
**     Copyright : 1997 - 2014 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file FC321.h
** @version 01.22
** @brief
**         This device "FreeCntr32" implements 32-bit Free Running Counter
*/         
/*!
**  @addtogroup FC321_module FC321 module documentation
**  @{
*/         

#ifndef __FC321
#define __FC321

/* MODULE FC321. */

/*Include shared modules, which are used for whole project*/
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PE_Timer.h"
#include "Cpu.h"

byte FC321_Enable(void);
/*
** ===================================================================
**     Method      :  FC321_Enable (component FreeCntr32)
*/
/*!
**     @brief
**         This method enables the component - the internal clocks are
**         counted.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
*/
/* ===================================================================*/

byte FC321_Disable(void);
/*
** ===================================================================
**     Method      :  FC321_Disable (component FreeCntr32)
*/
/*!
**     @brief
**         This method disables the component - the internal clocks are
**         not counted.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
*/
/* ===================================================================*/

byte FC321_Reset(void);
/*
** ===================================================================
**     Method      :  FC321_Reset (component FreeCntr32)
*/
/*!
**     @brief
**         This method clears the counter.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
*/
/* ===================================================================*/

byte FC321_GetTimeUS(word *Time);
/*
** ===================================================================
**     Method      :  FC321_GetTimeUS (component FreeCntr32)
**     Description :
**         Returns the time (as a 16-bit unsigned integer) in microseconds
**         since the last resetting after the last reset.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Time            - A pointer to the returned 16-bit value
**                           in microseconds
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_OVERFLOW - Software counter overflow
**                           ERR_MATH - Overflow during evaluation
** ===================================================================
*/

byte FC321_GetTimeMS(word *Time);
/*
** ===================================================================
**     Method      :  FC321_GetTimeMS (component FreeCntr32)
**     Description :
**         Returns the time (as a 16-bit unsigned integer) in milliseconds
**         since the last resetting after the last reset.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Time            - A pointer to the returned 16-bit value
**                           in milliseconds
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_OVERFLOW - Software counter overflow
**                           ERR_MATH - Overflow during evaluation
** ===================================================================
*/

void FC321_Init(void);
/*
** ===================================================================
**     Method      :  FC321_Init (component FreeCntr32)
**
**     Description :
**         Initializes the associated peripheral(s) and the component 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

__interrupt void FC321_Interrupt(void);
/*
** ===================================================================
**     Method      :  FC321_Interrupt (component FreeCntr32)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes the component event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/


/* END FC321. */

#endif /* ifndef __FC321 */
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