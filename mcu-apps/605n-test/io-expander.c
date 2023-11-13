
#include "io-expander.h"

#include "i2c.h"    // _I2C_H_

/*
    This code is for the PCAL6416AH io expanders on the rev 2021-02-02 base board

    i2c 7-bit address
        U2   010 0000  (addr sel pulled low  - see U2_address)
        U22  010 0001  (addr sel pulled high - see U22_address)

    WAKEUP_INT_N (interrupt output from both U2 and U22) ; this is a PSoC interrupt input

    command byte    register

        0            input port 0
        1            input port 1
        2           output port 0
        3           output port 1
        4           polarity inversion input port 0 (1 = invert)
        5           polarity inversion input port 1 (1 = invert)
        6           configuration port 0            (1 = input)
        7           configuration port 1            (1 = input)
     0x46           pullup/down enable              (1 = enable)
     0x47           pullup/down enable              (1 = enable)
     0x48           pullup/down selection           (1: 100K pullup  0: 100K pulldown)
     0x49           pullup/down selection           (1: 100K pullup  0: 100K pulldown)
*/


#define U2_address   0x20
#define U22_address  0x21

static const I2CDevice i2cDevice = 0 ;


typedef struct {
    uint16_t     SLOT3_EN                : 1 ;   // P0_0    output
    uint16_t     SLOT4_EN                : 1 ;   // P0_1    output
    uint16_t     MAG_SW_INT              : 1 ;   // P0_2     input
    uint16_t     SILEGO_INT              : 1 ;   // P0_3     input
    uint16_t     _3p3V_PG                : 1 ;   // P0_4    output
    uint16_t     _3p3V_EN                : 1 ;   // P0_5    output
    uint16_t     IRD_EN                  : 1 ;   // P0_6    output
    uint16_t     _5p0V_EN                : 1 ;   // P0_7    output
    uint16_t     SONIC_EN                : 1 ;   // P1_0    output
    uint16_t     EXT_I2C_VBATT_EN        : 1 ;   // P1_1    output
    uint16_t     GPS_RESET_N             : 1 ;   // P1_2    output
    uint16_t     GPS_EN_N                : 1 ;   // P1_3    output
    uint16_t     SLOT3_PRESENT           : 1 ;   // P1_4     input
    uint16_t     CHECK_SLOTS             : 1 ;   // P1_5    output
    uint16_t     TP8                     : 1 ;   // P1_6     input with weak pulldown
    uint16_t     SLOT4_PRESENT           : 1 ;   // P1_7     input
} ExpanderBits_U2 ;

typedef struct {
    uint16_t      CHECK_VBAT             : 1 ;   // P0_0    output
    uint16_t      AUXCHARGER_ACTIVE      : 1 ;   // P0_1     input
    uint16_t      AUXCHARGER_FAULT_N     : 1 ;   // P0_2     input
    uint16_t      BOARD_ID_BIT_2         : 1 ;   // P0_3     input
    uint16_t      BOARD_ID_BIT_1         : 1 ;   // P0_4     input
    uint16_t      BOARD_ID_BIT_0         : 1 ;   // P0_5     input
    uint16_t      CASE_OPEN              : 1 ;   // P0_6     input
    uint16_t      TP4                    : 1 ;   // P0_7    output
    uint16_t      TP5                    : 1 ;   // P1_0    output
    uint16_t      WIRELESS_CHARGER_EN1   : 1 ;   // P1_1    output
    uint16_t      WIRELESS_CHARGER_EN2   : 1 ;   // P1_2    output
    uint16_t      ACTIVITY_LED           : 1 ;   // P1_3    output
    uint16_t      BLE_LED                : 1 ;   // P1_4    output
    uint16_t      STATUS_LED             : 1 ;   // P1_5    output
    uint16_t      GYRO_INT1              : 1 ;   // P1_6     input
    uint16_t      GYRO_INT2              : 1 ;   // P1_7     input
} ExpanderBits_U22 ;

typedef union {
    ExpanderBits_U2   U2  ;
    ExpanderBits_U22  U22 ;
} ExpanderBits ;


