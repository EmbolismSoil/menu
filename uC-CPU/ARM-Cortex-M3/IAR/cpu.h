/*
*********************************************************************************************************
*                                               uC/CPU
*                                    CPU CONFIGURATION & PORT LAYER
*
*                          (c) Copyright 2004-2008; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*
*               uC/CPU is provided in source form for FREE evaluation, for educational
*               use or peaceful research.  If you plan on using uC/CPU in a commercial
*               product you need to contact Micrium to properly license its use in your
*               product.  We provide ALL the source code for your convenience and to
*               help you experience uC/CPU.  The fact that the source code is provided
*               does NOT mean that you can use it without paying a licensing fee.
*
*               Knowledge of the source code may NOT be used to develop a similar product.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            CPU PORT FILE
*
*                                            ARM-Cortex-M3
*                                            IAR C Compiler
*
* Filename      : cpu.h
* Version       : V1.19
* Programmer(s) : JJL
*                 BAN
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  CPU_CFG_MODULE_PRESENT
#define  CPU_CFG_MODULE_PRESENT


/*
*********************************************************************************************************
*                                          CPU INCLUDE FILES
*
* Note(s) : (1) The following CPU files are located in the following directories :
*
*               (a) \<CPU-Compiler Directory>\cpu_def.h
*
*               (b) \<CPU-Compiler Directory>\<cpu>\<compiler>\cpu*.*
*
*                       where
*                               <CPU-Compiler Directory>    directory path for common   CPU-compiler software
*                               <cpu>                       directory name for specific CPU
*                               <compiler>                  directory name for specific compiler
*
*           (2) Compiler MUST be configured to include the '\<CPU-Compiler Directory>\' directory & the
*               specific CPU-compiler directory as additional include path directories.
*********************************************************************************************************
*/

#include  <cpu_def.h>


/*$PAGE*/
/*
*********************************************************************************************************
*                                    CONFIGURE STANDARD DATA TYPES
*
* Note(s) : (1) Configure standard data types according to CPU-/compiler-specifications.
*
*           (2) (a) (1) 'CPU_FNCT_VOID' data type defined to replace the commonly-used function pointer
*                       data type of a pointer to a function which returns void & has no arguments.
*
*                   (2) Example function pointer usage :
*
*                           CPU_FNCT_VOID  FnctName;
*
*                           FnctName();
*
*               (b) (1) 'CPU_FNCT_PTR'  data type defined to replace the commonly-used function pointer
*                       data type of a pointer to a function which returns void & has a single void
*                       pointer argument.
*
*                   (2) Example function pointer usage :
*
*                           CPU_FNCT_PTR   FnctName;
*                           void          *pobj
*
*                           FnctName(pobj);
*********************************************************************************************************
*/

typedef            void       CPU_VOID;
typedef  unsigned  char       CPU_CHAR;                         /*  8-bit character                                     */
typedef  unsigned  char       CPU_BOOLEAN;                      /*  8-bit boolean or logical                            */
typedef  unsigned  char       CPU_INT08U;                       /*  8-bit unsigned integer                              */
typedef    signed  char       CPU_INT08S;                       /*  8-bit   signed integer                              */
typedef  unsigned  short      CPU_INT16U;                       /* 16-bit unsigned integer                              */
typedef    signed  short      CPU_INT16S;                       /* 16-bit   signed integer                              */
typedef  unsigned  int        CPU_INT32U;                       /* 32-bit unsigned integer                              */
typedef    signed  int        CPU_INT32S;                       /* 32-bit   signed integer                              */
typedef  unsigned  long long  CPU_INT64U;                       /* 64-bit unsigned integer                              */
typedef    signed  long long  CPU_INT64S;                       /* 64-bit   signed integer                              */

typedef            float      CPU_FP32;                         /* 32-bit floating point                                */
typedef            double     CPU_FP64;                         /* 64-bit floating point                                */


typedef            void     (*CPU_FNCT_VOID)(void);             /* See Note #2a.                                        */
typedef            void     (*CPU_FNCT_PTR )(void *);           /* See Note #2b.                                        */


