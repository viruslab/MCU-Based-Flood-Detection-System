#include "_ffmc16.h"
#include "extern.h"
#include "monitor.h"

/*Definitions*/
#define LED		IO_PDR1.byte				/* OUTPUT LED PDR 								*/
#define INPUT	IO_DDR5.bit.D54				/* ADC CHANNEL DDR IO_DDR5.bit.D54 				*/
											/* Set to IO_DDR5.bit.D50 for lazy testing 		*/
#define BUZZER	IO_DDR1.bit.D17				/* OUTPUT BUZZER DDR 							*/

/*RIOS Structure*/
typedef struct task{
	unsigned long period;
	unsigned long elapsedTime;
	void (*TickFct)(void);
}task;

/* Function Prototypes */
void get_WaterLvl(void);
void output_LED(void);
void sound_BUZ(void);
void TimerOFF();
void timer_config(int milliseconds);
void io_port_config(void);
void ADC_config(void);
void ppgtimer_config(int selectorH, int selectorL);

/* GLobal Assignments */
task tasks[3];
int 		  AD_DATA			= 0,
			  WATER_LVL_DATA	= 0;
unsigned char tasksNum			= 3;
unsigned long tasksPeriodGCD	= 10;
unsigned long periodWaterLvl	= 100;
unsigned long periodLED			= 20;
unsigned long periodBUZ			= 5000;
unsigned char TimerFlag			= 0;
unsigned char i					= 0;

void main(void)
{
	unsigned char buff;
    __set_il(7);
	__EI();		/* enable interrupt(for Accemic) */

    /*Initialization*/
    io_port_config();
    ADC_config();
    ppgtimer_config(0,1000);
	timer_config(tasksPeriodGCD);

	/*Task 1 initialization*/
	tasks[0].period = periodWaterLvl;
	tasks[0].elapsedTime = tasks[0].period;
	tasks[0].TickFct = &output_LED;			/* Task 2										*/
	
	/*Task 2 initialization*/
	tasks[1].period = periodLED;
	tasks[1].elapsedTime = tasks[1].period;
	tasks[1].TickFct = &sound_BUZ;			/* Task 3										*/
	
	/*Task 3 initialization*/
	tasks[2].period = periodBUZ;
	tasks[2].elapsedTime = tasks[2].period;
	tasks[2].TickFct = &get_WaterLvl;		/* Task 1										*/
	
	i = 0;
	/* RIOS Scheduler */
	while(1)
	{
		if(TimerFlag == 1)
		{
			if (tasks[i].elapsedTime >= tasks[i].period) {
				tasks[i].TickFct();
				tasks[i].elapsedTime = 0;
				++i;
				if(i == tasksNum)
				{
					i = 0;
				} 
			}
			else
				tasks[i].elapsedTime += tasksPeriodGCD;
			TimerFlag = 0;
		}
		while(!TimerFlag); 					/* Pseudo Sleep									*/
		
	}
}

/*PORT Config*/
void io_port_config(void)
{
	IO_DDR1.byte = 0x70;					/* PORT1 DDR set(LED1-3) 						*/
	LED 		 = 0xFF;					/* PORT1 PDR set(LED1-3) set to off 			*/
	INPUT 		 = 0;						/* PORT5 DDR set(AD4) 							*/
	BUZZER 		 = 1;						/* P17 as BUZZER output 						*/
}

/*ADC Config*/
void ADC_config(void)
{
	IO_ICR03.byte 		 = 0x06;			/* A/D INT level set 							*/
	IO_ADCSH.bit.BUSY    = 0;				/* ADC stop 									*/
	IO_ADER.byte 		 = 0x10;			/* ADC input enable, ch-4, 0x10 				*/
											/* Set to 0x01 for lazy testing 				*/
	IO_ADCSL.byte 		 = 0xA4;			/* ADC mode set 0xA4 							*/
											/* Set to 0x80 for lazy testing 				*/
	IO_ADCRLH.byte.ADCRH = 0x78;			/* Set 10bit-AD, comp.176cycle, sample.128cycle */
											/* Set to 0xF8 for 8-bit						*/
	IO_ADCSH.bit.INTE 	 = 1;				/* Interrupt enable 							*/
	IO_ADCSH.bit.STRT 	 = 1;				/* Start ADC 									*/
}

