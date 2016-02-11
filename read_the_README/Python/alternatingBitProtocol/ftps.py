#!/usr/bin/env python
#Steven Miller
#ftps.py server alternating bit protocol
#ignoring out-of-order, as specificed in instructions

import select
import struct
import socket
import sys
import os

#------------------------------
#Function definitions
#------------------------------
def checkargs():
	if len(sys.argv) != 3:
		print('incorrect number of arguments\nftps.py <local-port> <troll-local-port>')
		quit()
	return

#------------------------------
def flip(bit):
	if bit==0:
		bit=1
	elif bit==1:
		bit=0
	else:
		print('invalid seq!')
	return bit

#------------------------------
def ackseg1(s,seq, troll):
	data, addr = s.recvfrom(12)
	remoteip, remoteport, flag, seqin, filesize = struct.unpack('4shcci', data)
	#this if should always happen, it's just a check.
	if int(flag)==1 and int(seqin)==int(seq):
		print("recevied segment 1, filesize = "+str(filesize))
		#addresses in the af_inet family are (host, port) pair.
		packed = struct.pack('4shc', remoteip, remoteport, seqin)
		s.sendto(packed, ('',int(troll)))
	else:
		print("something went wrong, should have segment 1, seq 0")
	return filesize

#------------------------------
def ackseg2(s,seq, troll):
	go = 1
	filename = ''
	while go==1:
		data, addr = s.recvfrom(28)
		datafront = data[:8]
		remoteip, remoteport, flag, seqin = struct.unpack('4shcc',datafront)

		if int(seq) == int(seqin):
			#we got the expected segment, so send ack and return filename
			go = 0
			remoteip, remoteport, flag, seqin, filename = struct.unpack('4shcc20s',data)
			filename = filename.strip()
			print("recevied segment 2, file name = "+filename)
		else:
			#this means that we got a duplicate, need to resend ack still though
			print('duplicate detected, resending ack')

		#addresses in the af_inet family are (host, port) pair.
		packed = struct.pack('4shc', remoteip, remoteport, seqin)
		remoteip = socket.inet_ntoa(remoteip)
		s.sendto(packed, ('', int(troll)))

	return filename
#------------------------------
def ackseg3(s,seq, troll):
	go = 1
	output = ''
	while go==1:
		data, addr = s.recvfrom(508)
		#just the first 8 bytes
		datafront = data[:8]
		remoteip, remoteport, flag, seqin = struct.unpack('4shcc',datafront)
		if int(seq) == int(seqin):
			#we got the expected segment, so send ack and return filename
			go = 0
			#set output to data without the front stuff
			output = data[8:]
		else:
			#this means that we got a duplicate, need to resend ack still though
			print('duplicate detected, resending ack')

		#addresses in the af_inet family are (host, port) pair.
		packed = struct.pack('4shc', remoteip, remoteport, seqin)
		remoteip = socket.inet_ntoa(remoteip)
		s.sendto(packed, ('', int(troll)))

	return output

#------------------------------
def makefile(filename):
	#make file/ open file / error check
	writefile = ''
	try:
		if os.path.exists("./recv"):
			writefile = open("./recv/"+filename,"w+")
		else:
			os.makedirs("recv")
			writefile = open("./recv/"+filename,"w+")
			print('new directory recv created')
	except:
		print('error creating file in recv')
	return writefile

#------------------------------
#START MAIN
#------------------------------

#check number of arguments
checkargs()

#read in command line arguments
program, localport, trolllocalport = sys.argv
localport = int(localport)
trolllocalport = int(trolllocalport)

#initiate a UDP socket
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP uses Datagram, but not stream
s.bind(('', int(localport)))

#set initial bit to 0, this is the expected bit
seq = 0

#print out local ip address
print('local ip is:')
print(socket.gethostbyname(socket.gethostname()))

print ("Waiting for packets...")
filesize = ackseg1(s, seq, trolllocalport)

seq = flip(seq)
filename = ackseg2(s, seq, trolllocalport)

writefile = makefile(filename)

go = 1
#get all the packets from segment 3
while go:
	seq = flip(seq)
	towrite = ackseg3(s, seq, trolllocalport)
	if len(towrite) < 500:
		#this means it's the last packet
		go = 0
	writefile.write(towrite)

#wait a sec to see if client will send more seq (if they didn't get the ACK)
print('got all data, closing connection... 5 seconds')
s.setblocking(0)
while 1:
	ready = select.select([s],[],[],5)
	if ready[0]:
		data, addr = s.recvfrom(508)
		#just the first 8 bytes
		datafront = data[:8]
		myip, myport, flag, seqin = struct.unpack('4shcc',datafront)
		#addresses in the af_inet family are (host, port) pair.
		remoteip = socket.inet_aton(addr[0])
		port = addr[1]
		packed = struct.pack('4shc', addr, port, seqin)
		s.sendto(packed, addr)
	else:
		break
#close stuff
writefile.close()
s.close()
print('done!')