/*$PAGE*/
/*
*********************************************************************************************************
*                                       CPU WORD CONFIGURATION
*
* Note(s) : (1) Configure CPU_CFG_ADDR_SIZE & CPU_CFG_DATA_SIZE with CPU's word sizes :
*
*                   CPU_WORD_SIZE_08             8-bit word size
*                   CPU_WORD_SIZE_16            16-bit word size
*                   CPU_WORD_SIZE_32            32-bit word size
*                   CPU_WORD_SIZE_64            64-bit word size            See Note #1a
*
*               (a) 64-bit word size NOT currently supported.
*
*           (2) Configure CPU_CFG_ENDIAN_TYPE with CPU's data-word-memory order :
*
*                   CPU_ENDIAN_TYPE_BIG         Big-   endian word order (CPU words' most  significant
*                                                                         octet @ lowest memory address)
*                   CPU_ENDIAN_TYPE_LITTLE      Little-endian word order (CPU words' least significant
*                                                                         octet @ lowest memory address)
*********************************************************************************************************
*/

                                                                /* Define  CPU         word sizes (see Note #1) :       */
#define  CPU_CFG_ADDR_SIZE              CPU_WORD_SIZE_32        /* Defines CPU address word size.                       */

#define  CPU_CFG_DATA_SIZE              CPU_WORD_SIZE_32        /* Defines CPU data    word size.                       */
#define  CPU_CFG_ENDIAN_TYPE            CPU_ENDIAN_TYPE_LITTLE  /* Defines CPU data    word-memory order.               */


/*
*********************************************************************************************************
*                                 CONFIGURE CPU ADDRESS & DATA TYPES
*********************************************************************************************************
*/

                                                                /* CPU address type based on address bus size.          */
#if     (CPU_CFG_ADDR_SIZE == CPU_WORD_SIZE_32)
typedef  CPU_INT32U  CPU_ADDR;
#elif   (CPU_CFG_ADDR_SIZE == CPU_WORD_SIZE_16)
typedef  CPU_INT16U  CPU_ADDR;
#else
typedef  CPU_INT08U  CPU_ADDR;
#endif

                                                                /* CPU data    type based on data    bus size.          */
#if     (CPU_CFG_DATA_SIZE == CPU_WORD_SIZE_32)
typedef  CPU_INT32U  CPU_DATA;
#elif   (CPU_CFG_DATA_SIZE == CPU_WORD_SIZE_16)
typedef  CPU_INT16U  CPU_DATA;
#else
typedef  CPU_INT08U  CPU_DATA;
#endif


typedef  CPU_DATA    CPU_ALIGN;                                 /* Defines CPU data-word-alignment size.                */
typedef  CPU_DATA    CPU_SIZE_T;                                /* Defines CPU standard 'size_t'   size.                */


