;**********************************************************
;
; The following software is for demonstration purposes only. 
; It is not fully tested, nor validated in order to fullfill  
; its task under all circumstances. Therefore, this software  
; or any part of it must only be used in an evaluation        
; laboratory environment.                                    
; This software is subject to the rules of Accemic's 
; standard DISCLAIMER, available at www.accemic.com.
;
; Copyright (c) 2001-2003 Accemic GmbH & Co. KG 
; All rights reserved  
;
;**********************************************************
;
; Module name        : monitor16lx.asm
; Version            : 5.01      
; Date of creation.  : 07.03.2003
; Last Modification  : 03.05.2004
; Authors            : Rainer Harthaus (RH)
; 
;**********************************************************
; 
; Description: 
; Reservation of memory for monitor
; and dummy declaration for application functions
;
;**********************************************************
;
; History
;
; Vers   * Datum    * Name  * Subject
; 5.00   * 07.03.03 * RH    * first creation
; 5.01   * 03.05.04 * AL    * MB90895 monitor section added 
;                             functions offsets changed  
; 
;**********************************************************
;
; Please visit www.accemic.com
; for updates and other valuable developer ressources
;
;**********************************************************/

  .PROGRAM  MONITORFUNC
  .TITLE    "MONITOR RESERVATION AND FUNCTIONS"
    
;**********************************************************
; Version information
;**********************************************************/
  ACC_FILE_VERSION: .EQU 0x0007
  	
;**********************************************************
; monitor data area reservation 
; 0x000880 .. 0x0008FF
;**********************************************************/
  .SECTION ACC_MDATA,DATA,LOCATE=0x000880
  .RES.B 0x80
	
;**********************************************************
; monitor target information area reservation 
; 0xFFFB00 .. 0xFFFBFF
;**********************************************************/
  .SECTION ACC_UDATA,CONST,LOCATE=0xFFFB00
  .RES.B 0x100
	
;**********************************************************
; monitor kernel area reservation 
;**********************************************************/

#include"cpu_16lx.h"

#ifdef __CPU_MB90895_SERIES 
   .SECTION ACC_MCODE,CONST,LOCATE=0x00FFD000
#else 
   .SECTION ACC_MCODE,CONST,LOCATE=0x00FFA000
#endif  

    .EXPORT _acc_WatchdogEnable
    .EXPORT _acc_MessageBox
    .EXPORT _acc_printf
    .EXPORT _acc_KernelUART
    .EXPORT _acc_SetLockState

    .RES.B 0x10

    _acc_WatchdogEnable: .RES.L 1
    _acc_KernelUART:     .RES.L 1
    _acc_MessageBox:     .RES.L 1
    _acc_printf:         .RES.L 1
    _acc_SetLockState:   .RES.L 1
	  
    .RES.B 0x2000-0x10-(5*4)

  .END

