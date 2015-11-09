
#ifndef TINY85IO_H_
#define TINY85IO_H_

#include <stdint.h>


void initADC(void);
uint16_t analogRead(int8_t channel);


#endif /* TINY85IO_H_ */