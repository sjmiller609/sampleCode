#!/usr/bin/env python
import sys
from bin.id import Id
from bin.expression import Expression

class Assign:
	#constructor: does parsing
	def __init__(self,tokenizer):
		#remove id
		self._id = Id(tokenizer)
		#should be "=" now
		if tokenizer.getToken() != tokenizer.specials[2]:
			tokenizer.parseError()
		tokenizer.skipToken()
		#remove expression
		self._exp = Expression(tokenizer)

	#prints out this assign node
	def prettyPrint(self,indent):
		for i in range(0,indent):
			sys.stdout.write("\t")
		self._id.prettyPrint()
		sys.stdout.write(" = ")
		self._exp.prettyPrint()

	#assign the value of exp to this id 
	def execute(self,ds):
		self._id.set(self._exp.execute(ds))
