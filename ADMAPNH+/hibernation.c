#include <msp430.h>
#include <stdio.h>
#include <string.h>
#include "hibernation.h"


#pragma SET_DATA_SECTION(".fram_vars")

unsigned long int *FRAM_write_ptr = (unsigned long int *) FRAM_START; // define a pointer that points the Start Address

//This pointer is used to copy the RAM
unsigned long int *RAM_copy_ptr = (unsigned long int *) RAM_START; // define a pointer for memory copy
unsigned long int *FLAG_V_monitor = (unsigned long int *) FLAG; // define a pointer for memory copy
unsigned long int *FLAG_V_monitor_2 = (unsigned long int *) FLAG_2; // define a pointer for memory copy
unsigned long int *FRAM_read_ptr = (unsigned long int *) FRAM_TEST_START;  // define a pointer that points the Start Address
//unsigned long int *FLAG_V_monitor_3 = (unsigned long int *) FLAG_3; // define a pointer for memory copy



unsigned long int *FLAG_interrupt = (unsigned long int *) INT; // define a pointer for memory copy
unsigned long int *V_monitor = (unsigned long int *) VMON; // define a pointer for memory copy
unsigned long int *V_monitor_2 = (unsigned long int *) VMON_2; // define a pointer for memory copy
unsigned long int *V_monitor_3 = (unsigned long int *) VMON_3; // define a pointer for memory copy


unsigned long int *Counter = (unsigned long int *) COUNT; // define a pointer for memory copy
unsigned long int *Counter_2 = (unsigned long int *) COUNT_2; // define a pointer for memory copy
//unsigned long int *Counter_3 = (unsigned long int *) COUNT_3; // define a pointer for memory copy

unsigned long int *VH_Calibr = (unsigned long int *) CALIBR; // define a pointer for memory copy
unsigned long int *CC_Check = (unsigned long int *) CHECK; // define a pointer for memory copy

unsigned long int n;


int t;
int cal;

//This pointer is used to copy the general registers
unsigned int *Memory_copy_ptr; // define a pointer for memory copy

//These pointers and variable are used to set the PC
unsigned long int *FRAM_pc = (unsigned long int *) PC;

unsigned long int* current_SP;
unsigned long int PC_values;

unsigned long data; //4 Byte

extern int counter;

//This array is used to restore the state
unsigned int Registers[230];

unsigned int *Reg_copy_ptr;

unsigned int counter_check_reg;

unsigned int temp;

unsigned int counter_check;


// extern unsigned int CC_check;

unsigned int prova;

int pro = 0;
int saved;

unsigned int set;
unsigned int PN;

int c;

long int deltaV;
long int sleepV;

#pragma SET_DATA_SECTION()

const unsigned int vol[256]= {
	1966, 1973, 1979, 1986, 1992, 1999, 2006, 2012, 2019, 2025, 2032, 2038, 2045, 2051, 2058, 2065, 2071, 2078, 2084, 2091,
	2097, 2104, 2110, 2117, 2124, 2130, 2137, 2143, 2150, 2156, 2163, 2169, 2176, 2183, 2189, 2196, 2202, 2209, 2215, 2222,
	2228, 2235, 2242, 2248, 2255, 2261, 2268, 2274, 2281, 2287, 2294, 2301, 2307, 2314, 2320, 2327, 2333, 2340, 2346, 2353,
	2360, 2366, 2373, 2379, 2386, 2392, 2399, 2405, 2412, 2419, 2425, 2432, 2438, 2445, 2451, 2458, 2464, 2471, 2478, 2484,
	2491, 2497, 2504, 2510, 2517, 2523, 2530, 2537, 2543, 2550, 2556, 2563, 2569, 2576, 2582, 2589, 2596, 2602, 2609, 2615,
	2622, 2628, 2635, 2641, 2648, 2655, 2661, 2668, 2674, 2681, 2687, 2694, 2700, 2707, 2714, 2720, 2727, 2733, 2740, 2746,
	2753, 2759, 2766, 2773, 2779, 2786, 2792, 2799, 2805, 2812, 2818, 2825, 2832, 2838, 2845, 2851, 2858, 2864, 2871, 2877,
	2884, 2891, 2897, 2904, 2910, 2917, 2923, 2930, 2936, 2943, 2950, 2956, 2963, 2969, 2976, 2982, 2989, 2995, 3002, 3009,
	3015, 3022, 3028, 3035, 3041, 3048, 3054, 3061, 3068, 3074, 3081, 3087, 3094, 3100, 3107, 3113, 3120, 3127, 3133, 3140,
	3146, 3153, 3159, 3166, 3172, 3179, 3186, 3192, 3199, 3205, 3212, 3218, 3225, 3231, 3238, 3245, 3251, 3258, 3264, 3271,
	3277, 3284, 3290, 3297, 3304, 3310, 3317, 3323, 3330, 3336, 3343, 3349, 3356, 3363, 3369, 3376, 3382, 3389, 3395, 3402,
	3408, 3415, 3422, 3428, 3435, 3441,	3448, 3454, 3461, 3467, 3474, 3481, 3487, 3494, 3500, 3507, 3513, 3520, 3526, 3533,
	3540, 3546, 3553, 3559, 3566, 3572, 3579, 3585, 3592, 3599, 3605, 3612, 3618, 3625, 3631, 3638 };


const unsigned int ref0[32]= {
          CDREF0_0, CDREF0_1, CDREF0_2, CDREF0_3, CDREF0_4, CDREF0_5, CDREF0_6, CDREF0_7, CDREF0_8, CDREF0_9,
          CDREF0_10, CDREF0_11, CDREF0_12, CDREF0_13, CDREF0_14, CDREF0_15, CDREF0_16, CDREF0_17, CDREF0_18, CDREF0_19,
          CDREF0_20, CDREF0_21, CDREF0_22, CDREF0_23, CDREF0_24, CDREF0_25, CDREF0_26, CDREF0_27, CDREF0_28, CDREF0_29,
          CDREF0_30, CDREF0_31};