/*$PAGE*/
/*
*********************************************************************************************************
*                                   CRITICAL SECTION CONFIGURATION
*
* Note(s) : (1) Configure CPU_CFG_CRITICAL_METHOD with CPU's/compiler's critical section method :
*
*                                                       Enter/Exit critical sections by ...
*
*                   CPU_CRITICAL_METHOD_INT_DIS_EN      Disable/Enable interrupts
*                   CPU_CRITICAL_METHOD_STATUS_STK      Push/Pop       interrupt status onto stack
*                   CPU_CRITICAL_METHOD_STATUS_LOCAL    Save/Restore   interrupt status to local variable
*
*               (a) CPU_CRITICAL_METHOD_INT_DIS_EN  is NOT a preferred method since it does NOT support
*                   multiple levels of interrupts.  However, with some CPUs/compilers, this is the only
*                   available method.
*
*               (b) CPU_CRITICAL_METHOD_STATUS_STK    is one preferred method since it DOES support multiple
*                   levels of interrupts.  However, this method assumes that the compiler allows in-line
*                   assembly AND will correctly modify the local stack pointer when interrupt status is
*                   pushed/popped onto the stack.
*
*               (c) CPU_CRITICAL_METHOD_STATUS_LOCAL  is one preferred method since it DOES support multiple
*                   levels of interrupts.  However, this method assumes that the compiler provides C-level
*                   &/or assembly-level functionality for the following :
*
*                     ENTER CRITICAL SECTION :
*                       (a) Save    interrupt status into a local variable
*                       (b) Disable interrupts
*
*                     EXIT  CRITICAL SECTION :
*                       (c) Restore interrupt status from a local variable
*
*           (2) Critical section macro's most likely require inline assembly.  If the compiler does NOT
*               allow inline assembly in C source files, critical section macro's MUST call an assembly
*               subroutine defined in a 'cpu_a.asm' file located in the following software directory :
*
*                   \<CPU-Compiler Directory>\<cpu>\<compiler>\
*
*                       where
*                               <CPU-Compiler Directory>    directory path for common   CPU-compiler software
*                               <cpu>                       directory name for specific CPU
*                               <compiler>                  directory name for specific compiler
*
*           (3) To save/restore interrupt status, a local variable 'cpu_sr' of type 'CPU_SR' MAY need to
*               be declared (e.g. if 'CPU_CRITICAL_METHOD_STATUS_LOCAL' method is configured).  Configure
*               'CPU_SR' data type with the appropriate-sized CPU data type large enough to completely
*               store the CPU's/compiler's status word.
*********************************************************************************************************
*/

typedef  CPU_INT32U  CPU_SR;                                    /* Defines   CPU status register size (see Note #3).    */

                                                                /* Configure CPU critical method      (see Note #1) :   */
#define  CPU_CFG_CRITICAL_METHOD        CPU_CRITICAL_METHOD_STATUS_LOCAL

#define  CPU_CRITICAL_ENTER()           { cpu_sr = CPU_SR_Save(); }
#define  CPU_CRITICAL_EXIT()            { CPU_SR_Restore(cpu_sr); }


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void        CPU_IntDis       (void);
void        CPU_IntEn        (void);

void        CPU_IntSrcDis    (CPU_INT08U  pos);
void        CPU_IntSrcEn     (CPU_INT08U  pos);
CPU_INT16S  CPU_IntSrcPrioGet(CPU_INT08U  pos);
void        CPU_IntSrcPrioSet(CPU_INT08U  pos,
                              CPU_INT08U  prio);

CPU_SR      CPU_SR_Save      (void);
void        CPU_SR_Restore   (CPU_SR      cpu_sr);

CPU_INT32U  CPU_CntLeadZeros (CPU_INT32U  val);
CPU_INT32U  CPU_RevBits      (CPU_INT32U  val);

void        CPU_WaitForInt   (void);
void        CPU_WaitForExcept(void);

void        CPU_BitBandClr   (CPU_ADDR    addr,
                              CPU_INT08U  bit_nbr);
void        CPU_BitBandSet   (CPU_ADDR    addr,
                              CPU_INT08U  bit_nbr);

/*
*********************************************************************************************************
*                                           INTERRUPT SOURCES
*********************************************************************************************************
*/

#define  CPU_INT_STK_PTR                                   0
#define  CPU_INT_RESET                                     1
#define  CPU_INT_NMI                                       2
#define  CPU_INT_HFAULT                                    3
#define  CPU_INT_MEM                                       4
#define  CPU_INT_BUSFAULT                                  5
#define  CPU_INT_USAGEFAULT                                6
#define  CPU_INT_RSVD_07                                   7
#define  CPU_INT_RSVD_08                                   8
#define  CPU_INT_RSVD_09                                   9
#define  CPU_INT_RSVD_10                                  10
#define  CPU_INT_SVCALL                                   11
#define  CPU_INT_DBGMON                                   12
#define  CPU_INT_RSVD_13                                  13
#define  CPU_INT_PENDSV                                   14
#define  CPU_INT_SYSTICK                                  15


/*
*********************************************************************************************************
*                                             CPU REGISTERS
*********************************************************************************************************
*/

