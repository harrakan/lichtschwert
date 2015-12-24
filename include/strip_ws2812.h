
#ifndef STRIP_WS2812_H_
#define STRIP_WS2812_H_

#include "light_ws2812.h"


//struct cRGB black = {0,0,0};
//struct cRGB white = {255,255,255};

struct cRGB hsi2rgb(float H, float S, float I);

struct cStrip { uint8_t pin; uint8_t led_count;};

void initStrip(uint8_t pin, uint8_t led_count);

uint8_t getStripLength(void);

void setColor(struct cRGB color, int from, int to, uint8_t brightness)  ;

void setStripColor(struct cRGB color,  uint8_t brightness) ;


#endif /* STRIP_WS2812_H_ */