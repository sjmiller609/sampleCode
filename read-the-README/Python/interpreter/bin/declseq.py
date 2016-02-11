#!/usr/bin/env python
from bin.tokenizer import Tokenizer
from bin.declaration import Declaration

class DeclarationSeq:
	#constructor, parses
	def __init__(self,tokenizer):
		self._decl = None
		self._ds = None
		#get first decl
		if(tokenizer.getToken()!=tokenizer.reserved[1]):
			self._decl = Declaration(tokenizer)
			#get second decl
			if(tokenizer.getToken()==tokenizer.reserved[3]):
				self._ds = DeclarationSeq(tokenizer)

	#prints out
	def prettyPrint(self,indent):
		if(self._decl is not None):
			self._decl.prettyPrint(indent)
			if(self._ds is not None):
				self._ds.prettyPrint(indent)

	#executes
	def execute(self):
		if(self._decl is not None):
			self._decl.execute()
			if(self._ds is not None):
				self._ds.execute()
		