#define  CPU_REG_NVIC_NVIC              (*((volatile CPU_INT32U *)(0xE000E004))) /* Int Ctrl'er Type Reg.               */
#define  CPU_REG_NVIC_ST_CTRL           (*((volatile CPU_INT32U *)(0xE000E010))) /* SysTick Ctrl & Status Reg.          */
#define  CPU_REG_NVIC_ST_RELOAD         (*((volatile CPU_INT32U *)(0xE000E014))) /* SysTick Reload      Value Reg.      */
#define  CPU_REG_NVIC_ST_CURRENT        (*((volatile CPU_INT32U *)(0xE000E018))) /* SysTick Current     Value Reg.      */
#define  CPU_REG_NVIC_ST_CAL            (*((volatile CPU_INT32U *)(0xE000E01C))) /* SysTick Calibration Value Reg.      */
                                                                                 /* IRQ Set En Reg.                     */
#define  CPU_REG_NVIC_SETEN(n)          (*((volatile CPU_INT32U *)(0xE000E100 + (n) * 4)))
                                                                                 /* IRQ Clr En Reg.                     */
#define  CPU_REG_NVIC_CLREN(n)          (*((volatile CPU_INT32U *)(0xE000E180 + (n) * 4)))
                                                                                 /* IRQ Set Pending Reg.                */
#define  CPU_REG_NVIC_SETPEND(n)        (*((volatile CPU_INT32U *)(0xE000E200 + (n) * 4)))
                                                                                 /* IRQ Clr Pending Reg.                */
#define  CPU_REG_NVIC_CLRPEND(n)        (*((volatile CPU_INT32U *)(0xE000E280 + (n) * 4)))
                                                                                 /* IRQ Active Reg.                     */
#define  CPU_REG_NVIC_ACTIVE(n)         (*((volatile CPU_INT32U *)(0xE000E300 + (n) * 4)))
                                                                                 /* IRQ Prio Reg.                       */
#define  CPU_REG_NVIC_PRIO(n)           (*((volatile CPU_INT32U *)(0xE000E400 + (n) * 4)))

#define  CPU_REG_NVIC_CPUID             (*((volatile CPU_INT32U *)(0xE000ED00))) /* CPUID Base Reg.                     */
#define  CPU_REG_NVIC_ICSR              (*((volatile CPU_INT32U *)(0xE000ED04))) /* Int Ctrl State  Reg.                */
#define  CPU_REG_NVIC_VTOR              (*((volatile CPU_INT32U *)(0xE000ED08))) /* Vect Tbl Offset Reg.                */
#define  CPU_REG_NVIC_AIRCR             (*((volatile CPU_INT32U *)(0xE000ED0C))) /* App Int/Reset Ctrl Reg.             */
#define  CPU_REG_NVIC_SCR               (*((volatile CPU_INT32U *)(0xE000ED10))) /* System Ctrl Reg.                    */
#define  CPU_REG_NVIC_CCR               (*((volatile CPU_INT32U *)(0xE000ED14))) /* Cfg    Ctrl Reg.                    */
#define  CPU_REG_NVIC_SHPRI1            (*((volatile CPU_INT32U *)(0xE000ED18))) /* System Handlers  4 to  7 Prio.      */
#define  CPU_REG_NVIC_SHPRI2            (*((volatile CPU_INT32U *)(0xE000ED1C))) /* System Handlers  8 to 11 Prio.      */
#define  CPU_REG_NVIC_SHPRI3            (*((volatile CPU_INT32U *)(0xE000ED20))) /* System Handlers 12 to 15 Prio.      */
#define  CPU_REG_NVIC_SHCSR             (*((volatile CPU_INT32U *)(0xE000ED24))) /* System Handler Ctrl & State Reg.    */
#define  CPU_REG_NVIC_CFSR              (*((volatile CPU_INT32U *)(0xE000ED28))) /* Configurable Fault Status Reg.      */
#define  CPU_REG_NVIC_HFSR              (*((volatile CPU_INT32U *)(0xE000ED2C))) /* Hard  Fault Status Reg.             */
#define  CPU_REG_NVIC_DFSR              (*((volatile CPU_INT32U *)(0xE000ED30))) /* Debug Fault Status Reg.             */
#define  CPU_REG_NVIC_MMFAR             (*((volatile CPU_INT32U *)(0xE000ED34))) /* Mem Manage Addr Reg.                */
#define  CPU_REG_NVIC_BFAR              (*((volatile CPU_INT32U *)(0xE000ED38))) /* Bus Fault  Addr Reg.                */
#define  CPU_REG_NVIC_AFSR              (*((volatile CPU_INT32U *)(0xE000ED3C))) /* Aux Fault Status Reg.               */