typedef struct {
    uint8_t     i2cAddress ;
    uint16_t    outputCopy ;    // copy of whatever was written to the output port
} Expander ;

static Expander expander [NumberOfIOExpanders] = {
    { U2_address  ,  0 },
    { U22_address ,  0 }
} ;

#define Index_U2   0
#define Index_U22  1


typedef struct {
    ExpanderBits  inputPort ;           // read-only
    ExpanderBits outputPort ;           // read/write   default = all 1
    ExpanderBits  inputInversion ;      // read/write   default = all 0 ; 1 = invert input reading
    ExpanderBits configureAsInput ;     // read/write   default = all 1 ; 1 = configure bit as an input
} ExpanderRegisters ;
#define RegisterAddress_inputPort              0
#define RegisterAddress_outputPort             2
#define RegisterAddress_inputInversion         4
#define RegisterAddress_configureAsInput       6
#define RegisterAddress_enablePullUpDown    0x46
#define RegisterAddress_enablePullUp        0x48
#define RegisterAddress_interruptMask       0x4a


static void write (uint8_t _7BitAddress, uint8_t registerAddress, uint16_t bits)
{
    uint8_t i2cTxfr [3] ;
    i2cTxfr [0] = registerAddress ;
    i2cTxfr [1] = bits  & 0xff ;
    i2cTxfr [2] = bits >>    8 ;

    i2c_open (i2cDevice) ;

    I2CAddressNumBits addressBits = 7 ;
    bool stop = true ;

    i2c_startThenWrite (i2cDevice, addressBits, _7BitAddress, i2cTxfr, sizeof (i2cTxfr), stop) ;

    i2c_close (i2cDevice) ;
}


static uint16_t read (uint8_t _7BitAddress, uint8_t registerAddress)
{
    union {
        uint16_t  asWord ;
        uint8_t   asBytes [2] ;
    } data ;


    i2c_open (i2cDevice) ;

    I2CAddressNumBits addressBits = 7 ;
    bool stop = false ;
#if 1
         stop = true ;
#endif
    i2c_startThenWrite (i2cDevice, addressBits, _7BitAddress, & registerAddress, sizeof (registerAddress), stop) ;

    uint8_t * dataPtr    =         data.asBytes  ;
    uint8_t   dataLength = sizeof (data.asBytes) ;
    stop = true ;
    i2c_startThenRead (i2cDevice, addressBits, _7BitAddress, dataPtr, dataLength, stop) ;

    i2c_close (i2cDevice) ;

    return data.asWord ;
}



static void init_OutputLevels_U2 (void)
{
    Expander *           expanderPtr = & expander [Index_U2] ;
    uint8_t i2cAddress = expanderPtr -> i2cAddress ;

    union {
        ExpanderBits_U2  asBits ;
        uint16_t         asWord ;
    } data ;

    data.asWord = 0x0000 ;      // default levels are all 0

    data.asBits.EXT_I2C_VBATT_EN = 0 ;

    // the following are not set to the default of 0
    data.asBits. GPS_EN_N = 1 ;

    expanderPtr->outputCopy = data.asWord ;     // update the local copy

    write (i2cAddress, RegisterAddress_outputPort, data.asWord) ;
}



static void init_OutputLevels_U22 (void)
{
    Expander *           expanderPtr = & expander [Index_U22] ;
    uint8_t i2cAddress = expanderPtr -> i2cAddress ;

    union {
        ExpanderBits_U22  asBits ;
        uint16_t          asWord ;
    } data ;

    data.asWord = 0x0000 ;      // default levels are all 0

    data.asBits.WIRELESS_CHARGER_EN1 = 0 ;
    data.asBits.WIRELESS_CHARGER_EN2 = 0 ;

    // the following are not set to the default of 0
/// data.asBits. tbd = 1 ;     none

    expanderPtr->outputCopy = data.asWord ;    // update the local copy

    write (i2cAddress, RegisterAddress_outputPort, data.asWord) ;
}



