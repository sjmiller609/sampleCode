from bin.tokenizer import Tokenizer
import sys
from bin.operator import Operator

class Comparison:
	#constructor, parses
	def __init__(self,tokenizer):
		#check for "if"
		if(tokenizer.getToken()!=tokenizer.specials[8]):
			tokenizer.parseError()
		tokenizer.skipToken()
		#remove op
		self._op1 = Operator(tokenizer)
		#check if next is special,deal with it then remove
		if(tokenizer.getToken() in tokenizer.specials):
			#check to make sure a valid one
			self._sindex = tokenizer.specials.index(tokenizer.getToken())
			if(self._sindex > 12):
				self._compop = tokenizer.getToken()
			else:
				tokenizer.parseError()
		else:
			tokenizer.parseError()
		tokenizer.skipToken()
		#remove op2
		self._op2 = Operator(tokenizer)
		#remove the parenthesis
		if(tokenizer.getToken()!=tokenizer.specials[9]):
			tokenizer.parseError()
		tokenizer.skipToken()

	#prints out hella nicely
	def prettyPrint(self):
		sys.stdout.write("(")
		self._op1.prettyPrint()
		sys.stdout.write(" "+self._compop+" ")
		self._op2.prettyPrint()
		sys.stdout.write(")")

	#i'm using eval() so i dont have to store which kind of special charater it was
	def execute(self,ds):
		return eval(str(self._op1.execute(ds))+self._compop+str(self._op2.execute(ds)))
