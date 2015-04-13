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
#pragma section     DATA=IO_PRL23,locate=0x003914
#pragma section FAR_DATA=IO_PRL23,locate=0x003914

__VOLATILE __UNION_D(io_prl23, IO_PRL23)	/* addr 3914-3917h */