int cal=0;
long int CalValue_1=0;
long int CalValue_2=0;
long int CalValue_3=0;
long int CalValue_4=0;
long int CalValue_5=0;
long int CalValue_6=0;
long int Vmax=0;
long int Vmon=2000;

unsigned long int Counter_3=0;


unsigned int h=0;


extern volatile unsigned long int ADCResult;

unsigned int i, k;
unsigned int ModeAddress =  FRAM_START;
unsigned int StartAddress =  RAM_START;

unsigned int TX_Buffer_V1[4] = {0,0,0,0};

void SystemInit(void){

  GPIO_configuration();

  Clock_configuration();


  // Calibration

  if((*VH_Calibr)!=1){

		*VH_Calibr=0;  // It is the first time that the algorithm is running so we have to consider the calibration.

  }


  if((*VH_Calibr)==0){

	  // P3.6 Calibration Debugger
	 	  //P3DIR |= BIT6;
	 	 // P3OUT |= BIT6; // LED  ON

	 	  deltaV=0;

		  Calibration();

		  P3OUT &= ~BIT6; // LED 4 OFF

	}


  // Test

  // Vmonitor is the function for dinamically setting the external comparator threshold
  // at the minimun value before executing the test.

  Vmonitor();

  // P2.0 is used for generating the interrupt from the external analog comparator circuit


  //this t flag is to understand if the P2.0 is already 1.If it is, we don't have to put the system in LPM4

//  t=1;

  __delay_cycles(500); // (?) we need this delay ?


  t=0;

  t = (P2IN & 0x0001);

  if(t==0){

	    *FLAG_interrupt=1;

	    P2LO();

	    // Port Configuration

	    //This Pin P1.4 is not working properly. don't use.

		P1OUT &= ~(BIT4);
		P1DIR |= BIT4;

		// LPM4
	    __bis_SR_register(LPM4_bits + GIE);        // LPM0, ADC10_ISR will force exit
	    __no_operation();


  }

  Test();


  //PN = 1; //debug
  P3OUT &= ~(BIT6);
  PN=0;
  if(CalValue_2 < CalValue_1){

	Set_internal_comparator ();
	PN=1;
	//if((CalValue_1 - CalValue_2)>100){PN=1;} //1
  	//P3DIR |= BIT6;
  	//P3OUT |= BIT6;
   unsigned int vmonitor_3;

//    if(1){

	   Counter_3=0;

//     vmonitor_3=3600;  //minimun value

	   vmonitor_3=VMIN;  //minimun value


   // set up external Comparator = Vmonitor

   Set_V(vmonitor_3); // Vmonitor by using the external circuit

   //__delay_cycles(500);

   t=1;

   t = (P2IN & 0x0001);

   if(t==0){  //the voltage is increasing slowly

		*FLAG_interrupt=3;

		P2LO();

		StartDebounceTimer_2();

		// LPM4
		__bis_SR_register(LPM4_bits + GIE);        // LPM0, ADC10_ISR will force exit
		__no_operation();


      }else{
    	  //the voltage is increasing quickly

    	  vmonitor_3=VMAX;  //minimun value

    	     // set up external Comparator = Vmonitor

    	   Set_V(vmonitor_3); // Vmonitor by using the external circuit

    	   //__delay_cycles(500);

    	   t=1;

    	   t = (P2IN & 0x0001);

    	   if(t==0){  //the voltage is increasing slowly

    	  		*FLAG_interrupt=1;

    	  		P2LO();

    	  		StartDebounceTimer_2();

    	  		// LPM4
    	  		__bis_SR_register(LPM4_bits + GIE);        // LPM0, ADC10_ISR will force exit
    	  		__no_operation();

    	   }

      }



  }


  if ((*CC_Check!=0) && (*CC_Check!=1) ){

//	  unsigned long int value= 3400 + deltaV;

	  *CC_Check=0;
	  *FLAG_interrupt=2;

	  unsigned int try = (unsigned int) (VMINN + deltaV);

	  if (PN==1){

		  try=try+sleepV;
		  *FLAG_interrupt=5;
	  }

//      unsigned long int value = VMIN2;


		Set_V(try);

		  //__delay_cycles(500); // (?) we need this delay ?

		P2HI();

		   __bis_SR_register(GIE);        // LPM0, ADC10_ISR will force exit
		   __no_operation();


	  }



   if (*CC_Check==0){

			deltaV=deltaV+10;
			sleepV=sleepV+10;

			*CC_Check=0;

			unsigned int value_2 = (unsigned int) (VMINN + deltaV);
	   			*FLAG_interrupt=2;
	   			if (PN==1){
	   				value_2=value_2+sleepV;
		 			*FLAG_interrupt=5;}

			Set_V(value_2);

			P2HI();

			__bis_SR_register(GIE);        // LPM0, ADC10_ISR will force exit
			__no_operation();

	  }


   pro=0;

   if (*CC_Check==1){

	   *CC_Check=0;

	   unsigned int	value_3 = (unsigned int) (VMINN + deltaV);
	   if (PN==1){value_3=value_3+sleepV;}

		Set_V(value_3);

	    P3DIR |= BIT0;
	    P3OUT |= BIT0; // LED  ON

		Restore();

	  }

   }


