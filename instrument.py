import json



def instrument():
	instr = []
	with open('targetfunction.txt') as f:
		for line in f:
			instr.append(json.loads(line))
	file = open('targetfunction.json','a')
	temp =[]
	for j in range(len(instr)):
		tup={"filename":instr[j]['filecpp'],"function":instr[j]['method'],"return_type":instr[j]['return_type']}
		temp.append(tup)
	json.dump(temp,file)
	file.close()
	file = open('Deprecator_target.txt','a')
	for j in range(len(instr)):
		file.write(instr[j]['filecpp'])
		file.write(":")
		file.write(instr[j]['method'])
		file.write("\n")
	file.close()
	file = open('return_target.txt','a')
	for j in range(len(instr)):
		file.write(instr[j]['filecpp'])
		file.write(":")
		file.write(instr[j]['method'])
		file.write(":")
		file.write(instr[j]['return_type'])
		file.write("\n")
	file.close()

	

def main():
	instrument()
	
main()

