import sys
from bin.operator import Operator

class Factor:
	#constructor, parses
	def __init__(self,tokenizer):
		#remove operator
		self._op = Operator(tokenizer)
		self._fac = None
		#check for *
		if(tokenizer.getToken()==tokenizer.specials[12]):
			tokenizer.skipToken()
			self._fac = Factor(tokenizer)

	#prints
	def prettyPrint(self):
		self._op.prettyPrint()
		if self._fac:
			sys.stdout.write(" * ")
			self._fac.prettyPrint()

	#executes
	def execute(self,ds):
		result = self._op.execute(ds)
		if self._fac:
			result = result * self._fac.execute(ds)
		return result
