#!/usr/bin/python
import socket
from struct import *
import sys

if len(sys.argv) < 2:
	print (sys.argv[0]+' <port to send to>')
	exit()
UDP_IP = "127.0.0.1"
UDP_PORT = int(sys.argv[1])
#MESSAGE = pack('hhh',1000,1,1234)
MESSAGE = b'\x00\x00\x04\xD2\x00\x00\x00\x01\x00\x00\x04\xD2'
print(MESSAGE)
sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
sock.sendto(MESSAGE,(UDP_IP,UDP_PORT))