void Test(void){


  	  //ADC initialization for the test

  	  // Configure ADC10 - Pulse sample mode; ADC10SC trigger
  	  ADC10CTL0 = ADC10SHT_11 + ADC10ON;         // 16 ADC10CLKs; ADC ON

  	  ADC10CTL1 = ADC10SHP + ADC10CONSEQ_0;     // s/w trig, single ch/conv
  	  ADC10CTL2 = ADC10RES;                     // 10-bit conversion results
 //	  ADC10MCTL0 = ADC10SREF_1 + ADC10INCH_11;  // AVcc/2 internal divider for Vcc

  	  ADC10MCTL0 = ADC10SREF_3 + ADC10INCH_2;  // Veref+ and external divider - In A2

  	  // Configure internal reference
  	  while(REFCTL0 & REFGENBUSY);              // If ref generator busy, WAIT
  	  REFCTL0 |= REFVSEL_1+REFON;               // Select internal ref = 1.5V
  	                                            // Internal Reference ON
  	  __delay_cycles(400);                      // Delay for Ref to settle


	  ADC10CTL0 |= ADC10ENC + ADC10SC;        // Sampling and conversion start
	  while (ADC10CTL1 & ADC10BUSY);          // ADC10BUSY?
	  __no_operation();                       // For debug only

	  CalValue_1 = ADC10MEM0;

	 test();

	 CalValue_2=0;

	  ADC10CTL0 |= ADC10ENC + ADC10SC;        // Sampling and conversion start
	  while (ADC10CTL1 & ADC10BUSY);          // ADC10BUSY?
	  __no_operation();                       // For debug only

	 CalValue_2 = ADC10MEM0;




  	// ADC is turned off

  	  ADC10CTL0 &= ~ ADC10ON;        // 16 ADC10CLKs; ADC ON

  	 // only for debugging

  	  CalValue_1=(((CalValue_1*1179)/1024)*4200)/1000; //4152

  	  CalValue_2=(((CalValue_2*1179)/1024)*4200)/1000; //4152

        //Test_completed

  	  *FLAG_V_monitor=1;

  	  P1OUT &= ~ BIT7; // LED  ON


}

void Set_internal_comparator (void){

// Configure CD0
	P3SEL1 |= BIT1;  //CD13
	P3SEL0 |= BIT1;

	CDCTL0|=CDIPEN+CDIPSEL_13; // Enable V+, input channel CD13
	CDCTL2|=CDRSEL;			// VREF is applied to -terminal

	// Reference Voltage Generator
	CDCTL2|=CDREFL_2; //2v // Shared Ref. 1.2V from the shared reference ON: Comp. D Reference voltage level 3 : 1.5V
	// R-ladder off; bandgap ref voltage (1.2V)
	// supplied ref amplifier to get Vcref=1.5V (CDREFL_2)

	CDCTL2|=CDRS_2; // Comp. D Reference Source 2 : Shared Ref. CDRS_2=10b

	CDCTL1&= ~(CDSHORT+CDEX+CDMRVS);

	n=0;

	CDCTL2 |= ref0[(int)n];

	CDCTL1|=CDF;
	CDCTL1|=CDFDLY_0;

}



void GPIO_configuration (void){

    P2REN &= ~(BIT0);                  // Disable pull-down and pull-up resistor
    P2DIR &= ~(BIT0);                  // Direction = BIT0 input

	//GPIO configuration

	P1OUT &= ~(0xF9);
	P2OUT &= ~(0xFE);
	P3OUT &= ~(0xF9);//FB previously
	P4OUT &= ~(0xFF);
	PJOUT &= ~(0xFF); // Configure the pull-down resistor


	P1DIR |= 0xF9;
	P2DIR |= 0xFE;
	P3DIR |= 0xF9;
	P4DIR |= 0xFF;
	PJDIR |= 0xFF; // // Direction = output


	//External reference (Vcc)

	P1SEL1 |= BIT1;
	P1SEL0 |= BIT1;

	//External ADC input

	P1SEL1 |= BIT2;
	P1SEL0 |= BIT2;

//	__delay_cycles(5000); // (?) we need this delay ?

	//This GPIO is used for the short-circuit

	P1OUT &= ~(BIT3);
	P1DIR |= BIT3;


}

void Clock_configuration (void){

	  // Startup clock system in max. DCO setting ~8MHz
	  // The first thing is to set all clocks of the system MCLK, ACLK, SMCLK by using the CS control registers.

	  // Note: The registers have 16bits. For a generic register, the suffix "_L" (ANYREG_L) refers to the lower byte
	  // of the register (bits 0 through 7). The suffix "_H" refers to the upper byte of the register(bits 8 through 15).

	  // The registers are locked, then, is need to use a pwd to unlock them.
	  // CSCTL0 Clock System Control Register 0 is only used to unlock the other CS registers used for clock setting.

	  // Allow for settling delay
	  //  __delay_cycles(10000);

	  //core frequency

	  CSCTL0_H = 0xA5;                          // Unlock register
	                                            //CSKEY password. Always reads as 0A5h. By writing any other value in
	                                            // word mode generates a PUC. After a correct password, CS register accesses are enabled.

	  //  CSCTL1 |= DCORSEL;  //Set max. DCO setting. 20MH in this case

	  CSCTL1 &= ~(DCORSEL);  //Set max. DCO setting. 5.33MH in this case

	  CSCTL1 |= DCOFSEL0 + DCOFSEL1;            //DCO frequency select register
	                                              //Set max. DCO setting. 8MH in this case

	//  CSCTL1 &= ~(DCOFSEL1);  //Set max. DCO setting. 5.33MH in this case
	//  CSCTL1 &= ~(DCOFSEL0);  //Set max. DCO setting. 5.33MH in this case

	  CSCTL2 = SELA_1 + SELS_3 + SELM_3;        // Selects the ACLK, MCLK AND SMCLK sources register
	                                            // set ACLK = VLO; MCLK = DCO; SMCLK = DCO;

	  CSCTL3 = DIVA_0 + DIVS_0 + DIVM_0;        // MCLK, SMCLK and ACLK source divider/prescaler register.

	  CSCTL0_H = 0x01;                          // Lock Register


}

void StartDebounceTimer(void)

 // Timer_A is a 16-bit timer/counter with up to seven capture/compare registers. Timer_A can support
 // multiple capture/compares, PWM outputs, and interval timing. Timer_A also has extensive interrupt
 // capabilities.

{

  // default delay = 0
  // Debounce time = 1500* 1/8000 = ~200ms
    TA1CCTL0 = CCIE;                          // TACCR0 interrupt enabled

//  TA1CCR0 = 750; // 88ms
    TA1CCR0 = 375; // 88ms

//  TA1CCR0 = 16000; // 88ms

  TA1CTL = TASSEL_1 + MC_1;                 // Timer_A clock source select TA1CTL registers: 01b = ACLK

}