#define  CPU_REG_NVIC_PFR0              (*((volatile CPU_INT32U *)(0xE000ED40))) /* Processor Feature Reg 0.            */
#define  CPU_REG_NVIC_PFR1              (*((volatile CPU_INT32U *)(0xE000ED44))) /* Processor Feature Reg 1.            */
#define  CPU_REG_NVIC_DFR0              (*((volatile CPU_INT32U *)(0xE000ED48))) /* Debug     Feature Reg 0.            */
#define  CPU_REG_NVIC_AFR0              (*((volatile CPU_INT32U *)(0xE000ED4C))) /* Aux       Feature Reg 0.            */
#define  CPU_REG_NVIC_MMFR0             (*((volatile CPU_INT32U *)(0xE000ED50))) /* Memory Model Feature Reg 0.         */
#define  CPU_REG_NVIC_MMFR1             (*((volatile CPU_INT32U *)(0xE000ED54))) /* Memory Model Feature Reg 1.         */
#define  CPU_REG_NVIC_MMFR2             (*((volatile CPU_INT32U *)(0xE000ED58))) /* Memory Model Feature Reg 2.         */
#define  CPU_REG_NVIC_MMFR3             (*((volatile CPU_INT32U *)(0xE000ED5C))) /* Memory Model Feature Reg 3.         */
#define  CPU_REG_NVIC_ISAFR0            (*((volatile CPU_INT32U *)(0xE000ED60))) /* ISA Feature Reg 0.                  */
#define  CPU_REG_NVIC_ISAFR1            (*((volatile CPU_INT32U *)(0xE000ED64))) /* ISA Feature Reg 1.                  */
#define  CPU_REG_NVIC_ISAFR2            (*((volatile CPU_INT32U *)(0xE000ED68))) /* ISA Feature Reg 2.                  */
#define  CPU_REG_NVIC_ISAFR3            (*((volatile CPU_INT32U *)(0xE000ED6C))) /* ISA Feature Reg 3.                  */
#define  CPU_REG_NVIC_ISAFR4            (*((volatile CPU_INT32U *)(0xE000ED70))) /* ISA Feature Reg 4.                  */
#define  CPU_REG_NVIC_SW_TRIG           (*((volatile CPU_INT32U *)(0xE000EF00))) /* Software Trigger Int Reg.           */

#define  CPU_REG_MPU_TYPE               (*((volatile CPU_INT32U *)(0xE000ED90))) /* MPU Type Reg.                       */
#define  CPU_REG_MPU_CTRL               (*((volatile CPU_INT32U *)(0xE000ED94))) /* MPU Ctrl Reg.                       */
#define  CPU_REG_MPU_REG_NBR            (*((volatile CPU_INT32U *)(0xE000ED98))) /* MPU Region Nbr Reg.                 */
#define  CPU_REG_MPU_REG_BASE           (*((volatile CPU_INT32U *)(0xE000ED9C))) /* MPU Region Base Addr Reg.           */
#define  CPU_REG_MPU_REG_ATTR           (*((volatile CPU_INT32U *)(0xE000EDA0))) /* MPU Region Attrib & Size Reg.       */

#define  CPU_REG_DBG_CTRL               (*((volatile CPU_INT32U *)(0xE000EDF0))) /* Debug Halting Ctrl & Status Reg.    */
#define  CPU_REG_DBG_SELECT             (*((volatile CPU_INT32U *)(0xE000EDF4))) /* Debug Core Reg Selector Reg.        */
#define  CPU_REG_DBG_DATA               (*((volatile CPU_INT32U *)(0xE000EDF8))) /* Debug Core Reg Data     Reg.        */
#define  CPU_REG_DBG_INT                (*((volatile CPU_INT32U *)(0xE000EDFC))) /* Debug Except & Monitor Ctrl Reg.    */

