;==========================================================================
; FFMC-16 Family Softune C Compiler's startup routine,
; ALL RIGHTS RESERVED, COPYRIGHT (C) FUJITSU LIMITED 1986
; LICENSED MATERIAL - PROGRAM PROPERTY OF FUJITSU LIMITED
;==========================================================================
;==========================================================================
; Sample program for initialization (small model)
;--------------------------------------------------------------------------

		.PROGRAM	start
		.TITLE		start

;--------------------------------------------------------------------------
; external declaration of symbols
;--------------------------------------------------------------------------
		.EXPORT		__start
		.IMPORT		_main

;--------------------------------------------------------------------------
; definition to stack area
;--------------------------------------------------------------------------
		.SECTION	STACK,	  STACK,    ALIGN=1
		.RES.B		254
SSTACK_TOP:
		.RES.B		2
		.RES.B		254
USTACK_TOP:
		.RES.B		2

;--------------------------------------------------------------------------
; definition to start address of data, const and code section
;--------------------------------------------------------------------------
		.SECTION	DIRDATA,  DIR,	    ALIGN=2
DIRDATA_S:
		.SECTION	DIRINIT,  DIR,	    ALIGN=2
DIRINIT_S:
		.SECTION	DATA,	  DATA,	    ALIGN=2
DATA_S:
		.SECTION	INIT,	  DATA,	    ALIGN=2
INIT_S:
		.SECTION	LIBDATA,  DATA,	    ALIGN=2
LIBDATA_S:
		.SECTION	LIBINIT,  DATA,	    ALIGN=2
LIBINIT_S:
		.SECTION	DIRCONST, DIRCONST, ALIGN=2
DIRCONST_S:
		.SECTION	DCONST,	  CONST,    ALIGN=2
DCONST_S:
		.SECTION	CONST,	  CONST,    ALIGN=2
CONST_S:
		.SECTION	LIBDCONST,CONST,    ALIGN=2
LIBDCONST_S:
		.SECTION	DTRANS,	  CONST,    ALIGN=2
DTRANS_S:
		.SECTION	DCLEAR,	  CONST,    ALIGN=2
DCLEAR_S:


;--------------------------------------------------------------------------
; code area
;--------------------------------------------------------------------------
		.SECTION	CODE,	  CODE,	    ALIGN=1
__start:
;--------------------------------------------------------------------------
; set register bank is 0
;--------------------------------------------------------------------------
		MOV	RP, #0

;--------------------------------------------------------------------------
; set ILM to the lowest level
;--------------------------------------------------------------------------
		MOV	ILM, #7

;--------------------------------------------------------------------------
; set direct page register
;--------------------------------------------------------------------------
		MOV	A, #PAGE DIRDATA_S
		MOV	DPR, A

;--------------------------------------------------------------------------
; set system stack
;--------------------------------------------------------------------------
		AND	CCR, #0x20
		MOV	A, #BNKSYM SSTACK_TOP
		MOV	SSB, A
		MOVW	A, #SSTACK_TOP
		MOVW	SP, A
		AND	CCR, #0x00DF

;--------------------------------------------------------------------------
; copy initial value *CONST section to *INIT section
;--------------------------------------------------------------------------
#macro		ICOPY	src_addr, dest_addr, src_segment
		MOV	A, #BNKSYM \src_addr
		MOV	ADB, A
		MOV	A, #BNKSYM \dest_addr
		MOV	DTB, A
		MOVW	RW0, #SIZEOF (\src_segment)
		MOVW	A, #\dest_addr
		MOVW	A, #\src_addr
		MOVSI	DTB, ADB
#endm

		ICOPY	DIRCONST_S, DIRINIT_S, DIRCONST
		ICOPY	DCONST_S,   INIT_S,    DCONST
		ICOPY	LIBDCONST_S,LIBINIT_S, LIBDCONST

;--------------------------------------------------------------------------
; zero clear of *VAR section
;--------------------------------------------------------------------------
#macro		FILL0	src_addr, src_segment
		MOV	A, #BNKSYM \src_addr
		MOV	DTB, A
		MOVW	RW0, #SIZEOF (\src_segment)
		MOVW	A, #\src_addr
		MOVN	A, #0
		FILSI	DTB
#endm

		FILL0	DIRDATA_S, DIRDATA
		FILL0	DATA_S,	   DATA
		FILL0	LIBDATA_S, LIBDATA

;--------------------------------------------------------------------------
; copy initial value DCONST_module section to INIT_module section
;--------------------------------------------------------------------------
		MOV	A, #BNKSYM DTRANS_S
		MOV	DTB, A
		MOVW	RW1, #DTRANS_S
		BRA	LABEL2
LABEL1:
		MOVW	A, @RW1+6
		MOV	USB, A
		MOVW	A, @RW1+2
		MOV	ADB, A
		MOVW	A, @RW1+4
		MOVW	A, @RW1
		MOVW	RW0, @RW1+8
		MOVSI	SPB, ADB
		MOVN	A, #10
		ADDW	RW1, A
LABEL2:
		MOVW	A, RW1
		SUBW	A, #DTRANS_S
		CMPW	A, #SIZEOF (DTRANS)
		BNE	LABEL1

;--------------------------------------------------------------------------
; zero clear of DATA_module section
;--------------------------------------------------------------------------
		MOV	A, #BNKSYM DCLEAR_S
		MOV	DTB, A
		MOVW	RW1, #DCLEAR_S
		BRA	LABEL4
LABEL3:
		MOV	A, @RW1+2
		MOV	ADB, A
		MOVW	RW0, @RW1+4
		MOVW	A, @RW1
		MOVN	A, #0
		FILSI	ADB
		MOVN	A, #6
		ADDW	RW1, A
LABEL4:
		MOVW	A, RW1
		SUBW	A, #DCLEAR_S
		CMPW	A, #SIZEOF (DCLEAR)
		BNE	LABEL3

;--------------------------------------------------------------------------
; set data bank register
;--------------------------------------------------------------------------
		MOV	A, #BNKSYM DATA_S
		MOV	DTB, A

;--------------------------------------------------------------------------
; set user stack
;--------------------------------------------------------------------------
		MOV	A, #BNKSYM USTACK_TOP
		MOV	USB, A
		MOVW	A, #USTACK_TOP
		MOVW	SP, A

;--------------------------------------------------------------------------
; call main routine
;--------------------------------------------------------------------------
		CALL	_main
end:		jmp	end
		.END	__start