void StartDebounceTimer_2(void)

 // Timer_A is a 16-bit timer/counter with up to seven capture/compare registers. Timer_A can support
 // multiple capture/compares, PWM outputs, and interval timing. Timer_A also has extensive interrupt
 // capabilities.

{

  // default delay = 0
  // Example: Debounce time = 1500* 1/8000 = 187.5ms

  TA1CCTL0 = CCIE;                          // TACCR0 interrupt enabled

//  TA1CCR0 = 800;  // Example: Debounce time = 800 * 1/8000 = 100ms
    TA1CCR0 = 80;  // Example: Debounce time = 80 * 1/8000 = 10ms


  TA1CTL = TASSEL_1 + MC_1;                 // Timer_A clock source select TA1CTL registers: 01b = ACLK

}


void test(void){

    while(FRAM_read_ptr < (unsigned long int *) (FRAM_TEST_END)) {

    	data=*FRAM_read_ptr++;

    }


}


void Vmonitor(void){



	unsigned int vmonitor;

	// If it is the first time *FLAG_V_monitor==FFFF

	if ((*FLAG_V_monitor!=0) && (*FLAG_V_monitor!=1) ){

		*V_monitor=VMIN; // Minimun value

//		*V_monitor=3400; //for testing

		*Counter=0; // Counter is for counting how many times the test fails
		            // Because the energy available is not enough to complete the test


	}



	// If the test fails

	if (*FLAG_V_monitor==0){

		*Counter=*Counter+1;

		*V_monitor=VMIN +((*Counter)*100);

//		*V_monitor=3400+((*Counter)*100);  //for testing


	}

	// If the test is good, *V_monitor conteins the right value for the next times

	vmonitor= (unsigned int) (*V_monitor);

	// STATE = 0

	*FLAG_V_monitor=0; //Flag is set to 0 when the test is starting.

    // P1.7 Test routine debugger pin
	P1DIR |= BIT7;
	P1OUT |= BIT7; // LED  ON

	  // set up external Comparator = vmonitor

    Set_V(vmonitor); // Vmonitor by using the external circuit


}


void Vmonitor_2(void){


	unsigned int vmon;


	if ((*FLAG_V_monitor_2!=0) && (*FLAG_V_monitor_2!=1) ){

		*V_monitor_2 = VMIN; //min. value

//		*V_monitor_2 = 3400; //for testing

		*Counter_2=0; // Counter is for counting how many times the test fails
		            // Because the energy available is not enough to complete the test

	}


	if (*FLAG_V_monitor_2==0){

		*Counter_2=*Counter_2+1;

		*V_monitor_2=VMIN +((*Counter)*100);

//		*V_monitor_2=3400+((*Counter_2)*100);  //for testing


	}


	vmon = (unsigned int) (*V_monitor_2);

	// STATE = 0

	*FLAG_V_monitor_2=0; //Flag is set to 0 when the test is starting.

	  // set up external Comparator = Vmonitor

    Set_V(vmon); // Vmonitor by using the external circuit


}



//}


