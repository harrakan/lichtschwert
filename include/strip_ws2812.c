

struct cRGB black = {0,0,0};    
struct cRGB red = {255,0,0};  
struct cRGB white = {255,255,255};


struct cStrip { uint8_t pin; uint8_t led_count;};
struct cStrip strip;

void setStrip(uint8_t pin, uint8_t led_count)
{
    strip.pin = pin;
    strip.led_count = led_count;
    
    /* define Pin as configured in ws2812_config.h as LEDstrip Pin */
    DDRB|=_BV(ws2812_pin);
}

void setColor(struct cRGB color, int from, int to, unsigned char brightness) 
{
    struct cRGB led[strip.led_count];
    
    for (int i = 0; i <= strip.led_count; i++) {
        if ( i >= from &&  i < to )
        {
            led[i].r=color.g * brightness/255.0;
            led[i].g=color.r * brightness/255.0;
            /*
             * original colors; depends on LEDs
            led[i].r=color.r * brightness/255;
            led[i].g=color.g * brightness/255;
            */
            led[i].b=color.b * brightness/255.0;  
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


void setStripColor(struct cRGB color,  unsigned char brightness) 
{    
      setColor(color, 0, strip.led_count, brightness);
}