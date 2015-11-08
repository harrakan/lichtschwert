#define F_CPU 8000000L
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#define  STRIPLENGTH     50
#include "rgb_conversion.h"
#include "light_ws2812.h"
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))


#define  ADC1PIN          1    // Analog pin 1(digital pin #2)
#define  ADC2PIN          2    // Analog pin 2(digital pin #4)
#define  ADC3PIN          3    // Analog pin 3(digital pin #3)



struct cRGB black = {0,0,0};    


void setColor(struct cRGB color, int from, int to, uint8_t brightness) 
{
    struct cRGB led[STRIPLENGTH];
    
    for (int i = 0; i <= STRIPLENGTH; i++) {
        if ( i >= from &&  i < to )
        {
            led[i].r=brightness/255.0 * color.g;
            led[i].g=brightness/255.0 * color.r ;
            /*
             * original colors; depends on LEDs
             *           led[i].r=color.r * brightness/255;
             *           led[i].g=color.g * brightness/255;
             */
            led[i].b=brightness/255.0 * color.b;  
        }
        else
        {
            led[i].r=0;
            led[i].g=0;
            led[i].b=0;  
        }
    }
    ws2812_setleds(led,STRIPLENGTH);
}


void initWS2812 ()
{
    /* define Pin as configured in ws2812_config.h as LEDstrip Pin */
    DDRB|=_BV(ws2812_pin);
    
    setColor(black ,0,STRIPLENGTH,0);
    
}

void init() 
{
    DDRB  = 0x00; /* alle Pins von Port B als Eingang */
    CLKPR=_BV(CLKPCE);
    CLKPR=0;			// set clock prescaler to 1 (attiny 25/45/85/24/44/84/13/13A)    
    
}


void initADC()
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

int main ()
{
    uint32_t color;
    init() ;
    initWS2812 ();  
    initADC();
    
    for (;;)
    {
        
        float hue = 180 + (analogRead(ADC1PIN) / 1023.0) * 360;
        float saturation = analogRead(ADC2PIN) / 1023.0;
        float intensity =  1; //analogRead(ADC2Pin)/1023.0;
        int to = round((analogRead(ADC3PIN) / 1023.0) * STRIPLENGTH);
        //int to = 60;
        color = hsi2rgb(hue, saturation, intensity);
        //setColor(color, 0, to, 255);
        _delay_ms(100);
        
    }
}