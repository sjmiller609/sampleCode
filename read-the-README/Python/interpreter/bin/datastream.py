#!/usr/bin/env python
#this is for reading the input from the data file
import sys

class DataStream:
		#this nested class is to implement the singleton pattern
		class __OnlyOne:
			def __init__(self,filename):
				#open that file
				self.inputfile = open(filename,'r')

				self.tokens = []
				#index of current token
				self.currenttoken = 0
				#fill up tokens with first token or tokens
				self.getMoreTokens()

			def skipToken(self):
				#if we are about to be out of tokens, get more
				if len(self.tokens) <= (self.currenttoken + 1):
					self.getMoreTokens()
				self.currenttoken += 1
				if self.currenttoken >= len(self.tokens):
					print("error: advancing out of range.")
					quit()

			#returns value of current int. if it's not an unsigned int, then error
			def intVal(self):
				if self.isUnsignedInt(self.tokens[self.currenttoken]):
					return int(self.tokens[self.currenttoken])
				else:
					print("error: current token not an unsigned int")
					quit()

			#puts more tokens in the token array
			#will tokenize up to the next whitespace in the file (not including leading whitespaces)
			def getMoreTokens(self):
				whitespacetoken = self.getFirstWhitespaceToken(self.inputfile)
				self.tokens.append(whitespacetoken)

			#returns a string up to the first whitespace, not including leading whitespaces
			#or returns "end_of_file" if there are no more tokens
			def getFirstWhitespaceToken(self,inputfile):
				result = ""
				#strip leading whitespace
				check = inputfile.read(1)
				while check.isspace():
					check = inputfile.read(1)
				#check for eof
				if check == '':
					return "end_of_file"
				#read until we get a space or eof
				while check != '' and not check.isspace():
					result += check
					check = inputfile.read(1)
				return result
				
			def isUnsignedInt(self,token):
				return bool(self.unsignedintregex.match(token))


		instance = None
		def __init__(self, filename):
			if not DataStream.instance:
				DataStream.instance = DataStream.__OnlyOne(filename)

		def _skipToken(self):
			DataStream.instance.skipToken()

		#retrieves the next int and skips it
		def getNextInt(self):
			try:
				result = int(DataStream.instance.tokens[DataStream.instance.currenttoken])
				DataStream.instance.skipToken()
			except ValueError:
				result = None
			return result