/*Timer Config*/
void timer_config(int milliseconds)
{	
	IO_ICR03.byte 		= 0x00;				/* set interrupt priority to highest 			*/
	IO_TMR[0] 			= milliseconds * 62;
	IO_TMCSR0.bit.CSL	= 2;				/* use 25T count clock cycle; trigger disabled 	*/
	IO_TMCSR0.bit.RELD	= 1;				/* reload mode 									*/
	IO_TMCSR0.bit.INTE 	= 1;				/* underflow interrupt enable 					*/
	IO_TMCSR0.bit.CNTE 	= 1;				/* enable timer operation 						*/
	IO_TMCSR0.bit.TRG 	= 1;				/* start counting after reload; start counter 	*/
}

/*PPG Timer Config*/
void ppgtimer_config(int selectorH, int selectorL)
{
	/* setting up PPG Timer PPG3 */
	
	IO_PPGC23.bit.PE1 = 1;					/* enable PPG pin output 						*/
	IO_PPG23.byte = 0x00;					/* PPG count clock setting (same as CPU) 		*/

	IO_PRL23.byte.PRLL3 = selectorL;		/* low-level value % of 1000 (clock factor) 	*/
	IO_PRL23.byte.PRLH3 = selectorH;		/* high-level value % of 1000 (clock factor) 	*/

	IO_PPGC23.bit.PEN1 = 1;					/* enable PPG Timer 							*/

	/* The above configuration will generate a 2 KHz signal at 50% duty cycle at P17 		*/
}

/*First Task*/
void get_WaterLvl(void)
{
	if (WATER_LVL_DATA < AD_DATA)			/* Get the highest reading						*/
		WATER_LVL_DATA = AD_DATA;
}

/*Second Task*/
void output_LED(void)
{
	if(WATER_LVL_DATA < 512)
		LED = 0x01 << 4 ;
	else if(WATER_LVL_DATA < 900)
		LED = 0x02 << 4;
	else
		LED = 0x04 << 4;
}

void sound_BUZ(void)
{
	if (WATER_LVL_DATA > 899)				/* Sound the buzzer if critical level reached 	*/
		ppgtimer_config(4000, 12000);
	else
	{
		ppgtimer_config(0,1000);
		tasks[2].elapsedTime = 4900;		/* Task 3 does not run 							*/
	}
	WATER_LVL_DATA = 0;						/* Reset the Water Level Data 					*/
}

void TimerOFF() 
{ 
 	IO_TMCSR0.bit.CNTE = 0; 				/* Disable timer operation 						*/
}

/*****************************************************************************
	interrupt routine
******************************************************************************/
/* reload timer interrupt routine */
__interrupt void TimerISR(void)
{
	IO_TMCSR0.bit.UF = 0; 
	if (TimerFlag)
	{
		/* Timer ticked before task processing done */
	}
	else
	{
		TimerFlag = 1;
	}
}

/* ADC interrupt routine */
__interrupt void ADC_interrupt(void)
{
	IO_ADCSH.bit.INT=0x0;					/* Clear INT request flag 						*/

	AD_DATA = IO_ADCRLH.DATA10 & 0x3FF;
}


/* Vector Table */
#pragma section INTVECT,locate=0xfffc00
#pragma intvect _start			0x8  0x0	/* Reset Vector 								*/
#pragma intvect TimerISR		17			/* Reload Timer Interrupt Vector 				*/
#pragma intvect ADC_interrupt	18			/* AD Converter Vector 			 				*/