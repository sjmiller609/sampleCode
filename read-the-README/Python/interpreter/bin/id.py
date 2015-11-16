import sys
from bin.tokenizer import Tokenizer
from bin.datastream import DataStream

class Id:
	#class variable, dictionary mapping id name strings to values
	idtable = {}
	def __init__(self,tokenizer):
		#check is id
		if not tokenizer.isId(tokenizer.getToken()):
			tokenizer.parseError()
		#save name
		self.string = tokenizer.getToken()
		#initialize in dict if not used yet
		if not self.string in Id.idtable.keys():
			Id.idtable[self.string] = None
		tokenizer.skipToken()

	def prettyPrint(self):
		sys.stdout.write(self.string)

	#used to read from data stream
	def read(self,ds):
		temp = ds.getNextInt()
		if temp is not None:
			self.set(temp)
		else:
			self.set(int(input()))

	#prints out the value
	def write(self):
		print(str(Id.idtable[self.string]))

	#sets the value of the id
	def set(self,val):
		Id.idtable[self.string] = val

	#gets the value of the id
	def get(self):
		return Id.idtable[self.string]
