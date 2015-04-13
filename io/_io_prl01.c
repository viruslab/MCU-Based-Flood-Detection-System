/*
 F2MC-16L/16LX/16/16H/16F family
 I/O register macro declaration file V30L12
 ALL RIGHTS RESERVED, COPYRIGHT (C) FUJITSU LIMITED 1998
 LICENSED MATERIAL - PROGRAM PROPERTY OF FUJITSU LIMITED
*/

#include "_ffmc16.h"

/*****************************************************/
/* Sample program for I/O variables of 8/16-bit PPG. */
/*****************************************************/
#pragma section     DATA=IO_PRL01,locate=0x003910
#pragma section FAR_DATA=IO_PRL01,locate=0x003910

__VOLATILE __UNION_D(io_prl01, IO_PRL01)	/* addr 3910-3913h */
