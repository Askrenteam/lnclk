default: all

all: lnclk.c
	gcc lnclk.c -lxcb -lxcb-ewmh -o lnclk

clean:
	rm lnclk
