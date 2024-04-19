CC = gcc
DEVICE ?= /dev/ttyUSB0

run: src/IoTCommunication.c src/singly_linked_list.o
	mkdir -p bin
	$(CC) -o bin/testTermios $^
	bin/testTermios $(DEVICE)

clean:
	rm -rf bin