/*
*********************************************************************************************************
*                                          CPU REGISTER BITS
*********************************************************************************************************
*/

                                                                /* ---------- SYSTICK CTRL & STATUS REG BITS ---------- */
#define  CPU_REG_NVIC_ST_CTRL_COUNTFLAG         DEF_BIT_16
#define  CPU_REG_NVIC_ST_CTRL_CLKSOURCE         DEF_BIT_02
#define  CPU_REG_NVIC_ST_CTRL_TICKINT           DEF_BIT_01
#define  CPU_REG_NVIC_ST_CTRL_ENABLE            DEF_BIT_00


                                                                /* -------- SYSTICK CALIBRATION VALUE REG BITS -------- */
#define  CPU_REG_NVIC_ST_CAL_NOREF              DEF_BIT_31
#define  CPU_REG_NVIC_ST_CAL_SKEW               DEF_BIT_30

                                                                /* -------------- INT CTRL STATE REG BITS ------------- */
#define  CPU_REG_NVIC_ICSR_NMIPENDSET           DEF_BIT_31
#define  CPU_REG_NVIC_ICSR_PENDSVSET            DEF_BIT_28
#define  CPU_REG_NVIC_ICSR_PENDSVCLR            DEF_BIT_27
#define  CPU_REG_NVIC_ICSR_PENDSTSET            DEF_BIT_26
#define  CPU_REG_NVIC_ICSR_PENDSTCLR            DEF_BIT_25
#define  CPU_REG_NVIC_ICSR_ISRPREEMPT           DEF_BIT_23
#define  CPU_REG_NVIC_ICSR_ISRPENDING           DEF_BIT_22
#define  CPU_REG_NVIC_ICSR_RETTOBASE            DEF_BIT_11

                                                                /* ------------- VECT TBL OFFSET REG BITS ------------- */
#define  CPU_REG_NVIC_VTOR_TBLBASE              DEF_BIT_29

                                                                /* ------------ APP INT/RESET CTRL REG BITS ----------- */
#define  CPU_REG_NVIC_AIRCR_ENDIANNESS          DEF_BIT_15
#define  CPU_REG_NVIC_AIRCR_SYSRESETREQ         DEF_BIT_02
#define  CPU_REG_NVIC_AIRCR_VECTCLRACTIVE       DEF_BIT_01
#define  CPU_REG_NVIC_AIRCR_VECTRESET           DEF_BIT_00

                                                                /* --------------- SYSTEM CTRL REG BITS --------------- */
#define  CPU_REG_NVIC_SCR_SEVONPEND             DEF_BIT_04
#define  CPU_REG_NVIC_SCR_SLEEPDEEP             DEF_BIT_02
#define  CPU_REG_NVIC_SCR_SLEEPONEXIT           DEF_BIT_01

                                                                /* ----------------- CFG CTRL REG BITS ---------------- */
#define  CPU_REG_NVIC_CCR_STKALIGN              DEF_BIT_09
#define  CPU_REG_NVIC_CCR_BFHFNMIGN             DEF_BIT_08
#define  CPU_REG_NVIC_CCR_DIV_0_TRP             DEF_BIT_04
#define  CPU_REG_NVIC_CCR_UNALIGN_TRP           DEF_BIT_03
#define  CPU_REG_NVIC_CCR_USERSETMPEND          DEF_BIT_01
#define  CPU_REG_NVIC_CCR_NONBASETHRDENA        DEF_BIT_00

                                                                /* ------- SYSTEM HANDLER CTRL & STATE REG BITS ------- */
