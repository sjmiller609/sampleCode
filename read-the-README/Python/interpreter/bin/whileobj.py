import sys
from bin.tokenizer import Tokenizer
from bin.condition import Condition

class While:
	#constructor, parses
	def __init__(self,tokenizer):
		from bin.statementseq import StatementSeq
		#get rid of "while"
		if(tokenizer.getToken()!=tokenizer.reserved[7]):
			tokenizer.parseError()
		tokenizer.skipToken()
	
		self._c = Condition(tokenizer)
		#tokenizer starts with "loop",skip it
		if(tokenizer.getToken()!=tokenizer.reserved[8]):
			tokenizer.parseError()
		tokenizer.skipToken()

		self._ss = StatementSeq(tokenizer)
		#should now start with "end"
		if(tokenizer.getToken()!=tokenizer.reserved[2]):
			tokenizer.parseError()
		tokenizer.skipToken()


	#prints
	def prettyPrint(self,indent):
		if indent < 0:
			print("ERROR: indent < 0")
			quit()

		for i in range(0,indent):
			sys.stdout.write("\t")
		sys.stdout.write("while ")

		#condition doesn't take indent input
		self._c.prettyPrint()
		sys.stdout.write(" loop\n")
		self._ss.prettyPrint(indent+1)

		for i in range(0,indent):
			sys.stdout.write("\t")
		sys.stdout.write("end")

	#executes
	def execute(self,ds):
		while (self._c.execute(ds)):
			self._ss.execute(ds)
