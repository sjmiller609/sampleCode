import sys
from bin.tokenizer import Tokenizer
from bin.idlist import IdList

class Output:
	#contructor, parses
	def __init__(self,tokenizer):
		#check for write,remove it
		if(tokenizer.getToken()!=tokenizer.reserved[10]):
			tokenizer.parseError()
		tokenizer.skipToken()
		self._idlist = IdList(tokenizer)
		self._reserved10 = tokenizer.reserved[10]

	#prints
	def prettyPrint(self,indent):
		for i in range(0,indent):
			sys.stdout.write("\t")
		sys.stdout.write("write ")
		self._idlist.prettyPrint()

	#executes
	def execute(self,ds):
		ids = []
		self._idlist.execute(ids)
		for i in ids:
			i.write()
