# AVR-GCC Makefile
#---Prefs---
PROJECT=main
MMCU=attiny25
F_CPU=1000000 #1 MHz
FUSEBITS=0x62
#-----------



SOURCES=main.c
CC=avr-gcc
OBJCOPY=avr-objcopy

CFLAGS=-g -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -Wstrict-prototypes -DF_CPU=$(F_CPU) -Wa,-adhlns=$(<:.c=.lst) -I./ -mmcu=$(MMCU) -Wall
AVRDUDEFLAGS=-p $(MMCU)

ALL: $(PROJECT).hex filesize

$(PROJECT).hex: $(PROJECT).elf
	avr-objcopy -j .text -O ihex $(PROJECT).elf $(PROJECT).hex

$(PROJECT).elf: $(SOURCES)
	$(CC) $(CFLAGS) -o $(PROJECT).elf $(SOURCES)

program: $(PROJECT).hex
	avrdude $(AVRDUDEFLAGS) -c usbtiny -e -U flash:w:$(PROJECT).hex

burn-fuse:
	avrdude $(AVRDUDEFLAGS) -u -U lfuse:w:$(FUSEBITS):m

# Display size of file.
filesize:
	avr-size -C --mcu=$(MMCU) $(PROJECT).elf 

clean:
	rm -f $(PROJECT).elf $(PROJECT).hex $(PROJECT).lst
