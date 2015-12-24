#ifndef F_CPU
#define F_CPU 8000000L
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#include "light_ws2812.h"
#include "tiny85io.h"
#include "strip_ws2812.h"

#define  STRIPPIN         0    // Digital Pin
#define  ADC1PIN          1    // Analog pin 1(digital pin #2)
#define  ADC2PIN          2    // Analog pin 2(digital pin #4)
#define  ADC3PIN          3    // Analog pin 3(digital pin #3)
#define  BUTTONPIN        1   // Digital Pin


struct cRGB white = {255,255,255};

void initWS2812 ()
{
    /* define Pin as configured in ws2812_config.h as LEDstrip Pin */
     //  DDRB|=_BV(ws2812_pin);
    initStrip(STRIPPIN, 50);
    
}

void init() 
{
    //DDRB  = 0x00; /* alle Pins von Port B als Eingang */
    CLKPR=_BV(CLKPCE);
    CLKPR=0;			// set clock prescaler to 1 (attiny 25/45/85/24/44/84/13/13A)    
    
    initWS2812 ();  
    initADC();
    pinMode(BUTTONPIN,INPUT_PULLUP);
    pinMode(ADC1PIN,INPUT);
    pinMode(ADC2PIN,INPUT);
    pinMode(ADC3PIN,INPUT);
}


int main ()
{    
    float hue,saturation,intensity;
    uint8_t to; 
    init() ;
    
    
    for (;;)
    {        
        
        if (digitalRead(BUTTONPIN) == HIGH )
        {
            setStripColor(white,  255);
        }
        else
        {
            hue = 180 + (analogRead(ADC1PIN) / 1023.0) * 360;
            saturation = analogRead(ADC2PIN) / 1023.0;
            intensity =  1; //analogRead(ADC2Pin)/1023.0;
            to = round((analogRead(ADC3PIN) / 1023.0) * getStripLength());
            
            struct cRGB color = hsi2rgb(hue, saturation, intensity);
            setColor(color, 0, to, 255);    
        }
        _delay_ms(100);     
        
        
        
    }
}
