import sys
from bin.tokenizer import Tokenizer
from bin.idlist import IdList

class Input:
	#constructor, parses
	def __init__(self,tokenizer):
		#check for read
		if(tokenizer.getToken()!=tokenizer.reserved[9]):
			tokenizer.parseError()
		tokenizer.skipToken()
		#remove idlist
		self._idlist = IdList(tokenizer)

	#prints out
	def prettyPrint(self,indent):
		for i in range(0,indent):
			sys.stdout.write("\t")
		sys.stdout.write("read ")
		self._idlist.prettyPrint()

	#executes
	def execute(self,ds):
		ids = []
		self._idlist.execute(ids)
		for id in ids:
			id.read(ds)
