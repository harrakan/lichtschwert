
#ifndef TINY85IO_H_
#define TINY85IO_H_

#include <stdint.h>

#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

void initADC(void);
uint16_t analogRead(int8_t channel);
void digitalWrite(uint8_t pin, uint8_t val);
int digitalRead(uint8_t pin);
void pinMode(uint8_t pin, uint8_t mode);


#endif /* TINY85IO_H_ */