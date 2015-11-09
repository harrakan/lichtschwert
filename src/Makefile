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
ODIR=../obj
BDIR =../bin

CFLAGS =-I$(IDIR)
CFLAGS+= -g2 -I. -mmcu=$(DEVICE) -DF_CPU=$(F_CPU) 
CFLAGS+= -Os -ffunction-sections -fdata-sections -fpack-struct -fno-move-loop-invariants -fno-tree-scev-cprop -fno-inline-small-functions  
CFLAGS+= -Wall -Wno-pointer-to-int-cast -std=gnu99

_OBJ = light_ws2812.o rgb_conversion.o tiny85io.o strip_ws2812.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

#Targets

	
$(TARGET): $(OBJ)
	@echo Building $@
	$(CC) -o $(ODIR)/$@.o $^ $(CFLAGS) main.c
	@avr-size $(ODIR)/$@.o
	@avr-objcopy -j .text  -j .data -O ihex $(ODIR)/$@.o $(BDIR)/$@.hex
	@avr-objdump -d -S $(ODIR)/$@.o > $(ODIR)/$@.lss

$(ODIR)/%.o:$(IDIR)/%.c 
	$(CC) -c -o $@ $< $(CFLAGS)
	
.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(ODIR)/*.lss $(BDIR)/*.hex

program : $(TARGET)
	$(GPIO) mode 22 out
	$(GPIO) write 22 0
	$(AVRDUDE) -p $(AVRDUDEMCU) -P /dev/spidev0.0 -c linuxspi -b 20000 -U flash:w:$(BDIR)/$(TARGET).hex
	$(GPIO) write 22 1