void Calibration(void){

	P3DIR |= BIT6;
	P3OUT |= BIT6; // LED  ON


	  Vmonitor_2();

	  // P2.0 is used for generating the interrupt from the external analog comparator circuit

	  //this flag is to understand if the P2.0 is already 1.If it is, we don't have to put the system in LPM4

	  __delay_cycles(500);

	  t=1;

	  t = (P2IN & 0x0001);

	  if(t==0){

		    *FLAG_interrupt=1;

		    P2LO();

			// LPM4
		    __bis_SR_register(LPM4_bits + GIE);        // LPM0, ADC10_ISR will force exit
		    __no_operation();


	  }


  	  //ADC initialization for the test

  	  // Configure ADC10 - Pulse sample mode; ADC10SC trigger
  	  ADC10CTL0 = ADC10SHT_11 + ADC10ON;         // 16 ADC10CLKs; ADC ON

  	  ADC10CTL1 = ADC10SHP + ADC10CONSEQ_0;     // s/w trig, single ch/conv
  	  ADC10CTL2 = ADC10RES;                     // 10-bit conversion results
 		//	  ADC10MCTL0 = ADC10SREF_1 + ADC10INCH_11;  // AVcc/2 internal divider for Vcc

  	  ADC10MCTL0 = ADC10SREF_3 + ADC10INCH_2;  // Veref+ and external divider - In A2

  	  // Configure internal reference
  	  while(REFCTL0 & REFGENBUSY);              // If ref generator busy, WAIT
  	  REFCTL0 |= REFVSEL_1+REFON;               // Select internal ref = 1.5V
  	                                            // Internal Reference ON
  	  __delay_cycles(400);                      // Delay for Ref to settle


		//  	  for(i=0; i<10; i++){


  	 ADC10CTL0 |= ADC10ENC + ADC10SC;        // Sampling and conversion start
  	 while (ADC10CTL1 & ADC10BUSY);          // ADC10BUSY?
  	 __no_operation();                       // For debug only

  	 CalValue_3 = ADC10MEM0;


  	 // GPIO to short-circuit the input

     P1DIR |= BIT3;
	 P1OUT |= BIT3;


	  ADC10CTL0 &= ~(ADC10ON);
	  cal=1;
	  CheckPoint();

		//      ADC10CTL0 = ADC10ON;


	  // Configure ADC10 - Pulse sample mode; ADC10SC trigger
	  ADC10CTL0 = ADC10SHT_11 + ADC10ON;         // 16 ADC10CLKs; ADC ON

	  ADC10CTL1 = ADC10SHP + ADC10CONSEQ_0;     // s/w trig, single ch/conv
	  ADC10CTL2 = ADC10RES;                     // 10-bit conversion results
		//	  ADC10MCTL0 = ADC10SREF_1 + ADC10INCH_11;  // AVcc/2 internal divider for Vcc

	  ADC10MCTL0 = ADC10SREF_3 + ADC10INCH_2;  // Veref+ and external divider - In A2

	  // Configure internal reference
	  while(REFCTL0 & REFGENBUSY);              // If ref generator busy, WAIT
	  REFCTL0 |= REFVSEL_1+REFON;               // Select internal ref = 1.5V
	                                            // Internal Reference ON
	  __delay_cycles(400);                      // Delay for Ref to settle


	  CalValue_4 = 0;



		//	  for(i=0; i<10;i++){

	  ADC10CTL0 |= ADC10ENC + ADC10SC;        // Sampling and conversion start
	  while (ADC10CTL1 & ADC10BUSY);          // ADC10BUSY?
	  __no_operation(); // For debug only


	  CalValue_4 = ADC10MEM0;
	  P3OUT &= ~BIT6;

	  TA1CCTL0 = CCIE;                          // TACCR0 interrupt enabled
	  		TA1CCR0 = 8;  // Example: Debounce time = 80 * 1/8000 = 10ms
	  		TA1CTL = TASSEL_1 + MC_1;

	  	  P3DIR |= BIT6;
	  	  P3OUT |= BIT6;
	      	// LPM4
	      	__bis_SR_register(LPM4_bits + GIE);        // LPM0, ADC10_ISR will force exit
	      	__no_operation();

	  		//      ADC10CTL0 = ADC10ON;


	  	  // Configure ADC10 - Pulse sample mode; ADC10SC trigger
	  	  ADC10CTL0 = ADC10SHT_11 + ADC10ON;         // 16 ADC10CLKs; ADC ON

	  	  ADC10CTL1 = ADC10SHP + ADC10CONSEQ_0;     // s/w trig, single ch/conv
	  	  ADC10CTL2 = ADC10RES;                     // 10-bit conversion results
	  		//	  ADC10MCTL0 = ADC10SREF_1 + ADC10INCH_11;  // AVcc/2 internal divider for Vcc

	  	  ADC10MCTL0 = ADC10SREF_3 + ADC10INCH_2;  // Veref+ and external divider - In A2

	  	  // Configure internal reference
	  	  while(REFCTL0 & REFGENBUSY);              // If ref generator busy, WAIT
	  	  REFCTL0 |= REFVSEL_1+REFON;               // Select internal ref = 1.5V
	  	                                            // Internal Reference ON
	  	  __delay_cycles(400);                      // Delay for Ref to settle


	  	  CalValue_6 = 0;

	  		//	  for(i=0; i<10;i++){

	  	  ADC10CTL0 |= ADC10ENC + ADC10SC;        // Sampling and conversion start
	  	  while (ADC10CTL1 & ADC10BUSY);          // ADC10BUSY?
	  	  __no_operation();                       // For debug only

	  	  CalValue_6 = ADC10MEM0;

	  P1OUT &= ~(BIT3);

		//	  CalValue_3 = CalValue_3/10;

	  ADC10CTL0 &= ~(ADC10ON);


	  // only for debugging

	  CalValue_4=(((CalValue_4*1179)/1024)*4200)/1000; //4152

	  CalValue_3=(((CalValue_3*1179)/1024)*4200)/1000; //3  instead of 3.2 because of the op.amp.

	  CalValue_6=(((CalValue_6*1179)/1024)*4200)/1000;

	  deltaV= (long int) (CalValue_3 - CalValue_4 - 50);

		  P3OUT &= ~BIT6;
	  __delay_cycles(500);

	  //SCalibration();

	  sleepV= (long int) (CalValue_4 - CalValue_6 + 50);

	  *FLAG_V_monitor_2=1;
	  *VH_Calibr=1;


}



void Set_V (unsigned int v) {

	P3DIR |= BIT6;
	P3OUT |= BIT6;

    counter_check=0;

  	for (i= 0; i<256; i++)
  	  {
  		if(v>vol[i]){
  			counter_check++;
  		} else {
  			break;
  		}

   	}


    P4OUT &= ~(BIT0); //blue
    P2OUT &= ~(BIT1); //yellow
    P2OUT &= ~(BIT2); // green
  	P3OUT &= ~(BIT3);  //orange
    P1OUT &= ~(BIT0); //blue
    P1OUT &= ~(BIT5); //yellow
    P1OUT &= ~(BIT6); //green
    P2OUT &= ~(BIT5); //orange




//  	P4DIR |= BIT0;
    P4OUT |= (BIT0 & (counter_check & 0x0001)); //blue

//    P2DIR |= BIT1;
    P2OUT |= (BIT1 & (counter_check & 0x0002)); //yellow

//    P2DIR |= BIT2;
    P2OUT |= (BIT2 & (counter_check & 0x0004)); // green

//    P3DIR |= BIT3;
  	P3OUT |= (BIT3 & (counter_check & 0x0008));  //orange

//  	P1DIR |= BIT0;
    P1OUT |= (BIT0 & ((counter_check & 0x0010) >> 4)); //blue

//    P1DIR |= BIT5;
    P1OUT |= (BIT5 & (counter_check & 0x0020)); //yellow

//    P1DIR |= BIT6;
    P1OUT |= (BIT6 & (counter_check & 0x0040)); //green

//   P2DIR |= BIT5;
    P2OUT |= (BIT5 & ((counter_check & 0x0080) >> 2)); //orange

    P3OUT &= ~(BIT6);
    __delay_cycles(500);
}


