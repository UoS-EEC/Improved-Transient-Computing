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

#define FRAM2_START 0xEDF0
#define FRAM2_END 0xFF40
#define RAM_START 0x1C00
#define RAM_END 0x2000//2400

#define RAM_LOC 0x1C00
#define RAM_BACKUP_LOC 0x20000
#define RAM_SIZE 1024   //  for dma,


#define FRAM_START 0xEDF0
#define FRAM_START2 0xEE30
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

//Current profile

//#define VMIN 2300
//#define VMINN 2000
//#define VMAX 3600

//debugger profile

//#define VMIN 3400
//#define VMINN 3400
//#define VMAX 3600


// All Stack

#define PC 0xEDF0


#define V_THRESH 2200 // 2.2 volts

#define R_BACKUP_LOC (FRAM_START2 + RAM_SIZE)
#define R_LOC 0x100
#define R_SIZE 0x30

#define R_BACKUP_LOC2 (R_BACKUP_LOC + R_SIZE)
#define R_LOC2 0x140
#define R_SIZE2 0x72

#define R_BACKUP_LOC3 (R_BACKUP_LOC2 + R_SIZE2)
#define R_LOC3 0x200
#define R_SIZE3 0x40

#define R_BACKUP_LOC4 (R_BACKUP_LOC3 + R_SIZE3)
#define R_LOC4 0x320
#define R_SIZE4 0x150

#define R_BACKUP_LOC5 (R_BACKUP_LOC4 + R_SIZE4)
#define R_LOC5 0x4A0
#define R_SIZE5 0x160

#define R_BACKUP_LOC6 (R_BACKUP_LOC5 + R_SIZE5)
#define R_LOC6 0x640
#define R_SIZE6 0x30

#define R_BACKUP_LOC7 (R_BACKUP_LOC6 + R_SIZE6)
#define R_LOC7 0x700
#define R_SIZE7 0x20

#define R_BACKUP_LOC8 (R_BACKUP_LOC7 + R_SIZE7)
#define R_LOC8 0x8C0
#define R_SIZE8 0x10





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
extern void Restore_Comparator_State(void);
extern void Set_V(unsigned int);
extern void Calibration(void);
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
extern void P2HI(void);
extern void P2LO(void);


extern volatile unsigned long int ADCResult = 0;
