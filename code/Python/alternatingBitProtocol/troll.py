#!/usr/bin/env python
#author:Steven Miller

from random import randint
import select
import socket
import struct
import sys
import os

 #check number of arguments
if len(sys.argv) != 3:
	print('incorrect number of arguments\nftpc.py <troll port> <drop percent>')
	quit()

#open socket
program, trollport, droppercent = sys.argv
droppercent = int(droppercent)
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(('',int(trollport)))

sock.setblocking(0)
timeoutseconds = 10
#loop to receive packets and switch address
while 1:
	random=randint(1,100)
	ready = select.select([sock],[],[],timeoutseconds)
	if ready[0]:
		#swap out the packet info
		data, localaddr = sock.recvfrom(900)
		datafront = data[:6]
		forwardip, forwardport = struct.unpack('4sh', datafront)
		forwardip = socket.inet_ntoa(forwardip)
		localip = socket.inet_aton(socket.gethostbyname(socket.gethostname()))
		datafront = struct.pack('4sh',localip,int(localaddr[1]))
		data = datafront + data[6:]
		#send packet sometimes
		if random > droppercent:
			sock.sendto(data,(forwardip, forwardport))
		else:
			print("troll dropped a packet")
	else:
		break

#close socket
sock.close()
print('troll: connection timed out, port closed')
