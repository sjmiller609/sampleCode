#!/usr/bin/env python
#this is python (obviously)
#steven miller

#need sys to get args from cli
import sys
import re

#tokenizer class
class Tokenizer:

		def __init__(self,filename):
			#open that file
			self.inputfile = open(filename,'r')
			#once rest of project is done, move this stuff to a config class
			#initialize regex so it only compiles once
			self.idregex = re.compile("^[A-Z]+[0-9]*$")
			#allowing leading zeros
			self.unsignedintregex = re.compile("^[0-9]+$")
			self.special = [";",",","=","!","[","]","&&","||","(",")","+","-","*","!=","==","<",">","<=",">="]

			self.reserved = ["program","begin","end","int","if","then","else","while","loop","read","write"]
			self.debug = None
			#tokens will hold the tokens
			self.tokens  = []
			#index of current token
			self.currenttoken = 0
			#current line number of file
			self.currentline = 0
			#fill up tokens with first token or tokens
			self.getMoreTokens()

		#1-11 = reserved
		#12-30 = special
		#31 = integer
		#32 = indentifier
		#33 = EOF
		def getToken(self):
			return self.currentNum()

		#go to the next token, read from file if we need more tokens in the array
		def skipToken(self):
			#if we are about to be out of tokens, get more
			if len(self.tokens) <= (self.currenttoken + 1):
				self.getMoreTokens()
			self.currenttoken += 1
			if self.currenttoken >= len(self.tokens):
				print("error: advancing out of range.")
				quit()

		#returns the current id. If it's not an id, then error
		def idName(self):
			if self.isIdentifier(self.tokens[self.currenttoken]):
				return self.tokens[self.currenttoken]
			else:
				print("error: current token not an id")
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
			#gets the first token based only on whitespace (may be multiple tokens once done)
			whitespacetoken = self.getFirstWhitespaceToken(self.inputfile)
			self.appendNextToken(whitespacetoken,self.tokens)		

		#returns a string up to the first whitespace, not including leading whitespaces
		#or returns "end_of_file" if there are no more tokens
		def getFirstWhitespaceToken(self,inputfile):
			result = ""
			check = inputfile.read(1)
			if check == '\n':
				self.currentline+=1
			#strip leading whitespace
			while check.isspace():
				check = inputfile.read(1)
				if check == '\n':
					self.currentline+=1
			#check for eof
			if check == '':
				return "end_of_file"
			#read until we get a space or eof
			while check != '' and not check.isspace():
				result += check
				check = inputfile.read(1)
				if check == '\n':
					self.currentline+=1
			return result
			
		#checks if id is an identifier
		def isIdentifier(self,token):
			return bool(self.idregex.match(token))

		#returns the int value assigned to each kind of token of the current token
		def currentNum(self):
			token = None
			if self.currenttoken < len(self.tokens):
				token = self.tokens[self.currenttoken]
				if token in self.special:
					#12-30
					return self.special.index(token)+12
				elif token in self.reserved:
					#1-11
					return self.reserved.index(token)+1
				elif self.isUnsignedInt(token):
					#31
					return 31
				elif self.isIdentifier(token):
					#32
					return 32
				elif token == "end_of_file": 
					return 33
			self.debug = "There was an invalid token.\nFile \'"+self.inputfile.name+"\', line "+str(self.currentline)+", token \'"+token+"\'"
			return -1

		#input token length > 0
		#tokenizes specials off the front of "token" and appends them to "tokens"
		def appendUntilNotSpecial(self,token,tokens):
			#want to iterate backwards to give priority to the longer specials
			#i.e. "===" should be tokenized into "==","="
			i = len(self.special) - 1
			while i >= 0:
				if token.startswith(self.special[i]):
					#get special token as substring
					specialtoken = token[0:len(self.special[i])]
					#remove that substring from the token we are processing
					token = token[len(self.special[i]):len(token)]
					#add special to the result
					tokens.append(specialtoken)
					#reset the loop if there's still stuff in token
					if len(token) > 0:
						i = len(self.special)
				i-=1
			#escaped while, so now the token doesn't start w/ a special
			return token

		#returns true / false depending if "token" starts with a special character
		def startsWithSpecial(self,token):
			i = 0
			while i < len(self.special):
				if token.startswith(self.special[i]):
					return True
				i+=1
			return False

		#tokenizes until a special is found, appending to "tokens"
		def appendUntilSpecial(self,token,tokens):
			#for checking for specials. can't be one at front, so start w/1
			checkindex = 1
			if len(token) > 0:
				while checkindex < len(token):
					if self.startsWithSpecial(token[checkindex:len(token)]):
						toadd = token[0:checkindex]
						token = token[checkindex:len(token)]
						tokens.append(toadd)
						return token
					checkindex+=1
				#we escaped the while, so there are no specials here
				tokens.append(token)
				token = ""
			return token

		#sub-tokenizes the input token into the tokens array
		def appendNextToken(self,token,tokens):
			while len(token) > 0:
				#append the special tokens in the front of the token to tokens
				token = self.appendUntilNotSpecial(token,tokens)
				#now the next token is not a special character. append that to tokens (there should only be one token before either end of token or another special)
				token = self.appendUntilSpecial(token,tokens)

		def isUnsignedInt(self,token):
			return bool(self.unsignedintregex.match(token))
				

#Check args method
#checks for the correct num of input args from cli
def checkargs():
	if len(sys.argv) != 2:
		print('incorrect number of arguments\nUsage:\ntokenizer <file name>')
		exit()
	return

#--------------------------------------------------------
#MAIN ---------------------------------------------------
#prints out the integer value used to represent each token
# 1-11 = reserved
# 12-30 = special
# 31 = integer
# 32 = indentifier
# 33 = EOF
#this is used to display that the tokenizer class works before implementing the rest of the interpreter.
#--------------------------------------------------------

#check num args
checkargs()
filename = str(sys.argv[1])
tokenizer = Tokenizer(filename)
while True:
	temp = tokenizer.getToken()
	if temp > 0:
		print(str(temp))
		if temp !=33:
			tokenizer.skipToken()
		else:
			break
	else:
		print (tokenizer.debug)
		break
exit()
