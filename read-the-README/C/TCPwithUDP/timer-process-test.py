#!/usr/bin/python
import socket
from struct import *
import sys
from threading import Thread
from random import randint
from os import times
from time import sleep

def listening(returntimes,packnum):
	insock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
	insock.bind(("127.0.0.1",1234))
	i = 0
	while i < packnum:
		i+=1
		data = insock.recvfrom(4)
		intime = int(1000*(times()[4]))
		seqnum, = unpack('!i',data[0])
		returntimes[seqnum] = intime

if len(sys.argv) < 3:
	print (sys.argv[0]+' <port to send to> <number of packets>')
	exit()

#set up wait times
packnum = int(sys.argv[2])
waittimes = [None]*packnum
for i in range(0,packnum):
	waittimes[i] = randint(1000,5000)

UDP_IP = "127.0.0.1"
UDP_PORT = int(sys.argv[1])

sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
returntimes = [None]*packnum
senttimes = [None]*packnum
listener = Thread(target = listening, args = (returntimes,packnum))
listener.start()
for i in range(0,packnum):
	MESSAGE = pack('!iii',waittimes[i],i,1234)
	sock.sendto(MESSAGE,(UDP_IP,UDP_PORT))
	senttimes[i] = int(1000*(times()[4]))
	sleep(1.0/1000.0)
	
listener.join()

actualwaits = [None]*packnum
avg = 0
diffs = [None]*packnum
for i in range(0,packnum):
	actualwaits[i] = returntimes[i]-senttimes[i]
	diffs[i] = actualwaits[i] - waittimes[i]
	avg +=  actualwaits[i] - waittimes[i]
avg=avg/packnum
	

print("expected waits")
print(waittimes)
print("experimental waits")
print(actualwaits)
print("differences")
print(diffs)
print("average difference between expected and experiemental (msec)")
print(avg)
