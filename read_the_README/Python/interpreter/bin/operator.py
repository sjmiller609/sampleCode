import sys
from bin.id import Id

class Operator:
	#constructor, parses
	def __init__(self,tokenizer):
		self._int = None
		self._id = None
		self._exp = None
		#check for parenthesis
		if(tokenizer.getToken()==tokenizer.specials[8]):
			from bin.expression import Expression
			tokenizer.skipToken()
			self._exp = Expression(tokenizer)
			if(tokenizer.getToken()!=tokenizer.specials[9]):
				tokenizer.parseError()
			tokenizer.skipToken()
		#otherwise we are int or id
		else:
			#try / except is very a "pythony" way to do this
			#not considered bad practice
			try:
				self._int = int(tokenizer.getToken())
				tokenizer.skipToken()
			except ValueError:
				self._id = Id(tokenizer)

	#prints
	def prettyPrint(self):
		if self._exp is not None:
			sys.stdout.write(tokenizer.specials[8])
			self._exp.prettyPrint()
			sys.stdout.write(tokenizer.specials[9])
		elif self._int is not None:
			sys.stdout.write(str(self._int))
		else:
			self._id.prettyPrint()

	#executes
	def execute(self,ds):
		if self._exp is not None:
			return self._exp.execute(ds)
		elif self._int is not None:
			return self._int
		else:
			return self._id.get()
