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

program, trollport, droppercent = sys.argv
clientsock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
clientsock.bind(('',int(trollport)))
#the first packet received is from the client
#"packet sniff" to determine where the server is
data, addr = clientsock.recvfrom(900)
clientaddr = addr
unpacked = struct.unpack('4s h c i',data)
serverip = socket.inet_ntoa(unpacked[0])
print('server ip = '+serverip)
serverport = unpacked[1]
random=randint(1,100)
droppercent = int(droppercent)
if random > droppercent:
	clientsock.sendto(data,(serverip,serverport))

clientsock.setblocking(0)
timeoutseconds = 7
while 1:
	random=randint(1,100)
	ready = select.select([clientsock],[],[],timeoutseconds)
	if ready[0]:
		data, addr = clientsock.recvfrom(900)
		if addr==clientaddr:
			if random > droppercent:
				clientsock.sendto(data,(serverip,serverport))
			else:
				print('packet from client to server dropped!')
		else:
			if random > droppercent:
				clientsock.sendto(data,clientaddr)
			else:
				print('packet from server to client dropped!')
	else:
		break

print('troll: connection timed out, port closed')
clientsock.close()
