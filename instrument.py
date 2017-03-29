import psycopg2,os,fnmatch,re
import os.path,string
import sys,json
sys.path.append('/Users/innotommy91/qwer/src/third_party/WebKit/Source/bindings/scripts') #import folder for idl_reader
from idl_reader import IdlReader

def instrument():
	instr = []
	with open('targetfunction.txt') as f:
		for line in f:
			instr.append(json.loads(line))
	file = open('instrument.json','a')
	temp =[]
	for j in range(len(instr)):
		tup={"filename":instr[j]['filecpp'],"function":instr[j]['method']}
		temp.append(tup)
	json.dump(temp,file)
	file.close()


def main():
	instrument()
	
main()

