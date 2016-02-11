#!/usr/bin/env python
import sys
from bin.tokenizer import Tokenizer
from bin.elseobj import Else
from bin.condition import Condition

class If:
	#constructor, parses
	def __init__(self,tokenizer):
		#check for "if"
		if(tokenizer.getToken()!=tokenizer.reserved[4]):
			tokenizer.parseError()
		tokenizer.skipToken()
	
		self._c = Condition(tokenizer)
		#tokenizer starts with "then",skip it
		if(tokenizer.getToken()!=tokenizer.reserved[5]):
			tokenizer.parseError()
		tokenizer.skipToken()
		#need to import down here to prevent circular dependency issues with inital import
		from bin.statementseq import StatementSeq
		self._ss = StatementSeq(tokenizer)
		#assume there is an else. sometimes else does nothing
		self._else = Else(tokenizer)
		#should now start with "end"
		if(tokenizer.getToken()!=tokenizer.reserved[2]):
			tokenizer.parseError()
		tokenizer.skipToken()

	#prints it out
	def prettyPrint(self,indent):
		for i in range(0,indent):
			sys.stdout.write("\t")
		sys.stdout.write("if ")

		#condition doesn't take indent input
		self._c.prettyPrint()
		sys.stdout.write(" then\n")
		self._ss.prettyPrint(indent+1)

		for i in range(0,indent):
			sys.stdout.write("\t")
		sys.stdout.write("end")

	#executes
	def execute(self,ds):
		if (self._c.execute(ds)):
			self._ss.execute(ds)
		else:
			self._else.execute(ds)
