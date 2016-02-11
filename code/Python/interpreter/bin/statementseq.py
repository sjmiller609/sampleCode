#!/usr/bin/env python
from bin.tokenizer import Tokenizer
from bin.statement import Statement

class StatementSeq:
	#constructor, parses
	def __init__(self,tokenizer):
		self._ss = None
		#remove statement, might be nothing (added public instance variable none to check here)
		self._statement = Statement(tokenizer)
		#if it wasn't nothing, then get the following statement(s)
		if not self._statement.none:
			self._ss = StatementSeq(tokenizer)

	#prints
	def prettyPrint(self,indent):
		if(self._statement is not None):
			self._statement.prettyPrint(indent)
			if(self._ss is not None):
				self._ss.prettyPrint(indent)

	#executes
	def execute(self,ds):
		if(self._statement is not None):
			self._statement.execute(ds)
			if(self._ss is not None):
				self._ss.execute(ds)
