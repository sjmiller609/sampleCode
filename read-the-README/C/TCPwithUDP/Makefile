ALL = tcpd ftps ftpc

all: $(ALL)

clean: 
	rm -f *.o $(ALL)
tcpd: tcpd.c
	gcc -o tcpd tcpd.c
ftps: ftps.c
	gcc -o ftps ftps.c
ftpc: ftpc.c
	gcc -o ftpc ftpc.c
experiment: experiment.c
	gcc experiment.c -o experiment -lpthread