#define  CPU_REG_NVIC_SHCSR_USGFAULTENA         DEF_BIT_18
#define  CPU_REG_NVIC_SHCSR_BUSFAULTENA         DEF_BIT_17
#define  CPU_REG_NVIC_SHCSR_MEMFAULTENA         DEF_BIT_16
#define  CPU_REG_NVIC_SHCSR_SVCALLPENDED        DEF_BIT_15
#define  CPU_REG_NVIC_SHCSR_BUSFAULTPENDED      DEF_BIT_14
#define  CPU_REG_NVIC_SHCSR_MEMFAULTPENDED      DEF_BIT_13
#define  CPU_REG_NVIC_SHCSR_USGFAULTPENDED      DEF_BIT_12
#define  CPU_REG_NVIC_SHCSR_SYSTICKACT          DEF_BIT_11
#define  CPU_REG_NVIC_SHCSR_PENDSVACT           DEF_BIT_10
#define  CPU_REG_NVIC_SHCSR_MONITORACT          DEF_BIT_08
#define  CPU_REG_NVIC_SHCSR_SVCALLACT           DEF_BIT_07
#define  CPU_REG_NVIC_SHCSR_USGFAULTACT         DEF_BIT_03
#define  CPU_REG_NVIC_SHCSR_BUSFAULTACT         DEF_BIT_01
#define  CPU_REG_NVIC_SHCSR_MEMFAULTACT         DEF_BIT_00

                                                                /* -------- CONFIGURABLE FAULT STATUS REG BITS -------- */
#define  CPU_REG_NVIC_CFSR_DIVBYZERO            DEF_BIT_25
#define  CPU_REG_NVIC_CFSR_UNALIGNED            DEF_BIT_24
#define  CPU_REG_NVIC_CFSR_NOCP                 DEF_BIT_19
#define  CPU_REG_NVIC_CFSR_INVPC                DEF_BIT_18
#define  CPU_REG_NVIC_CFSR_INVSTATE             DEF_BIT_17
#define  CPU_REG_NVIC_CFSR_UNDEFINSTR           DEF_BIT_16
#define  CPU_REG_NVIC_CFSR_BFARVALID            DEF_BIT_15
#define  CPU_REG_NVIC_CFSR_STKERR               DEF_BIT_12
#define  CPU_REG_NVIC_CFSR_UNSTKERR             DEF_BIT_11
#define  CPU_REG_NVIC_CFSR_IMPRECISERR          DEF_BIT_10
#define  CPU_REG_NVIC_CFSR_PRECISERR            DEF_BIT_09
#define  CPU_REG_NVIC_CFSR_IBUSERR              DEF_BIT_08
#define  CPU_REG_NVIC_CFSR_MMARVALID            DEF_BIT_07
#define  CPU_REG_NVIC_CFSR_MSTKERR              DEF_BIT_04
#define  CPU_REG_NVIC_CFSR_MUNSTKERR            DEF_BIT_03
#define  CPU_REG_NVIC_CFSR_DACCVIOL             DEF_BIT_01
#define  CPU_REG_NVIC_CFSR_IACCVIOL             DEF_BIT_00

                                                                /* ------------ HARD FAULT STATUS REG BITS ------------ */
#define  CPU_REG_NVIC_HFSR_DEBUGEVT             DEF_BIT_31
#define  CPU_REG_NVIC_HFSR_FORCED               DEF_BIT_30
#define  CPU_REG_NVIC_HFSR_VECTTBL              DEF_BIT_01

                                                                /* ------------ DEBUG FAULT STATUS REG BITS ----------- */
#define  CPU_REG_NVIC_DFSR_EXTERNAL             DEF_BIT_04
#define  CPU_REG_NVIC_DFSR_VCATCH               DEF_BIT_03
#define  CPU_REG_NVIC_DFSR_DWTTRAP              DEF_BIT_02
#define  CPU_REG_NVIC_DFSR_BKPT                 DEF_BIT_01
#define  CPU_REG_NVIC_DFSR_HALTED               DEF_BIT_00

/*$PAGE*/
/*
*********************************************************************************************************
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*/

#ifndef   CPU_CFG_ADDR_SIZE
#error   "CPU_CFG_ADDR_SIZE              not #define'd in 'cpu.h'               "
#error   "                         [MUST be  CPU_WORD_SIZE_08   8-bit alignment]"
#error   "                         [     ||  CPU_WORD_SIZE_16  16-bit alignment]"
#error   "                         [     ||  CPU_WORD_SIZE_32  32-bit alignment]"

