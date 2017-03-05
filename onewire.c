// -----------------------------------------------------------------------------
// Copyright Stebbing Computing. 2013.
// $Id: $
// -----------------------------------------------------------------------------
#include <avr/interrupt.h>
#include <util/delay.h>
#include "onewire.h"

// XXX debug
//#include <stdio.h>

// set pin to input. No internal pullup enabled
#define ONEWIRE_RELEASE_BUS() ONEWIRE_DDR &=~ _BV(ONEWIRE_DATA_PIN); ONEWIRE_PORT &=~ _BV(ONEWIRE_DATA_PIN)
// set pin to input. Internal pullup enabled
//#define ONEWIRE_RELEASE_BUS() ONEWIRE_DDR &=~ _BV(ONEWIRE_DATA_PIN); ONEWIRE_PORT |= _BV(ONEWIRE_DATA_PIN)

// set pin to output, pull bus (data pin) low
#define ONEWIRE_SET_LO()      ONEWIRE_DDR |= _BV(ONEWIRE_DATA_PIN);  ONEWIRE_PORT &=~ _BV(ONEWIRE_DATA_PIN)

// macros to disable/save and reenable interrupts
#define ONEWIRE_INTERRUPTS_DISABLE() onewire_interrupts_off()
#define ONEWIRE_INTERRUPTS_ENABLE()  onewire_interrupts_on()

// timings in microseconds
#define ONEWIRE_DELAY_TIME_SLOT 60
#define ONEWIRE_DELAY_A 6
#define ONEWIRE_DELAY_B 64
#define ONEWIRE_DELAY_C 60
#define ONEWIRE_DELAY_D 10
#define ONEWIRE_DELAY_E 9
#define ONEWIRE_DELAY_F 55
#define ONEWIRE_DELAY_G 0
#define ONEWIRE_DELAY_H 480
#define ONEWIRE_DELAY_I 70
#define ONEWIRE_DELAY_J 410

// function declarations
static void onewire_write0();
static void onewire_write1();

static void onewire_interrupts_on()
{
    sei();
}

static void onewire_interrupts_off()
{ 
    cli();
}




void onewire_skip_rom()
{
    // Send the SKIP ROM command on the bus.
    onewire_send_byte(ONEWIRE_ROM_SKIP);
}

uint8_t onewire_receive_byte()
{
    uint8_t data=0x0;
    unsigned char i;

    // Do once for each bit
    for (i = 0; i < 8; i++)
    {
        // Shift temporary input variable right.
        data >>= 1;
        // Set the msb if a '1' value is read from the bus.
        // Leave as it is ('0') else.
        if (onewire_read_bit()){
            // Set msb
            data |= 0x80;
        }
    }
    return data;
    
}

void onewire_send_byte(uint8_t byte)
{
    uint8_t temp;
    unsigned char i;
    
    // Do once for each bit
    for (i = 0; i < 8; i++)
    {
        // Determine if lsb is '0' or '1' and transmit corresponding
        // waveform on the bus.
        temp = byte & 0x01;
        if (temp)
        {
            onewire_write1();
        }
        else
        {
	    onewire_write0();
        }
        // Right shift the data to get next bit.
        byte >>= 1;
    }
}

uint8_t onewire_detect_presence()
{
    uint8_t presenceDetected=0;

    ONEWIRE_INTERRUPTS_DISABLE();
    // set low and delay

    ONEWIRE_SET_LO();
    _delay_us(ONEWIRE_DELAY_H); // 480us
    // release and delay
    ONEWIRE_RELEASE_BUS();
    _delay_us(ONEWIRE_DELAY_I); // 70 us
    // sample bus to detect presence and delay - a present device will hold bus low
    presenceDetected = ~(ONEWIRE_PORT_IN & _BV(ONEWIRE_DATA_PIN));
    // XXX
    _delay_us(ONEWIRE_DELAY_J);

    ONEWIRE_INTERRUPTS_ENABLE();
//    printf("port: %01x, pd: %01x, bv: %01x pin: %01x\n",ONEWIRE_PORT_IN, presenceDetected),_BV(ONEWIRE_DATA_PIN), ONEWIRE_DATA_PIN;
    return presenceDetected ;
}

void onewire_init()
{
    ONEWIRE_RELEASE_BUS();
    // The first rising edge can be interpreted by a slave as the end of a
    // Reset pulse. Delay for the required reset recovery time (H) to be 
    // sure that the real reset is interpreted correctly.
    _delay_us(ONEWIRE_DELAY_H);
}

static void onewire_write0()
{
    ONEWIRE_INTERRUPTS_DISABLE();

    ONEWIRE_SET_LO();
    _delay_us(ONEWIRE_DELAY_C);
    ONEWIRE_RELEASE_BUS();
    _delay_us(ONEWIRE_DELAY_D);

    ONEWIRE_INTERRUPTS_ENABLE();
}

static void onewire_write1()
{
    ONEWIRE_INTERRUPTS_DISABLE();

    ONEWIRE_SET_LO();
    _delay_us(ONEWIRE_DELAY_A);
    ONEWIRE_RELEASE_BUS();
    _delay_us(ONEWIRE_DELAY_B);

    ONEWIRE_INTERRUPTS_ENABLE();
}

uint8_t onewire_read_bit()
{
    uint8_t bit=0;
    
    ONEWIRE_INTERRUPTS_DISABLE();

    // set lo and delay
    ONEWIRE_SET_LO();
    _delay_us(ONEWIRE_DELAY_A);
    // release and delay
    ONEWIRE_RELEASE_BUS();
    _delay_us(ONEWIRE_DELAY_E);
    // read bus and delay
    bit=ONEWIRE_PORT_IN & _BV(ONEWIRE_DATA_PIN);
    _delay_us(ONEWIRE_DELAY_F);

    ONEWIRE_INTERRUPTS_ENABLE();
    if(bit)
	return 1;
    else
	return 0;
}

uint8_t onewire_crc8(uint8_t byte, uint8_t seed)
{
   unsigned char bitsLeft;
    unsigned char temp;

    for (bitsLeft = 8; bitsLeft > 0; bitsLeft--)
    {
        temp = ((seed ^ byte) & 0x01);
        if (temp == 0)
        {
            seed >>= 1;
        }
        else
        {
            seed ^= 0x18;
            seed >>= 1;
            seed |= 0x80;
        }
        byte >>= 1;
    }
    return seed;       
}
