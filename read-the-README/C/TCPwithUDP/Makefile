ALL = crc-test timer-process
all: $(ALL)
clean: 
	rm -f *.o $(ALL)
timer-process: timer-process.c
	gcc -o timer-process timer-process.c
crc-test: crc.h crc-test.c
	gcc -o crc-test crc-test.c
