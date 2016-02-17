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
