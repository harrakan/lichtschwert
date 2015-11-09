//#define F_CPU 8000000L

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#include "light_ws2812.h"
#include "rgb_conversion.h"
#include "tiny85io.h"
#include "strip_ws2812.h"

#define  STRIPPIN         0    // Digital Pin
#define  ADC1PIN          1    // Analog pin 1(digital pin #2)
#define  ADC2PIN          2    // Analog pin 2(digital pin #4)
#define  ADC3PIN          3    // Analog pin 3(digital pin #3)



void initWS2812 ()
{
    /* define Pin as configured in ws2812_config.h as LEDstrip Pin 
    DDRB|=_BV(ws2812_pin);*/
    setStrip(STRIPPIN, 50);
    setStripColor(0,0);
    
}

void init() 
{
    DDRB  = 0x00; /* alle Pins von Port B als Eingang */
    CLKPR=_BV(CLKPCE);
    CLKPR=0;			// set clock prescaler to 1 (attiny 25/45/85/24/44/84/13/13A)    
    
    initWS2812 ();  
    initADC();
}


int main ()
{     
    init() ;
    
    for (;;)
    {        
        float hue = 180 + (analogRead(ADC1PIN) / 1023.0) * 360;
        float saturation = analogRead(ADC2PIN) / 1023.0;
        float intensity =  1; //analogRead(ADC2Pin)/1023.0;
        int to = round((analogRead(ADC3PIN) / 1023.0) * getStripLength());
        //int to = 60;
        
        uint32_t color = hsi2rgb(hue, saturation, intensity);
        setColor(color, 0, to, 255);
        _delay_ms(100);        
    }
}