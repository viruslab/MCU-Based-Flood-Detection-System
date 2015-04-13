/*
 F2MC-16L/16LX/16/16H/16F family
 I/O register macro declaration file V30L12
 ALL RIGHTS RESERVED, COPYRIGHT (C) FUJITSU LIMITED 1998
 LICENSED MATERIAL - PROGRAM PROPERTY OF FUJITSU LIMITED
*/

#include "_ffmc16.h"

/************************************************************/
/* Sample program for I/O variables of 16-bit reload timer. */
/************************************************************/
#pragma section     DATA=IO_TMR,locate=0x003900
#pragma section FAR_DATA=IO_TMR,locate=0x003900

#pragma section DIRDATA=ASD
__direct int a;

__VOLATILE __USHORT_AD(IO_TMR, 2)		/* addr 3900-3903h */
