ALL = ftps ftpc timer-process

all: $(ALL)

clean: 
	rm -f *.o $(ALL)
ftps: ftps.c
	gcc -o ftps ftps.c
ftpc: ftpc.c
	gcc -o ftpc ftpc.c
timer-process: timer-process.c
	gcc -o timer-process timer-process.c
experiment: experiment.c
	gcc experiment.c -o experiment -lpthread
crc-test: crc.h crc-test.c
	gcc -o crc-test crc-test.c
