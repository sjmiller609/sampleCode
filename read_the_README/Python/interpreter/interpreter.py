#!/usr/bin/env python
#steven miller

import sys
from bin.tokenizer import Tokenizer
from bin.program import Program
from bin.datastream import DataStream

#main------------

#check num args
if len(sys.argv) != 3:
	print('incorrect number of arguments\nUsage:\ninterpreter.py <file name> <<input data>')
	exit()

#create tokenizer
tokenizer = Tokenizer(sys.argv[1])
#create datastream
datastream = DataStream(sys.argv[2])
#pass tokenizer to new program
program = Program(tokenizer)
#print the program out
program.prettyPrint(0)
#execute the program
program.execute(datastream)