void CheckPoint (void) {

	if(cal==0){
		P3DIR |= BIT5;
		P3OUT |= BIT5; // LED 4 ON

		P2OUT &= ~BIT6; // LED  OFF

		*CC_Check=0;
	}

	// copy all the registers onto the FRAM

	  asm(" MOVA R1,&0xEDF4");
	  asm(" MOVA R2,&0xEDF8");
	  asm(" MOVA R3,&0xEDFC");
	  asm(" MOVA R4,&0xEE00");
	  asm(" MOVA R5,&0xEE04");
	  asm(" MOVA R6,&0xEE08");
	  asm(" MOVA R7,&0xEE0C");
	  asm(" MOVA R8,&0xEE10");
	  asm(" MOVA R9,&0xEE14");
	  asm(" MOVA R10,&0xEE18");
	  asm(" MOVA R11,&0xEE1C");
	  asm(" MOVA R12,&0xEE20");
	  asm(" MOVA R13,&0xEE24");
	  asm(" MOVA R14,&0xEE28");
	  asm(" MOVA R15,&0xEE2C");


			  current_SP = (void*) _get_SP_register();

			  *FRAM_pc= *current_SP;

				__data16_write_addr((unsigned short) &DMA0SA,(unsigned long) RAM_START);
														 // Source block address
				__data16_write_addr((unsigned short) &DMA0DA,(unsigned long) (FRAM_START2));
				DMA0SZ = RAM_SIZE;
				DMA0CTL = DMADT_1+DMASRCINCR_3+DMADSTINCR_3;
				DMA0CTL |= DMAEN;
				DMA0CTL |= DMAREQ;

				//Reg1

						__data16_write_addr((unsigned short) &DMA0SA,(unsigned long) R_LOC);
						// Source block address
						__data16_write_addr((unsigned short) &DMA0DA,(unsigned long) (R_BACKUP_LOC));
						DMA0SZ =R_SIZE;
						DMA0CTL |= DMAEN;
						DMA0CTL |= DMAREQ;

						//Reg2

						__data16_write_addr((unsigned short) &DMA0SA,(unsigned long) R_LOC2);
						// Source block address
						__data16_write_addr((unsigned short) &DMA0DA,(unsigned long) (R_BACKUP_LOC2));
						DMA0SZ = R_SIZE2;
						DMA0CTL |= DMAEN;
						DMA0CTL |= DMAREQ;

						//Reg3

						__data16_write_addr((unsigned short) &DMA0SA,(unsigned long) R_LOC3);
						// Source block address
						__data16_write_addr((unsigned short) &DMA0DA,(unsigned long) (R_BACKUP_LOC3));
						DMA0SZ = R_SIZE3;
						DMA0CTL |= DMAEN;
						DMA0CTL |= DMAREQ;

						//Reg4

						__data16_write_addr((unsigned short) &DMA0SA,(unsigned long) R_LOC4);
						// Source block address
						__data16_write_addr((unsigned short) &DMA0DA,(unsigned long) (R_BACKUP_LOC4));
						DMA0SZ = R_SIZE4;
						DMA0CTL |= DMAEN;
						DMA0CTL |= DMAREQ;

						//Reg5

						__data16_write_addr((unsigned short) &DMA0SA,(unsigned long) R_LOC5);
						// Source block address
						__data16_write_addr((unsigned short) &DMA0DA,(unsigned long) (R_BACKUP_LOC5));
						DMA0SZ = R_SIZE5;
						DMA0CTL |= DMAEN;
						DMA0CTL |= DMAREQ;

						//Reg6

						__data16_write_addr((unsigned short) &DMA0SA,(unsigned long) R_LOC6);
						// Source block address
						__data16_write_addr((unsigned short) &DMA0DA,(unsigned long) (R_BACKUP_LOC6));
						DMA0SZ = R_SIZE6;
						DMA0CTL |= DMAEN;
						DMA0CTL |= DMAREQ;

						//Reg7

						__data16_write_addr((unsigned short) &DMA0SA,(unsigned long) R_LOC7);
						// Source block address
						__data16_write_addr((unsigned short) &DMA0DA,(unsigned long) (R_BACKUP_LOC7));
						DMA0SZ = R_SIZE7;
						DMA0CTL |= DMAEN;
						DMA0CTL |= DMAREQ;

						//Reg8

						__data16_write_addr((unsigned short) &DMA0SA,(unsigned long) R_LOC8);
						// Source block address
						__data16_write_addr((unsigned short) &DMA0DA,(unsigned long) (R_BACKUP_LOC8));
						DMA0SZ = R_SIZE8;
						DMA0CTL |= DMAEN;
						DMA0CTL |= DMAREQ;

						if(cal==0){
							pro=0;
						    P3OUT &= ~BIT5; // LED 4 OFF

							*CC_Check = 1;
						}
						cal=0;

}

