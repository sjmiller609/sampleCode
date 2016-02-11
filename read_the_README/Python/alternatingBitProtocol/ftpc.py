#!/usr/bin/env python
#Steven Miller
#ftpc.py: client alternating bit protocol
#ignoring out-of-order in my algorithm, this is what Dr. Sinha discussed in class.

import select
import struct
import socket
import sys
import os

#------------------------------
#function definitions
#-----------------------------

#packs data into nice binary format for UDP packet
def pack(remoteip, remoteport, flag, seq, data):
	ip32bit = socket.inet_aton(remoteip)
	output = 0
	fmt = ''
	if flag==1:
		#segment 1
		fmt = '4shcci'
	elif flag==2:
		#segment 2
		length = len(data)
		#buffer with spaces
		if length > 20:
			print('file name is too long, make it 20 or less characters.')
			quit()
		data = data + (20-len(data))*' '
		fmt = '4shcc20s'
	elif flag==3:
		#segment 3
		if len(data) < 500:
			#this is only for the last packet in seg 3
			print('sending last packet')
			fmt = '4shcc'+str(len(data))+'s'
		else:
			fmt = '4shcc500s'
	else:
		#this should never happen
		print('invalid flag value')
		quit()
	output = struct.pack(fmt, ip32bit, remoteport, str(flag), str(seq), data)
	return output
#-----------------------------------

#check number of arguments
def checkargs():
	if len(sys.argv) != 5:
		print('incorrect number of arguments\nftpc.py <remote-IP> <remote-port> <troll-local-port> <local-file-to-transfer>')
		quit()
	return
#-----------------------------------

#this function opens a file and reports size or sends an error message if the file
#can't be found.
def openfile(localfile):
	try:
		readfile = open('./'+localfile,'rb')
		filesize = os.path.getsize('./'+localfile)
	except:
		print('could not file the file ',localfile,' in the current directory')
		quit()
	return readfile, filesize
#-----------------------------------

#sends packets, waits for response. resends if timeout
def send(packed, port, seq, s):
	go = -1
	while go!=int(seq):
		#'' indicates local ip
		s.sendto(packed,('', port))
		ready = select.select([s],[],[], .05)
		if ready[0]:
			data = s.recv(7)
			remoteip, remoteport, ack = struct.unpack('4shc',data)
			go = int(ack)
		else:
			print('ACK timeout (50 ms): resending seq = '+str(seq))
	return
#-----------------------------------

def flip(bit):
	if bit==1:
		bit=0
	elif bit==0:
		bit=1
	else:
		print('invalid sequence number')
		quit()
	return bit

#--------------------------------------
#START OF MAIN
#--------------------------------------

#verify correct args format
checkargs()

#read in command line args
program, remoteip, remoteport, trolllocalport, localfile = sys.argv
#everything is a string, need to switch some of them
remoteport = int(remoteport)
trolllocalport = int(trolllocalport)

#initiate UDP socket
s =  socket.socket(socket.AF_INET, socket.SOCK_DGRAM) #UDP
#im going to use select, so i want to stop blocking from occuring
s.setblocking(0)
s.bind(('',5050))

#open file
readfile, filesize = openfile(localfile)

#current bit = 0
seq = 0

#send segment 1
flag = 1
data = filesize
#pack up data for sending
packed = pack(remoteip, remoteport, flag, seq, data)
send(packed, trolllocalport, seq, s)

#send segment 2
seq = flip(seq)
flag = 2
data = localfile
packed = pack(remoteip, remoteport, flag, seq, data)
send(packed, trolllocalport, seq, s)

#send segment 3
flag = 3
go = 1
while go==1:
	seq = flip(seq)
	data = readfile.read(500)
	#this if statement activates on last iteration, 0<=len(data)<500
	if len(data) < 500: go = 0
	packed = pack(remoteip, remoteport, flag, seq, data)
	send(packed, trolllocalport, seq, s)

#done with file
readfile.close()
#done with socket
s.close()
print('done!')
