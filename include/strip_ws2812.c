

#include "strip_ws2812.h"
//#include "light_ws2812.h"
#include <avr/io.h>
#include <math.h>

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

#define M_PI_3   1.0471975512 /* pi/3 */

struct cStrip strip;

struct cRGB black = {0,0,0};

void initStrip(uint8_t pin, uint8_t led_count)
{
    strip.pin = pin;
    strip.led_count = led_count;
    setStripColor(black,0);
}

void setColor(struct cRGB color, int from, int to, uint8_t brightness) 
{
    struct cRGB led[strip.led_count];
    float scale = brightness/255.0;
    
    for (int i = 0; i <= strip.led_count; i++) {
        if ( i >= from &&  i < to )
        {
            led[i].r=(uint8_t)( color.g);
            led[i].g=(uint8_t)( color.r);
            led[i].b=(uint8_t)( color.b);  
        }
        else
        {
            led[i].r=0;
            led[i].g=0;
            led[i].b=0;  
        }
    }
    //ws2812_setleds(led,strip.led_count);
    ws2812_setleds_pin(led,strip.led_count,_BV(strip.pin));
}


void setStripColor(struct cRGB color,  uint8_t brightness) 
{    
      setColor(color, 0, strip.led_count, brightness);
}


uint8_t getStripLength(void)
{
    return strip.led_count;
}



int calcP(float H, float S, float I)  {
  return 255 * I / 3 * (1 + S * cos(H) / cos(M_PI_3 - H));
}

int calcQ(float H, float S, float I) {
  return 255 * I / 3 * (1 + S * (1 - cos(H) / cos(M_PI_3 - H)));
}

int calcT(float H, float S, float I) {
  return 255 * I / 3 * (1 - S);
}

struct cRGB  hsi2rgb(float H, float S, float I) {
 struct cRGB rgb;
  
  H = fmod(H, 360); // cycle H around to 0-360 degrees
  H = M_PI * H / (float)180; // Convert to radians.
  S = constrain(S, 0, 1);
  I = constrain(I, 0, 1);


  if (H < 2 * M_PI_3) {
    rgb.r = calcP(H, S, I);
    rgb.g = calcQ(H, S, I);
    rgb.b = calcT(H, S, I);
  } else if (H < 4 * M_PI_3) {
    H = H - 2 * M_PI_3;
    rgb.g = calcP(H, S, I);
    rgb.b = calcQ(H, S, I);
    rgb.r = calcT(H, S, I);
  } else {
    H = H - 4 * M_PI_3;
    rgb.b = calcP(H, S, I);
    rgb.r = calcQ(H, S, I);
    rgb.g = calcT(H, S, I);
  }
  
  return rgb;
}


