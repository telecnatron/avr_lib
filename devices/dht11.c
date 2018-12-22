// -----------------------------------------------------------------------------
// Copyright 2018 Stephen Stebbing. telecnatron.com
//
//    Licensed under the Telecnatron License, Version 1.0 (the “License”);
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//        https://telecnatron.com/software/licenses/
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an “AS IS” BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.
// -----------------------------------------------------------------------------   
#include "dht11.h"

#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>


// config data line as input, enable pullup
//#define DHT11_DATA_IN() PIN_LO(DHT11_DDR, DHT11_PIN); PIN_HI(DHT11_PORT_OUT, DHT11_PIN)
// config data line as input, don't enable pullup
#define DHT11_DATA_IN() PIN_LO(DHT11_DDR, DHT11_PIN); 


// config data line as output, set it low
#define DHT11_DATA_OUT() PIN_HI(DHT11_DDR, DHT11_PIN); PIN_LO(DHT11_PORT_OUT, DHT11_PIN);
// set the data line high
#define DHT11_DATA_HI() PIN_HI(DHT11_PORT_OUT, DHT11_PIN);
// set the data line low
#define DHT11_DATA_LO() PIN_LO(DHT11_PORT_OUT, DHT11_PIN);
// read the value on the data line
#define DHT11_DATA_READ() (DHT11_PORT_IN & _BV(DHT11_PIN))


uint8_t dht11_read(dht11_data_t *reading)
{
    uint8_t data[5]={0}; // space for the 40 bits read from the device

    DHT11_TIMER_INIT();

    // set data line for output
    DHT11_DATA_OUT();
    // drive data lo for 20ms
    DHT11_DATA_LO();
    _delay_ms(21);
    DHT11_DATA_HI();
    // release line, set as input (pulled high)and poll for response
    DHT11_DATA_IN();

    // disable interrupts, restore back to previous state on exiting the ATOMIC_BLOCK
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){

	// wait up to 20 to 40us for device to pull line lo
	DHT11_TIMER_RESET();
	while(DHT11_DATA_READ()){
	    if(DHT11_TIMER_COUNT > DHT11_TIMER_COUNT_40US ){
		// no response
		return DHT11_ERROR_NO_RESPONSE_1;
	    }
	}

	// sensor preamble.
	// now wait up to 80us for line to go hi
	DHT11_TIMER_RESET();
	while(!DHT11_DATA_READ()){
	    if(DHT11_TIMER_COUNT > DHT11_TIMER_COUNT_80US ){
		// no response
		return DHT11_ERROR_NO_RESPONSE_2;
	    }
	}
	// now wait for it to be pulled lo again, ~80us
	DHT11_TIMER_RESET();
	while(DHT11_DATA_READ()){
	    if(DHT11_TIMER_COUNT > DHT11_TIMER_COUNT_80US ){
		// no response
		return DHT11_ERROR_NO_RESPONSE_3;
	    }
	}


	// data transmission starts
	for(uint8_t i=0; i!=5; ++i){
	    // loop thru the 5 bytes of data

	    for(uint8_t j=0; j!=8; ++j){
		// loop thru each bit in data[i]

		// shift along ready for this bit
		data[i] <<= 1;

		// each bit is preceeded with 50us low
		DHT11_TIMER_RESET();
		while(!DHT11_DATA_READ()){
		    if(DHT11_TIMER_COUNT > DHT11_TIMER_COUNT_50US ){
			// no response
			return DHT11_ERROR_BIT_READ_1;
		    }
		}
		// line is high, this is the bit. If line goes low at around 26us then it's a 0, if 70us, it's a 1
		DHT11_TIMER_RESET();
		while(DHT11_DATA_READ()){
		    if(DHT11_TIMER_COUNT > DHT11_TIMER_COUNT_80US ){
			// no response
			return DHT11_ERROR_BIT_READ_2;
		    }
		}
		uint8_t count=DHT11_TIMER_COUNT; // record count asap

		if( count > DHT11_TIMER_COUNT_BIT0_MIN && count < DHT11_TIMER_COUNT_BIT0_MAX){
		    // zero bit- nothing need be done
		}else if(count > DHT11_TIMER_COUNT_BIT1_MIN && count < DHT11_TIMER_COUNT_BIT1_MAX){
		    // one bit
		    data[i] |= 0x01;
		}else{
		    return DHT11_ERROR_BIT_READ_3;
		}
	    }
	}


    }//ATOMIC_BLOCK    

    // check checksum
    if( ((data[0] + data[1] + data[2] + data[3]) & 0xFF) != data[4])  {
	return DHT11_ERROR_CHECKSUM;
    }

    // copy data
    reading->humidity = data[0];
    reading->temp=data[2];

    return DHT11_ERROR_OK;

}
