#!/usr/bin/env python
#author: Steven Miller

import select
import struct
import socket
import select
import sys
import os
import time

#check number of arguments
if len(sys.argv) != 2:
	print('incorrect number of arguments\nftpc.py <local port>')
	quit()

program, localport = sys.argv
clientsock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP uses Datagram, but not stream
clientsock.bind(('', int(localport)))
print('local ip is:')
print(socket.gethostbyname(socket.gethostname()))

print ("Waiting for packets...")
#wait for first packet
data, addr = clientsock.recvfrom(12)
struct1 = struct.Struct('4s h c i')
data = struct1.unpack(data)
print("recevied segment 1",data)
filesize = data[3]
clientsock.setblocking(0)

#respond when we've got it
received = 0
timeoutseconds = 2.0
sentcount = 0
clientsock.sendto('1'.encode(), addr)
sentcount=sentcount+1
#wait 1 sec then .5sec then .25 sec... until packet is sent 100 times then say connection could not b    e made after waiting a few more seconds
while (received==0) and sentcount<100:

	#wait a bit longer just in case before disconnecting
	if sentcount==99:
		print('having trouble connecting...')
		time.sleep(4)
	ready = select.select([clientsock],[],[],timeoutseconds)

	#this means that socket is ready
	if ready[0]:
		data, addr = clientsock.recvfrom(27)
		#going to try to unpack struct
		try:
			struct2 = struct.Struct('4s h c 20s')
			values = struct2.unpack(data)
			#print('checking: ',values[2],' ',2)
			if int(values[2])==2:
				print('received segment 2');
				received=1
		except:
			print('')#do nothing
	else:   
		#the socket didn't get any packets, so resend confirmations.
		print('socket wasnt ready, resending char 1')
		clientsock.sendto('1'.encode(), addr)
		sentcount=sentcount+1
		timeoutseconds = timeoutseconds/(2.0)
		if timeoutseconds<0.01: timeoutseconds=2.0

#if we exit the while loop but haven't got anything yet, the connection was lost.
if (received==0):   
	print('the connection was lost')
	exit()

i = 0
#send confirmations of receiving seg 2 as many times as appropriate based on the drop rate
while i < sentcount*2:
	clientsock.sendto('2'.encode(),addr)
	i=i+1

#format file name
filename = values[3]
filename = filename.strip()
print('file size and file name received')

#make file/ open file / error check
try:
	if os.path.exists("./recv"):
		writefile = open("./recv/"+filename,"w+")
	else:
		os.makedirs("recv")
		writefile = open("./recv/"+filename,"w+")
		print('new directory recv created')
except:
	print('error creating file in recv')

#this is the big part: receiving and confirming the 3rd segment.
#we will be done or disconnected if we don't receive any packets
#for 5 seconds, and the client algorithm will be extremely likely to
#connect by then even with high packet loss
timeoutseconds = 5.0
bufferspace = []
received = 0
while 1:

	print(str(100*received/filesize)+'%\r'),
	ready = select.select([clientsock],[],[],timeoutseconds)
	#ready to read with recv
	if ready[0]:
		#try to unpack data
		try:
			data, addr = clientsock.recvfrom(412)
			data =struct.unpack('4shci400s',data)
			#print('received data: ')
			if int(data[2])==3:
				temppair = [data[3],data[4]]
				#make sure that we don't insert duplicates
				try:
					bufferspace.remove(temppair)
					received = received - 400
				except:
					print('')
				bufferspace.append(temppair)
				received = received + 400
				confirm = struct.pack('ii',3,int(data[3]))
				clientsock.sendto(confirm,addr)
				#print('confirmation sent for packet ', data[3])
		except:
			print('did receive correct data packet: ', data)
	#timed out, done.
	else:
		break

#organize packets by index
bufferspace.sort();
i = 0
#write al of it to the file
while 1:
	if i>=len(bufferspace): break
	writefile.write(bufferspace[i][1])
	i=i+1
#close stuff
writefile.close()
clientsock.close()
