
#include <avr/io.h>
#include <util/delay.h>
#include "tiny85io.h"



#define BUTTON_PORT PORTB       /* PORTx - register for button output */
#define BUTTON_PIN PINB         /* PINx - register for button input */
#define BUTTON_BIT PB4          /* bit for button input/output */

#define DEBOUNCE_TIME 25        /* time to wait while "de-bouncing" button */
#define LOCK_INPUT_TIME 250     /* time to wait after a button press */


void pinMode(uint8_t pin, uint8_t mode)
{                 
	if (mode == INPUT) { 
                DDRB &= ~(1 << pin); //set port to input
                
	} else if (mode == INPUT_PULLUP) {
                DDRB &= ~(1 << pin); //set port to input
                PORTB |=(1<<pin); /* turn on internal pull-up resistor */
		
	} else {
            
                DDRB |= (1 << pin); //set port to output
	}
}


/*
void pinMode(uint8_t pin, uint8_t mode)
{
        uint8_t  portmapping [6] = {PB0,PB1,PB2,PB3,PB4,PB5};    
      
        
	if (mode == INPUT) { 
		
	} else if (mode == INPUT_PULLUP) {
                PORTB=(1<<portmapping[pin]);
		
	} else {
            
                DDRB |= (1 << DDB0);
                PORTB=(1<<portmapping[pin]);
		
	}
	
    
}*/


int digitalRead(uint8_t pin)
{
      
    /* the button is pressed when BUTTON_BIT is clear */
    if (bit_is_clear(PINB, pin))
    {
        _delay_ms(DEBOUNCE_TIME);
        if (bit_is_clear(PINB, pin)) return HIGH;
    }
    
    return LOW;
}


void digitalWrite(uint8_t pin, uint8_t val)
{
    if (val == LOW )
    {
        PORTB &= ~(1<<pin); // löscht Bit  in PORTB
    }
    PORTB |= (1<<pin); // setzt Bit  in PORTB
}

int button_is_pressed()
{
    
    /* the button is pressed when BUTTON_BIT is clear */
    if (bit_is_clear(BUTTON_PIN, BUTTON_BIT))
    {
        //_delay_ms(DEBOUNCE_TIME);
        if (bit_is_clear(BUTTON_PIN, BUTTON_BIT)) return 1;
    }
    
    return 0;
}



void initADC(void)
{
    /* this function initialises the ADC 
     * 
     *       ADC Notes
     *       
     *       Prescaler
     *       
     *       ADC Prescaler needs to be set so that the ADC input frequency is between 50 - 200kHz.
     *       
     *       Example prescaler values for various frequencies
     *       
     *       Clock   Available prescaler values
     *    - *--------------------------------------
     *    1 MHz   8 (125kHz), 16 (62.5kHz)
     *    4 MHz   32 (125kHz), 64 (62.5kHz)
     *    8 MHz   64 (125kHz), 128 (62.5kHz)
     *    16 MHz   128 (125kHz)
     *    
     *    below example set prescaler to 128 for mcu running at 8MHz
     *    
     *    
     */
    
    ADMUX =
    (0 << ADLAR) |     // left shift result
    (0 << REFS1) |     // Sets ref. voltage to VCC, bit 1
    (0 << REFS0) |     // Sets ref. voltage to VCC, bit 0
    (0 << MUX3)  |     // use ADC2 for input (PB4), MUX bit 3
    (0 << MUX2)  |     // use ADC2 for input (PB4), MUX bit 2
    (1 << MUX1)  |     // use ADC2 for input (PB4), MUX bit 1
    (0 << MUX0);       // use ADC2 for input (PB4), MUX bit 0
    
    ADCSRA = 
    (1 << ADEN)  |     // Enable ADC 
    (1 << ADPS2) |     // set prescaler to 64, bit 2 
    (1 << ADPS1) |     // set prescaler to 64, bit 1 
    (0 << ADPS0);      // set prescaler to 64, bit 0  
}



/* ADC Einzelmessung */
uint16_t ADC_Read( uint8_t channel )
{
    uint8_t low, high;
    // Kanal waehlen, ohne andere Bits zu beeinflußen
    ADMUX = (ADMUX & ~(0x1F)) | (channel & 0x1F);
    ADCSRA |= (1<<ADSC);            // eine Wandlung "single conversion"
    while (ADCSRA & (1<<ADSC) ) {   // auf Abschluss der Konvertierung warten
    }
    
    low  = ADCL;
    high = ADCH; 
    
    // combine the two bytes
    return (high << 8) | low;
}

/* ADC Mehrfachmessung mit Mittelwertbbildung */
/* beachte: Wertebereich der Summenvariablen */
uint16_t ADC_Read_Avg( uint8_t channel, uint8_t nsamples )
{
    uint32_t sum = 0;
    
    for (uint8_t i = 0; i < nsamples; ++i ) {
        sum += ADC_Read( channel );
    }
    
    return (uint16_t)( sum / nsamples );
}

uint16_t analogRead(int8_t channel)
{
    //return ADC_Read_Avg(channel,4);
    return ADC_Read(channel);
}

