#!/usr/bin/env python
import sys
from bin.idlist import IdList
from bin.id import Id

class Declaration:
	#constructor, parses
	def __init__(self,tokenizer):
		#checks for "int"
		if(tokenizer.getToken()!=tokenizer.reserved[3]):
			tokenizer.parseError()
		tokenizer.skipToken()
		self._idlist = IdList(tokenizer)
		#checks for ;
		if(tokenizer.getToken()!=tokenizer.specials[0]):
			tokneizer.parseError()
		tokenizer.skipToken()

	#prints the declaration
	def prettyPrint(self,indent):
		for i in range(0,indent):
			sys.stdout.write("\t")
		sys.stdout.write("int ")
		self._idlist.prettyPrint()
		sys.stdout.write(";\n")

	def execute(self):
		ids = []
		#idlist.execute returns an array of ids
		self._idlist.execute(ids)
		for i in ids:
			i.value = None
