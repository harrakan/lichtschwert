

#include "strip_ws2812.h"
//#include "light_ws2812.h"
#include <avr/io.h>


struct cStrip strip;

void setStrip(uint8_t pin, uint8_t led_count)
{
    strip.pin = pin;
    strip.led_count = led_count;
    
    /* define Pin as configured in ws2812_config.h as LEDstrip Pin */
    DDRB|=_BV(ws2812_pin);
}


void setColor(uint32_t color, int from, int to, uint8_t brightness) 
{
    uint8_t r,b,g;
    
    r = (uint8_t)(color >> 16);
    g = (uint8_t)(color >>  8);
    b = (uint8_t) color;
    
    struct cRGB led[strip.led_count];
    
    for (int i = 0; i <= strip.led_count; i++) {
        if ( i >= from &&  i < to )
        {
            led[i].r=brightness/255.0 * g;
            led[i].g=brightness/255.0 * r ;
            /*
             * original colors; depends on LEDs
             *           led[i].r=r * brightness/255;
             *           led[i].g=g * brightness/255;
             */
            led[i].b=brightness/255.0 * b;  
        }
        else
        {
            led[i].r=0;
            led[i].g=0;
            led[i].b=0;  
        }
    }
    ws2812_setleds(led,strip.led_count);
}

/*
void setColor(struct cRGB color, int from, int to, unsigned char brightness) 
{
    struct cRGB led[STRIPLENGTH];
    
    for (int i = 0; i <= STRIPLENGTH; i++) {
        if ( i >= from &&  i < to )
        {
            led[i].r=white.g * brightness/255.0;
            led[i].g=white.r * brightness/255.0;
            led[i].b=white.b * brightness/255.0;  
        }
        else
        {
            led[i].r=0;
            led[i].g=0;
            led[i].b=0;  
        }
    }
    ws2812_setleds(led,STRIPLENGTH);
}*/


void setStripColor(uint32_t color,  unsigned char brightness) 
{    
      setColor(color, 0, strip.led_count, brightness);
}


uint8_t getStripLength(void)
{
    return strip.led_count;
}



