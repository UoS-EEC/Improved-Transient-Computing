#include <msp430.h> 
#include "MyFFT.h"
#include "MyFFT_TestHarness.h"


/*
 * main.c
 */

#pragma SET_DATA_SECTION(".fram_vars")
unsigned int CC_check;

extern unsigned long int *FRAM_write_ptr;

//This pointer is used to copy the RAM
extern unsigned long int *RAM_copy_ptr; // define a pointer for memory copy

int counter=0;

//These pointers and variable are used to set the PC
extern unsigned long int *FRAM_pc;
extern unsigned long int* current_SP;
#pragma SET_DATA_SECTION()

unsigned int count=0;
unsigned int l=0;

#define CC 0xEFF6

long int j;

#define MyFFT_TEST_BUFFER_LENGTH		MyFFT_INPUT_LENGTH

MyFFT_Workspace_t MyFFT_TestBuffer[MyFFT_TEST_BUFFER_LENGTH];

volatile static int ErrorCount=0; // Note. static used for easier debugging

long int g=0;

















int main(void) {

	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	P3DIR |= BIT2;
	P3OUT |= BIT2; // LED  ON

//	 while(1);

	//This Pin P1.4 is not working properly. don't use.

   // P1OUT &= ~(BIT6);
   // P1DIR |= BIT6;

    //MCU ON

    //Restore Function


    volatile int i=0;
    volatile int j=0;

    SystemInit();


    while(1){

       	//For Debugging: System active
    	P2DIR |= BIT6;
       	P2OUT |= BIT6;

       	for(j=0;j<3;j++){

       	volatile static int ErrorCount=0; // Note. static used for easier debugging

       	for(i=0; i<MyFFT_TEST_BUFFER_LENGTH; i++)
       	{
				MyFFT_TestBuffer[i]=MyFFT_SampledR[i]; // fill with known data
			}

			MyFFT_Init(MyFFT_TestBuffer, MyFFT_TEST_BUFFER_LENGTH); // setup FFT input range

			MyFFT_CalculateDft(MyFFT_TestBuffer+0); // Calculate DFT at zero buffer offset

			ErrorCount=MyFFT_TestHarnessCompareTransformed();
			if(ErrorCount)
			{
				ErrorCount=ErrorCount; // set a break point here to check for errors
			}
       	}
       	//For Debugging: System active

       	P2OUT &=~(BIT6);

       	j=0;

       	  	   while(j<1000){
       	  	     	j++;
       	  	     }


       }
   }







/*#include <msp430.h>
#include "MyFFT.h"
#include "MyFFT_TestHarness.h"




#pragma SET_DATA_SECTION(".fram_vars")
unsigned int CC_check;

extern unsigned long int *FRAM_write_ptr;

//This pointer is used to copy the RAM
extern unsigned long int *RAM_copy_ptr; // define a pointer for memory copy

int counter=0;

//These pointers and variable are used to set the PC
extern unsigned long int *FRAM_pc;
extern unsigned long int* current_SP;
#pragma SET_DATA_SECTION()

unsigned int count=0;
unsigned int l=0;

#define CC 0xEFF6

long int j;

#define MyFFT_TEST_BUFFER_LENGTH		MyFFT_INPUT_LENGTH

MyFFT_Workspace_t MyFFT_TestBuffer[MyFFT_TEST_BUFFER_LENGTH];

volatile static int ErrorCount=0; // Note. static used for easier debugging

long int g=0;

 int main(void) {

	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	P3DIR |= BIT2;
	P3OUT |= BIT2; // LED  ON

//	 while(1);

	//This Pin P1.4 is not working properly. don't use.

    P1OUT &= ~(BIT4);
    P1DIR |= BIT4;

    //MCU ON

    //Restore Function


    volatile int i=0;

    SystemInit();


    while(1){


    	P2DIR |= BIT6;
    	P2OUT |= BIT6; // LED  ON





    	//Main() is completed

    	P2OUT &= ~(BIT6); // LED  OFF

       j=0;

  	   while(j<1000){
  	     	j++;
  	     }



    }


}
*/

