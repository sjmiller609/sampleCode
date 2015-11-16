#!/usr/bin/env python
import sys
from bin.tokenizer import Tokenizer
from bin.id import Id

class IdList:
	#constructor, parses
	def __init__(self,tokenizer):
		self._id = None
		self._idlist = None
		#remove id
		self._id = Id(tokenizer)
		#check for ,
		if(tokenizer.getToken()==tokenizer.specials[1]):
			tokenizer.skipToken()
			self._idlist = IdList(tokenizer)

	#prints
	def prettyPrint(self):
		if(self._id != None):
			self._id.prettyPrint()
			if(self._idlist != None):
				sys.stdout.write(", ")
				self._idlist.prettyPrint()

	#executes
	def execute(self,ids):
		if(self._id!=None):
			ids.append(self._id)
			if(self._idlist!=None):
				self._idlist.execute(ids)
		
