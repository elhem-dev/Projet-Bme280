.PHONY: clean
bme280: bme280.o main.o
	$(CC) -o bme280 bme280.o main.o -lwiringPi -lm
bme280.o: bme280.c bme280.h
	$(CC) -c -std=c11 -D_DEFAULT_SOURCE -D_BSD_SOURCE -o bme280.o bme280.c 
main.o: main.c 
	$(CC) -c -std=c11 -D_DEFAULT_SOURCE -D_BSD_SOURCE -o main.o main.c
clean:
	rm bme280
install:
	cp bme280 $(DESTDIR)/usr/bin
