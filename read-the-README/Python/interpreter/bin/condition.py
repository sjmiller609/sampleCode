#!/usr/bin/env python
import sys
from bin.tokenizer import Tokenizer
from bin.comparison import Comparison

class Condition:
	#constructor, does parsing
	def __init__(self,tokenizer):
		self._comp = None
		self._cond1 = None
		self._cond2 = None
		self._noflip = True
		self._isAndNotOr = None
		#check for !
		if(tokenizer.getToken()==tokenizer.specials[3]):
			tokenizer.skipToken()
			self._noflip = False
			self._cond1 = Condition(tokenizer)
		#check for && ||
		elif(tokenizer.getToken()==tokenizer.specials[4]):
			tokenizer.skipToken()
			self._cond1 = Condition(tokenizer)
			if (tokenizer.getToken()==tokenizer.specials[6]):
				#&&
				self._isAndNotOr = True
			elif(tokenizer.getToken()==tokenizer.specials[7]):
				#||
				self._isAndNotOr = False
			else:
				tokenizer.parseError()
			tokenizer.skipToken()
			self._cond2 = Condition(tokenizer)
			if(tokenizer.getToken()!=tokenizer.specials[5]):
				tokenizer.parseError()
			tokenizer.skipToken()
		#it must be a lonely comp
		else:
			self._comp = Comparison(tokenizer)

	#prints it out, self documented
	def prettyPrint(self):
		if self._comp:
			self._comp.prettyPrint()
		elif not self._noflip:
			sys.stdout.write("!")
			self._cond1.prettyPrint()
		else:
			sys.stdout.write(tokenizer.specials[4])
			self._cond1.prettyPrint()
			if self._isAndNotOr:
				sys.stdout.write(tokenizer.specials[6])
			else:
				sys.stdout.write(tokenizer.specials[7])
			self._cond2.prettyPrint()
			sys.stdout.write(tokenizer.specials[5])

	#executes, self documented
	def execute(self,ds):
		if self._comp:
			return self._comp.execute(ds)
		elif not self._noflip:
			return not self._cond1.execute(ds)
		else:
			if self._isAndNotOr:
				return self._cond1.execute(ds) and self._cond2.execute(ds)
			else:
				return self._cond1.execute(ds) or self._cond2.execute(ds)
