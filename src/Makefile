# Device
F_CPU = 8000000
DEVICE = attiny85
AVRDUDEMCU=t85

# Tools:
CC = avr-gcc
AVRDUDE=/usr/local/bin/avrdude
GPIO=gpio -g


#Variables
TARGET=lichtschwert

IDIR =../include

CFLAGS =-I$(IDIR)
CFLAGS+= -g2 -I. -mmcu=$(DEVICE) -DF_CPU=$(F_CPU) 
CFLAGS+= -Os -ffunction-sections -fdata-sections -fpack-struct -fno-move-loop-invariants -fno-tree-scev-cprop -fno-inline-small-functions  
CFLAGS+= -Wall -Wno-pointer-to-int-cast -std=gnu99

ODIR=../obj
BDIR =../bin

_DEPS = light_ws2812.h rgb_conversion.c
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = light_ws2812.o rgb_conversion.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

#Targets
$(ODIR)/%.o: %.c $(DEPS)
	@echo Building $@
	$(CC) -c -o $@ $< $(CFLAGS)

lichtschwert: $(OBJ)
	@echo Building $@
	#$(CC) $(CFLAGS) -o $(ODIR)/$@.o #main.c #../include/light_ws2812.c ../include/rgb_conversion.c
	$(CC) -o $@ $^ $(CFLAGS)
	@avr-size $(ODIR)/$@.o
	@avr-objcopy -j .text  -j .data -O ihex $(ODIR)/$@.o $(BDIR)/$@.hex
	@avr-objdump -d -S $(ODIR)/$@.o > $(ODIR)/$@.lss


.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(ODIR)/*.lss $(BDIR)/*.hex

program : lichtschwert
	$(GPIO) mode 22 out
	$(GPIO) write 22 0
	$(AVRDUDE) -p $(AVRDUDEMCU) -P /dev/spidev0.0 -c linuxspi -b 20000 -U flash:w:$(BDIR)/$(TARGET).hex
	$(GPIO) write 22 1