#elif   ((CPU_CFG_ADDR_SIZE != CPU_WORD_SIZE_08) && \
         (CPU_CFG_ADDR_SIZE != CPU_WORD_SIZE_16) && \
         (CPU_CFG_ADDR_SIZE != CPU_WORD_SIZE_32))
#error   "CPU_CFG_ADDR_SIZE        illegally #define'd in 'cpu.h'               "
#error   "                         [MUST be  CPU_WORD_SIZE_08   8-bit alignment]"
#error   "                         [     ||  CPU_WORD_SIZE_16  16-bit alignment]"
#error   "                         [     ||  CPU_WORD_SIZE_32  32-bit alignment]"
#endif


#ifndef   CPU_CFG_DATA_SIZE
#error   "CPU_CFG_DATA_SIZE              not #define'd in 'cpu.h'               "
#error   "                         [MUST be  CPU_WORD_SIZE_08   8-bit alignment]"
#error   "                         [     ||  CPU_WORD_SIZE_16  16-bit alignment]"
#error   "                         [     ||  CPU_WORD_SIZE_32  32-bit alignment]"

#elif   ((CPU_CFG_DATA_SIZE != CPU_WORD_SIZE_08) && \
         (CPU_CFG_DATA_SIZE != CPU_WORD_SIZE_16) && \
         (CPU_CFG_DATA_SIZE != CPU_WORD_SIZE_32))
#error   "CPU_CFG_DATA_SIZE        illegally #define'd in 'cpu.h'               "
#error   "                         [MUST be  CPU_WORD_SIZE_08   8-bit alignment]"
#error   "                         [     ||  CPU_WORD_SIZE_16  16-bit alignment]"
#error   "                         [     ||  CPU_WORD_SIZE_32  32-bit alignment]"
#endif



#ifndef   CPU_CFG_ENDIAN_TYPE
#error   "CPU_CFG_ENDIAN_TYPE            not #define'd in 'cpu.h'   "
#error   "                         [MUST be  CPU_ENDIAN_TYPE_BIG   ]"
#error   "                         [     ||  CPU_ENDIAN_TYPE_LITTLE]"

#elif   ((CPU_CFG_ENDIAN_TYPE != CPU_ENDIAN_TYPE_BIG   ) && \
         (CPU_CFG_ENDIAN_TYPE != CPU_ENDIAN_TYPE_LITTLE))
#error   "CPU_CFG_ENDIAN_TYPE      illegally #define'd in 'cpu.h'   "
#error   "                         [MUST be  CPU_ENDIAN_TYPE_BIG   ]"
#error   "                         [     ||  CPU_ENDIAN_TYPE_LITTLE]"
#endif




#ifndef   CPU_CFG_CRITICAL_METHOD
#error   "CPU_CFG_CRITICAL_METHOD        not #define'd in 'cpu.h'             "
#error   "                         [MUST be  CPU_CRITICAL_METHOD_INT_DIS_EN  ]"
#error   "                         [     ||  CPU_CRITICAL_METHOD_STATUS_STK  ]"
#error   "                         [     ||  CPU_CRITICAL_METHOD_STATUS_LOCAL]"

#elif   ((CPU_CFG_CRITICAL_METHOD != CPU_CRITICAL_METHOD_INT_DIS_EN  ) && \
         (CPU_CFG_CRITICAL_METHOD != CPU_CRITICAL_METHOD_STATUS_STK  ) && \
         (CPU_CFG_CRITICAL_METHOD != CPU_CRITICAL_METHOD_STATUS_LOCAL))
#error   "CPU_CFG_CRITICAL_METHOD  illegally #define'd in 'cpu.h'             "
#error   "                         [MUST be  CPU_CRITICAL_METHOD_INT_DIS_EN  ]"
#error   "                         [     ||  CPU_CRITICAL_METHOD_STATUS_STK  ]"
#error   "                         [     ||  CPU_CRITICAL_METHOD_STATUS_LOCAL]"
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of CPU cfg module inclusion.                     */