void Restore (void){

    	// Restore all Register and RAM

		MPUCTL0_H = 0xA5;
		PMMCTL0_H =  0xA5;
		FRCTL0_H = 0xA5;
		CSCTL0_H = 0xA5;

		//Reg1

		__data16_write_addr((unsigned short) &DMA0SA,(unsigned long) R_BACKUP_LOC);
		// Source block address
		__data16_write_addr((unsigned short) &DMA0DA,(unsigned long) (R_LOC));
		DMA0SZ =R_SIZE;
		DMA0CTL |= DMAEN;
		DMA0CTL |= DMAREQ;

		//Reg2

		__data16_write_addr((unsigned short) &DMA0SA,(unsigned long) R_BACKUP_LOC2);
		// Source block address
		__data16_write_addr((unsigned short) &DMA0DA,(unsigned long) (R_LOC2));
		DMA0SZ = R_SIZE2;
		DMA0CTL |= DMAEN;
		DMA0CTL |= DMAREQ;

		//Reg3

		__data16_write_addr((unsigned short) &DMA0SA,(unsigned long) R_BACKUP_LOC3);
		// Source block address
		__data16_write_addr((unsigned short) &DMA0DA,(unsigned long) (R_LOC3));
		DMA0SZ = R_SIZE3;
		DMA0CTL |= DMAEN;
		DMA0CTL |= DMAREQ;

		//Reg4

		__data16_write_addr((unsigned short) &DMA0SA,(unsigned long) R_BACKUP_LOC4);
		// Source block address
		__data16_write_addr((unsigned short) &DMA0DA,(unsigned long) (R_LOC4));
		DMA0SZ = R_SIZE4;
		DMA0CTL |= DMAEN;
		DMA0CTL |= DMAREQ;

		//Reg5

		__data16_write_addr((unsigned short) &DMA0SA,(unsigned long) R_BACKUP_LOC5);
		// Source block address
		__data16_write_addr((unsigned short) &DMA0DA,(unsigned long) (R_LOC5));
		DMA0SZ = R_SIZE5;
		DMA0CTL |= DMAEN;
		DMA0CTL |= DMAREQ;

		//Reg6

		__data16_write_addr((unsigned short) &DMA0SA,(unsigned long) R_BACKUP_LOC6);
		// Source block address
		__data16_write_addr((unsigned short) &DMA0DA,(unsigned long) (R_LOC6));
		DMA0SZ = R_SIZE6;
		DMA0CTL |= DMAEN;
		DMA0CTL |= DMAREQ;

		//Reg7

		__data16_write_addr((unsigned short) &DMA0SA,(unsigned long) R_BACKUP_LOC7);
		// Source block address
		__data16_write_addr((unsigned short) &DMA0DA,(unsigned long) (R_LOC7));
		DMA0SZ = R_SIZE7;
		DMA0CTL |= DMAEN;
		DMA0CTL |= DMAREQ;

		//Reg8

		__data16_write_addr((unsigned short) &DMA0SA,(unsigned long) R_BACKUP_LOC8);
		// Source block address
		__data16_write_addr((unsigned short) &DMA0DA,(unsigned long) (R_LOC8));
		DMA0SZ = R_SIZE8;
		DMA0CTL |= DMAEN;
		DMA0CTL |= DMAREQ;

		MPUCTL0_H = 0x01;
		PMMCTL0_H = 0x01;
		FRCTL0_H = 0x01;
		CSCTL0_H = 0x01;

		__data16_write_addr((unsigned short) &DMA0SA,(unsigned long) (FRAM_START2));
												 // Source block address
		__data16_write_addr((unsigned short) &DMA0DA,(unsigned long) RAM_START);
		DMA0SZ = RAM_SIZE;
		DMA0CTL = DMADT_1+DMASRCINCR_3+DMADSTINCR_3;
		DMA0CTL |= DMAEN;
		DMA0CTL |= DMAREQ;

		asm(" MOVA &0xEDF4,R1");
		asm(" MOVA &0xEDF8,R2");
		asm(" MOVA &0xEDFC,R3");
		asm(" MOVA &0xEE00,R4");
		asm(" MOVA &0xEE04,R5");
		asm(" MOVA &0xEE08,R6");
		asm(" MOVA &0xEE0C,R7");
		asm(" MOVA &0xEE10,R8");
		asm(" MOVA &0xEE14,R9");
		asm(" MOVA &0xEE18,R10");
		asm(" MOVA &0xEE1C,R11");
		asm(" MOVA &0xEE20,R12");
		asm(" MOVA &0xEE24,R13");
		asm(" MOVA &0xEE28,R14");
		asm(" MOVA &0xEE2C,R15");

		*current_SP=*FRAM_pc;

		P3OUT &= ~BIT0; // LED  ON

		__delay_cycles(500); // (?) we need this delay ?

		*FLAG_interrupt=2;

		P2HI();

		__bis_SR_register(GIE);        // LPM0, ADC10_ISR will force exit
		__no_operation();

		*CC_Check=0;

		pro=1;

		P2DIR |= BIT6;
		P2OUT |= BIT6; // LED  ON

    }

void P2LO(void){

    P2IFG = 0;                         // P2 IFG flag cleared
    P2IES &= ~(BIT0);                  // P2.0 Lo/Hi edge interrupt P2OUT &= ~(BIT0);
    P2IE |= BIT0;                      // P2.0 interrupt enabled
    P2IFG = 0;                         // P2 IFG flag cleared
}

