import sys
from bin.tokenizer import Tokenizer
from bin.ifobj import If
from bin.whileobj import While
from bin.input import Input
from bin.output import Output
from bin.assign import Assign

class Statement:
	#constructor, parses
	def __init__(self,tokenizer):
		self.none = False
		self._type = None
		self._count = None
		test = tokenizer.getToken()
		#check which kind
		if test == tokenizer.reserved[4]:
			self._type = If(tokenizer)
		elif (test == tokenizer.reserved[7]):
			self._type = While(tokenizer)
		elif (test == tokenizer.reserved[9]):
			self._input = True
			self._type = Input(tokenizer)
		elif (test == tokenizer.reserved[10]):
			self._type = Output(tokenizer)
		elif tokenizer.isId(test):
			self._type = Assign(tokenizer)
		else:
			#by design this happens sometimes
			#statement is nothing
			self.none = True

		#make sure to do nothing if we are none
		if(not self.none):
			#optimized the ; to be in statement's bnf
			if(tokenizer.getToken()!=tokenizer.specials[0]):
				tokenizer.parseError()
			tokenizer.skipToken()

	#prints
	def prettyPrint(self,indent):
		if(self._type is not None):
			self._type.prettyPrint(indent)
			sys.stdout.write(";\n")

	#executes
	def execute(self,ds):
		if(self._type is not None):
			self._type.execute(ds)
