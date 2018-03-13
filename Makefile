default: all

all: lnclk config 
	gcc *.o -lxcb -lxcb-ewmh -o lnclk

lnclk: lnclk.c
	gcc -c lnclk.c -lxcb -lxcb-ewmh -o lnclk.o

config: config.c
	gcc -c config.c -lxcb -lxcb-ewmh -o config.o

clean: 
	rm *.o
