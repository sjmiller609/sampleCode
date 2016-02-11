#!/usr/bin/env python
import sys
from bin.factor import Factor

class Expression:
	#constructor, parses
	def __init__(self,tokenizer):
		self._exp = None
		#remove factor
		self._factor = Factor(tokenizer)
		self._plusminus = None
		#check for + / -
		if(tokenizer.getToken()==tokenizer.specials[10] or tokenizer.getToken()==tokenizer.specials[11]):
			self._plusminus = tokenizer.getToken()
			tokenizer.skipToken()
			self._exp = Expression(tokenizer)

	#prints
	def prettyPrint(self):
		self._factor.prettyPrint()
		if self._exp:
			sys.stdout.write(" "+self._plusminus+" ")
			self._exp.prettyPrint()

	#executes
	def execute(self,ds):
		result = self._factor.execute(ds)
		if self._exp:
			result = eval(str(result)+self._plusminus+str(self._exp.execute(ds)))
		return result