void P2HI(void){

	  P2IFG = 0;                         // P2 IFG flag cleared
	  P2IES |= BIT0;                     // P2.0 Hi/lo edge interrupt
	  P2IE |= BIT0;                      // P2.0 interrupt enabled
	  P2IFG = 0;                         // P2 IFG flag cleared
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{

	CDCTL1 &= ~(CDON); //disable comparator
	CDINT &= ~(CDIFG + CDIIFG + CDIE); //clean flags and disable iterrupt

  switch(__even_in_range(P2IV,P2IV_P2IFG0)) // I think that even_in_range is correlated to the INT considered in a given Port
    {
     case P2IV_P2IFG0:

    	 //disable the interrupt for the timer

         TA1CCTL0 = 0;  // Disable Interrupt
    	 TA1CTL = 0;    // Disable Timer

		 // Toggles interrupt edge

//		 P2IES ^= BIT0;
    	 CDCTL1 &= ~(CDON); //disable comarator
    	 CDINT &= ~(CDIFG + CDIIFG + CDIE); //clean flags and disable iterrupt
		 P2IFG = 0;
		 P2IE &= ~BIT0;

//		 prova=(P2IES & 0x0001);

		 if (*FLAG_interrupt==3){

	         TA1CCTL0 = 0;  // Disable Interrupt
	    	 TA1CTL = 0;    // Disable Timer

			 Counter_3= Counter_3 + 1;

/*

			 StartDebounceTimer_2();
				// LPM4
			 __bis_SR_register(LPM4_bits + GIE);        // LPM0, ADC10_ISR will force exit
			 __no_operation();

*/

//			 vmonitor_3=3400+(Counter_3*100);  //minimun value

			 unsigned int vmonitor_4;

			 vmonitor_4= VMIN + (Counter_3*100);  //minimun value

			 Set_V(vmonitor_4); // Vmonitor by using the external circuit

			 t=1;

			 t = (P2IN & 0x0001);

			 if(t==0){ //the voltage is still increasing slowly

				*FLAG_interrupt=3;

				P2LO();

				// Port Configuration

				//This Pin P1.4 is not working properly. don't use.

				P1OUT &= ~(BIT4);
				P1DIR |= BIT4;



				StartDebounceTimer_2();

				// LPM4
				__bis_SR_register(LPM4_bits + GIE);        // LPM0, ADC10_ISR will force exit
				__no_operation();


			   }else{  //the voltage is increasing

				   unsigned int vmonitor_8;

			    	  vmonitor_8=VMAX;  //minimun value

			          // set up external Comparator = Vmonitor

			    	   Set_V(vmonitor_8); // Vmonitor by using the external circuit

			    	  //__delay_cycles(500);

			    	   t=1;

			    	   t = (P2IN & 0x0001);

			    	   if(t==0){  //the voltage is increasing slowly

			    	  		*FLAG_interrupt=1;

			    	  		P2LO();

			    	  		// Port Configuration

			    	  		//This Pin P1.4 is not working properly. don't use.

			    	  		P1OUT &= ~(BIT4);
			    	  		P1DIR |= BIT4;

			    	  		StartDebounceTimer_2();

			    	  		// LPM4
			    	  		__bis_SR_register(LPM4_bits + GIE);        // LPM0, ADC10_ISR will force exit
			    	  		__no_operation();

			    	   }

			      }

		 }

		 if (*FLAG_interrupt==2){

//			 if(prova==0){

			 P2IFG = 0;
			 P2IE &= ~BIT0;
			 P2OUT &=~(BIT6);
			 //CDCTL1 &= ~(CDON); //disable comarator
			 //CDINT &= ~(CDIFG + CDIIFG + CDIE); //clean flags and disable iterrupt

   //  		__delay_cycles(10000);                   //only way this interrupt works correctly

			 CheckPoint();

			 if(pro==0){

				    unsigned int vmonitor_5;

//				    vmonitor_5=VMIN + (Counter_3*100);

				    vmonitor_5 = VMIN + VRES + (Counter_3*100);

				    if(vmonitor_5>VMAX){

				    	vmonitor_5=VMAX;

				    }


				    Set_V(vmonitor_5); // Vmonitor by using the external circuit

					t=1;

					t = (P2IN & 0x0001);

					if(t==0){

						*FLAG_interrupt=4;

						P2LO();

						P3OUT &= ~BIT5;
						__bis_SR_register(LPM4_bits + GIE);        // LPM0, ADC10_ISR will force exit
						__no_operation();

					   }


			 }

		 }

		 if (*FLAG_interrupt==5 && pro==0){

		 			P1DIR |= BIT4;
		 			P1OUT |= BIT4;

		 			P2IFG = 0;
		 			P2IE &= ~BIT0;
		 			P2OUT &=~(BIT6);


		 			unsigned int vmonitor_8;

		 			vmonitor_8 =VMINN + deltaV;

		 			Set_V(vmonitor_8); // Vmonitor by using the external circuit

		 			unsigned int vmonitor_9;

		 			vmonitor_9 = VMIN + VRES + (Counter_3*100);

		 			if(vmonitor_9>VMAX){

		 				vmonitor_9=VMAX;

		 			}
		 			vmonitor_9=vmonitor_9/100;//otherwise numbers are too big
		 			n = (vmonitor_9*8)/10; //10/13Vcc > 2.5 * n/32

		 			//for(i = 0; i < n; i++);
		 			//n=15; // for debug
		 			CDCTL2 |= ref0[(unsigned int)n];

		 			*FLAG_interrupt=2;

		 			P2HI();

		 			P1OUT &= ~(BIT4);

		 			P1DIR |= BIT4;
		 			P1OUT |= BIT4;

		 			CDINT &= ~(CDIFG + CDIIFG);
		 			CDCTL1 |= CDON;
		 			CDINT |= CDIE;


		 			P1OUT &= ~(BIT4);
		 			__bis_SR_register(LPM4_bits + GIE);        // LPM0, ADC10_ISR will force exit
		 			__no_operation();

		 		}

//		        *FLAG_interrupt==2;

		 if (*FLAG_interrupt==4){

			 *FLAG_interrupt=2;
			 unsigned int value_7 = (unsigned int) (VMINN + deltaV);
			 if (PN==1){
				 value_7=value_7+sleepV;
				 *FLAG_interrupt=5;

			 }
//		   unsigned long int value_4 = VMIN2; //for testing

		   Set_V(value_7);

		   P2HI();

//		   StartDebounceTimer_2();

          // P2DIR |= BIT6;
          //P2OUT |= BIT6; // LED  ON

		   __bis_SR_register(GIE);        // LPM0, ADC10_ISR will force exit
		   __no_operation();

//		   break;

		 }

		P2DIR |= BIT6;
		P2OUT |= BIT6;
		pro=0;
		saved=0;
		P1OUT &= ~ BIT4;
		__bic_SR_register_on_exit(LPM4_bits); // Exit LPM4



         break;

         default:
         break;
      }

  }


/**********************************************************************//**
 * @brief Timer A1 ISR for debounce Timer
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR(void)
{
  TA1CCTL0 = 0;  // Disable Interrupt
  TA1CTL = 0;    // Disable Timer

  // stop also the interrupt related to the threshold

  P2IFG = 0;
  P2IE &= ~BIT0;
  saved=0;

  __bic_SR_register_on_exit(LPM4_bits); // Clear CPUOFF bit from 0 (SR)

}

#pragma vector=COMP_D_VECTOR
__interrupt void COMP_D_ISR(void)
{

	//P2DIR |= BIT0;
	//P2OUT |= BIT0;
	P2IFG = 0;
	P2IE &= ~BIT0;

	CDCTL1 &= ~(CDON); //disable comarator
	CDINT &= ~(CDIFG + CDIIFG + CDIE); //clean flags and disable iterrupt

	unsigned int value_7 = (unsigned int) (VMINN + deltaV+sleepV);

	*FLAG_interrupt=5;

	Set_V(value_7);

	P2HI();

	P2DIR |= BIT6;
	P2OUT |= BIT6; // LED  ON

	__bis_SR_register(GIE);        // LPM0, ADC10_ISR will force exit
	__no_operation();

	pro=0;

	__bic_SR_register_on_exit(LPM4_bits); // Clear CPUOFF bit from 0 (SR)

	//P2OUT &= ~BIT0;

}
