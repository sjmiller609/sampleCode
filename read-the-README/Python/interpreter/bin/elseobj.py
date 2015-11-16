#!/usr/bin/env python
import sys

class Else:
	def __init__(self,tokenizer):
		self._ss = None
		#check to make sure it starts with "else"
		if(tokenizer.getToken()!=tokenizer.reserved[2]):
			if(tokenizer.getToken()!=tokenizer.reserved[6]):
				tokenizer.parseError()
			else:
				tokenizer.skipToken()
			#need to import in body to prevent python problems with mutual dependence
			from bin.statementseq import StatementSeq
			self._ss = StatementSeq(tokenizer)

	def prettyPrint(self,indent):
		for i in range(0,indent):
			sys.stdout.write("\t")
		sys.stdout.write("else\n")
		self._ss.prettyPrint(indent+1)
		

	def execute(self,ds):
		if(self._ss is not None):
			self._ss.execute(ds)
