#!/usr/bin/env python
import sys
from bin.tokenizer import Tokenizer
from bin.declseq import DeclarationSeq
from bin.statementseq import StatementSeq

class Program:
	#contructor, parses
	def __init__(self,tokenizer):
		#check for "program", remove it
		if(tokenizer.getToken()!=tokenizer.reserved[0]):
			tokenizer.parseError()
		tokenizer.skipToken()
		#remove decl s
		self._ds = DeclarationSeq(tokenizer)
		#check for "begin", remove it
		if(tokenizer.getToken()!=tokenizer.reserved[1]):
			tokenizer.parseError()
		tokenizer.skipToken()
		#remove SS
		self._ss = StatementSeq(tokenizer)

		#should now start with "end"
		#check for "end", remove it
		if(tokenizer.getToken()!=tokenizer.reserved[2]):
			tokenizer.parseError()
		tokenizer.skipToken()

	#prints the whole program
	def prettyPrint(self,indent):

		for i in range(0,indent):
			sys.stdout.write("\t")
		sys.stdout.write("program\n")

		self._ds.prettyPrint(indent+1)

		for i in range(0,indent):
			sys.stdout.write("\t")
		sys.stdout.write("begin\n")

		self._ss.prettyPrint(indent+1)

		for i in range(0,indent):
			sys.stdout.write("\t")
		sys.stdout.write("end\n")

	#executes the prorgam
	def execute(self,ds):
		self._ds.execute()
		self._ss.execute(ds)