static void init_InputsOutputs_U2 (void)
{
    Expander *           expanderPtr = & expander [Index_U2] ;
    uint8_t i2cAddress = expanderPtr -> i2cAddress ;

    union {
        ExpanderBits_U2  asBits ;
        uint16_t         asWord ;
    } data ;

    // default to all outputs
    data.asWord = 0x0000 ;

    // configure inputs
    data.asBits. MAG_SW_INT    =
    data.asBits. SILEGO_INT    =
    data.asBits. SLOT3_PRESENT =
    data.asBits. SLOT4_PRESENT =
    data.asBits. TP8           = 1 ;

    write (i2cAddress, RegisterAddress_configureAsInput, data.asWord) ;

    // configure with weak pulldown
    data.asWord = 0x0000 ;
    data.asBits. TP8 = 0 ;  // 0: select pulldown
    write (i2cAddress, RegisterAddress_enablePullUp, data.asWord) ;
    data.asWord = 0x0000 ;
    data.asBits. TP8 = 1 ;  // 1: enable pulldpown
    write (i2cAddress, RegisterAddress_enablePullUpDown, data.asWord) ;

    // set interrupt masks (0 enables interrupt)
    data.asWord = 0xffff ;      // default all to no interrupt
/// data.asBits. SILEGO_INT =
/// data.asBits. MAG_SW_INT = 0 ;

    write (i2cAddress, RegisterAddress_interruptMask, data.asWord) ;
}



static void init_InputsOutputs_U22 (void)
{
    Expander *           expanderPtr = & expander [Index_U22] ;
    uint8_t i2cAddress = expanderPtr -> i2cAddress ;

    union {
        ExpanderBits_U22  asBits ;
        uint16_t          asWord ;
    } data ;

    // default to all outputs
    data.asWord = 0x0000 ;

    // configure inputs
    data.asBits. BOARD_ID_BIT_0     =
    data.asBits. BOARD_ID_BIT_1     =
    data.asBits. BOARD_ID_BIT_2     =
    data.asBits. CASE_OPEN          =
    data.asBits. GYRO_INT1          =
    data.asBits. GYRO_INT2          =
    data.asBits. AUXCHARGER_ACTIVE  =
    data.asBits. AUXCHARGER_FAULT_N = 1 ;

    write (i2cAddress, RegisterAddress_configureAsInput, data.asWord) ;

    // set interrupt masks (0 enables interrupt)
    data.asWord = 0xffff ;      // default all to no interrupt
/// data.asBits. CASE_OPEN = 0 ;
//  data.asBits. GYRO_INT1 =
//  data.asBits. GYRO_INT2 = 0 ;

    write (i2cAddress, RegisterAddress_interruptMask, data.asWord) ;
}



uint8_t ioExpander_getBit (IOExpander expanderIndex, uint8_t bitNumber)
{
    if (expanderIndex >= NumberOfIOExpanders)
        return 0 ;

    if (bitNumber > 15)
        return 0 ;

    Expander *           expanderPtr = & expander [expanderIndex] ;
    uint8_t i2cAddress = expanderPtr -> i2cAddress ;

    uint16_t data = read (i2cAddress, RegisterAddress_inputPort) ;

    return (data >> bitNumber) & 1 ;
}



void ioExpander_setBit (IOExpander expanderIndex, uint8_t bitNumber, uint8_t value)
{
    if (expanderIndex >= NumberOfIOExpanders)
        return ;

    if (bitNumber > 15)
        return ;

    Expander *           expanderPtr = & expander [expanderIndex] ;
    uint8_t i2cAddress = expanderPtr -> i2cAddress ;

    uint16_t data = expanderPtr->outputCopy ;   // get the local copy

    // modify the local copy
    uint16_t mask = (1 << bitNumber) ;
    if (value)  data |=   mask ;
    else        data &= ~ mask ;

    // update local copy
    expanderPtr->outputCopy = data ;

    write (i2cAddress, RegisterAddress_outputPort, data) ;
}



bool ioExpander_processRequest (ServiceBuffer * request, ServiceBuffer * response)
{
    (void) request ;
    (void) response ;

    // tbd

    return false ;
}



void ioExpander_initialize (void)
{
    init_OutputLevels_U2  () ;  init_InputsOutputs_U2  () ;
    init_OutputLevels_U22 () ;  init_InputsOutputs_U22 () ;
}

