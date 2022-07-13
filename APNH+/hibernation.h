#include <stdio.h>

// The FRAM section from 0xD000 - 0xF000 is used by all modes
// for performing writes to FRAM
// Do not use this section for code or data placement.
// It will get overwritten!

#define FLAG_3 0xEDC0
#define COUNT_3 0xEDC4
#define COUNT_2 0xEDC8
#define FLAG_2 0xEDCC
#define VMON_3 0xEDD0
#define VMON_2 0xEDD4
#define INT 0xEDD8
#define CHECK 0xEDDC
#define CALIBR 0xEDE0
#define COUNT 0xEDE4
#define FLAG 0xEDE8
#define VMON 0xEDEC


#define FRAM_START 0xEDF0
#define FRAM_END 0xFF40

// All RAM

#define RAM_START 0x1C00
#define RAM_END 0x2000 //All RAM

// Test

#define FRAM_TEST_START 0xD400
#define FRAM_TEST_END 0xE155
#define MEM_UNIT 0x200

//Voltage profile

#define VMIN 2000
#define VMINN 1880
#define VMAX 3600
#define VRES 600

//Current profile

//#define VMIN 2300
//#define VMINN 2000
//#define VMAX 3600
//#define VRES 500

//debugger profile

//#define VMIN 3400
//#define VMINN 3400
//#define VMAX 3600
//#define VRES 100


// All Stack

#define PC 0xEDF0


#define V_THRESH 2200 // 2.2 volts


// Function Declarations
extern void SystemInit(void);
extern void StartUpSequence(void);
extern void LongDelay(void);
extern void CheckPoint (void);
extern void VoltageCheck (void);
extern void Restore(void);
extern void Save_RAM (void);
extern void Save_Register (void);
extern void Restore_RAM (void);
extern void Restore_Register (void);
extern void StartDebounceTimer(void);
extern void StartDebounceTimer_2(void);
extern void Set_V(unsigned int);
extern void Calibration(void);
extern void SCalibration(void);
extern void Vmonitor(void);
extern void Vmonitor_2(void);
extern unsigned int Calibrate(void);
extern void ADCSetup(void);
extern void test(void);
extern void CheckPoint_calibrate (void);
extern void FRAM_Read(unsigned int StartAddress);
extern void GPIO_configuration(void);
extern void Clock_configuration(void);
extern void Test(void);
extern void Set_internal_comparator(void);
extern void P2LO(void);
extern void P2HI(void);

extern volatile unsigned long int ADCResult = 0;
