
#ifndef STRIP_WS2812_H_
#define STRIP_WS2812_H_

#include "light_ws2812.h"

struct cStrip { uint8_t pin; uint8_t led_count;};

void setStrip(uint8_t pin, uint8_t led_count);

uint8_t getStripLength(void);

void setColor(uint32_t color, int from, int to, uint8_t brightness)  ;

void setStripColor(uint32_t color,  unsigned char brightness) ;


#endif /* STRIP_WS2812_H_ */