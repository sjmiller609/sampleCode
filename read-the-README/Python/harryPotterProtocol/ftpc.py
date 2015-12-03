#!/usr/bin/env python
#author: steven miller

import select
import time
import struct
import socket
import select
import sys
import os

 #check number of arguments
if len(sys.argv) != 5:
	print('incorrect number of arguments\nftpc.py <remote-IP> <remote-port> <troll port> <local-file-to-transfer>')
	quit()

program, remoteip, remoteport, trollport, localfile = sys.argv

s =  socket.socket(socket.AF_INET, socket.SOCK_DGRAM) #UDP
#im going to use select, so i want to stop blocking from occuring
s.setblocking(0)

#find file
try:
	readfile = open('./'+localfile,'rb')
	filesize = os.path.getsize('./'+localfile)
except:
	print('could not file the file ',localfile,' in the current directory')

#get first packet ready
ipstring = remoteip
ip32bit = socket.inet_aton(ipstring)
remoteport = int(remoteport)
trollport = int(trollport)
flag = '1'
packed_data = struct.pack('4s h c i', ip32bit, remoteport, flag, filesize)
#now the data is packed in binary/bytes and ready to send

#send first packet 
received = 0
timeoutseconds = 2.0
sentcount = 0
s.sendto(packed_data,('',trollport))
sentcount=sentcount+1
#wait 1 sec then .5sec then .25 sec... until packet is sent 100 times then say connection could not be made after waiting a few more seconds
while (received==0) and sentcount<100:
	if sentcount==99:
		print('having trouble connecting to server...')
		time.sleep(2)
	ready = select.select([s],[],[],timeoutseconds)
	if ready[0]:
		data = s.recv(1)
		#print('received data: ', data.decode())
		if int(data.decode())==1: received = 1
	else:
		print('server hasnt responded, send seg 1 again')
		s.sendto(packed_data,('',trollport))
		sentcount=sentcount+1
		timeoutseconds = timeoutseconds/(2.0)
		if timeoutseconds < 0.01: timeoutseconds=2.0
if (received==0):
	print('a connection could not be established')
	exit()

#get second packet ready
print('getting ready to send second segment')
localfile = localfile + (20-len(localfile))*' '
flag = '2'
packed_data = struct.pack('4s h c 20s', ip32bit, remoteport, flag, localfile)
#send first packet 
received = 0
timeoutseconds = 2.0
sentcount = 0
print('sending: ',packed_data)
s.sendto(packed_data,('',trollport))
sentcount=sentcount+1

#do it again for the second segment, determining packet loss
#wait 1 sec then .5sec then .25 sec... until packet is sent 100 times then say connection could not be made after waiting a few more seconds
while (received==0) and sentcount<100:
	if sentcount==99:
		print('having trouble connecting to server...')
		time.sleep(2)
	ready = select.select([s],[],[],timeoutseconds)
	if ready[0]:
		data = s.recv(1)
		#print('data received: ',data.decode())
		#confirm with server it's ready for 3
		if int(data.decode())==2: received = 1
	else:
		print('server hasnt responded, send again')
		s.sendto(packed_data,('',trollport))
		sentcount=sentcount+1
		timeoutseconds = timeoutseconds/(2.0)
		if timeoutseconds < 0.01: timeoutseconds=2.0
if (received==0):
	print('a connection could not be established')
	exit()


#determine number of packets that will be sent in segment 3
numberofpackets = filesize/400+1
#send all packets in seg 3, index the packages, add each to a map (index,bytes to send)
bufferspace = []
numberofpackets = 0
i = 0
timeoutseconds = 2.0
consecutivetimeouts = 0
flag = '3'

#initialize bufferspace to send data
while 1:
	temp = readfile.read(400)
	if not temp: break
	packed_data = []
	packed_data = ['4shci400s',ip32bit,remoteport,flag,i,temp]
	bufferspace.append(packed_data)
	i = i+1
	numberofpackets = numberofpackets+1

#done with file
readfile.close()
totalleft = numberofpackets
resends = 0
j = 0

print('sending segment 3...\nnumber of packets to send = ',totalleft)
while 1 and totalleft>0:
	#send all packets that aren't yet confirmed
	print('sending remaining ',totalleft,' packets')
	#we are going to spam send, but need to keep in check.
	if resends>100: resends = 100
	#j loop: send @resends times (duplicates) to fight packet loss
	for j in range(0,resends):
		#sends all unconfirmed packets
		for i in range(0,numberofpackets):
			if bufferspace[i]!=0:
				temp = struct.pack(*bufferspace[i])
				s.sendto(struct.pack(*bufferspace[i]),('',trollport))
	#increment resends
	resends = resends+1
	#make sure we aren't spamming way too hard
	if totalleft > 1000: time.sleep(1)
	#check for confirmations
	while 1 and totalleft>0:
		ready = select.select([s],[],[],timeoutseconds)
		#we found a packet
		if ready[0]:
			try:
				data = s.recv(8)
				#print('size received: ',sys.getsizeof(data))
				unpacked = struct.unpack('ii',data)
				#print('unpacked: ', unpacked)

				#it's a confirmation packet
				#print messages act also as comments
				if unpacked[0]==3:
					#print('received confirmation for ', unpacked[1])
					timeoutseconds = 2.0
					consecutivetimeouts=0
					bufferspace.pop(unpacked[1])
					bufferspace.insert(unpacked[1],0)
					totalleft=totalleft-1
					#print('confirmation processed')
			except:
				print('did not receive correct packet: ', data)
		else:
			#print('socket did not have any confirmations')
			consecutivetimeouts = consecutivetimeouts + 1
			timeoutseconds = timeoutseconds/2.0
			if timeoutseconds<.01: timeoutseconds = 0.1 
			break

	if totalleft<=0:
		print('all packets confirmed')
		break
	if consecutivetimeouts>200: print('connection was lost')

print('done!')
s.close()
