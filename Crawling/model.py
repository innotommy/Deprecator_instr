import os.path,json
from collections import defaultdict

site = ["google.com.txt",
		"youtube.com.txt",
		"facebook.com.txt",
		"baidu.com.txt",
		"wikipedia.org.txt",
		"yahoo.com.txt",
		"reddit.com.txt",
		"google.co.in.txt",
		"qq.com.txt",
		"taobao.com.txt",
		"amazon.com.txt",
		"twitter.com.txt",
		"google.co.jp.txt",
		"tmall.com.txt",
		"sohu.com.txt",
		"live.com.txt",
		"vk.com.txt",
		"instagram.com.txt",
		"sina.com.cn.txt",
		"360.cn.txt",
		"jd.com.txt",
		"google.de.txt",
		"google.co.uk.txt",
		"linkedin.com.txt",
		"weibo.com.txt",
		"google.fr.txt",
		"google.ru.txt",
		"google.com.br.txt",
		"yandex.ru.txt",
		"yahoo.co.jp.txt",
		"netflix.com.txt",
		"google.com.hk.txt",
		"t.co.txt",
		"hao123.com.txt",
		"imgur.com.txt",
		"google.it.txt",
		"ebay.com.txt",
		"pornhub.com.txt",
		"google.es.txt",
		"detail.tmall.com.txt",
		"wordpress.com.txt",
		"msn.com.txt",
		"aliexpress.com.txt",
		"bing.com.txt",
		"tumblr.com.txt",
		"google.ca.txt",
		"microsoft.com.txt",
		"livejasmin.com.txt",
		"stackoverflow.com.txt",
		"twitch.tv.txt",
		"ok.ru.txt",
		"google.com.mx.txt",
		"ntd.tv.txt",
		"onclkds.com.txt",
		"imdb.com.txt",
		"office.com.txt",
		"blogspot.com.txt",
		"mail.ru.txt",
		"amazon.co.jp.txt",
		"github.com.txt",
		"apple.com.txt",
		"microsoftonline.com.txt",
		"pinterest.com.txt",
		"diply.com.txt",
		"tianya.cn.txt",
		"popads.net.txt",
		"xvideos.com.txt",
		"wikia.com.txt",
		"google.com.tr.txt",
		"csdn.net.txt",
		"google.com.au.txt",
		"service.tmall.com.txt",
		"alipay.com.txt",
		"google.com.tw.txt",
		"whatsapp.com.txt",
		"paypal.com.txt",
		"xhamster.com.txt",
		"adobe.com.txt",
		"youth.cn.txt",
		"pixnet.net.txt",
		"soso.com.txt",
		"coccoc.com.txt",
		"txxx.com.txt",
		"google.pl.txt",
		"dropbox.com.txt",
		"bongacams.com.txt",
		"amazon.de.txt",
		"login.tmall.com.txt",
		"googleusercontent.com.txt",
		"porn555.com.txt",
		"google.co.th.txt",
		"google.com.eg.txt",
		"google.com.sa.txt",
		"fc2.com.txt",
		"google.com.pk.txt",
		"china.com.txt",
		"bbc.co.uk.txt",
		"craigslist.org.txt",
		"espn.com.txt",
		"soundcloud.com.txt"]

database_site = []

def CleanLog(line):
	temp =[]
	temp =line
	save =[]
	j=0
	while temp[j]!="#":
		j=j+1
	j=j+2
	while temp[j]!="#":
		save.append(temp[j])
		j=j+1
	j=j+2
	save.append(":")
	while temp[j]!="#":
		save.append(temp[j])
		j=j+1
	temp = ''.join(save)
	return temp



def function():
	#check right format of function ../../third
	file = open(os.getcwd()+"/Deprecator_target.txt")
	intrumented_function = {}
	for line in file:
		temp = ""
		for i in range(len(line)-1):
			temp = temp+line[i]
		if temp not in intrumented_function:
			intrumented_function[temp]=0
	file = open('func_instr.json','a')
	tempinstr = []
	for key in intrumented_function:
		tempinstr.append(key)
	json.dump(tempinstr,file)
	file.close()
	templogext = {}
	templognoex = {}
	#create a dictionary of standard and file&method to extract statistic on standard
	instr = []
	tempStandardext = []
	tempStandardnoext = []
	Standardtemp = []
	with open('targetfunction.txt') as f:
		for line in f:
			instr.append(json.loads(line))
	d = defaultdict(list)
	for m in range(len(instr)):
		if(instr[m]['standard'] not in d.keys()):
			d[instr[m]['standard']].append(instr[m]['filecpp']+':'+instr[m]['method'])
		if(instr[m]['filecpp']+':'+instr[m]['method'] not in d[instr[m]['standard']]):
				d[instr[m]['standard']].append(instr[m]['filecpp']+':'+instr[m]['method'])
	for i in range(len(site)):
		var ={
		'site':'',
		'statlog_noextension':{},
		'statlog_withextension':{},
		'unused_function':[],
		'onlyused_func_noextension':[],
		'onlyused_func_withextension':[],
		'Standard_used':[],
		'Standard_unused':[],
		'Standard_usedonlywithext':[],
		'Standard_usedonlynoextension':[]
		}
		print "["+str(i+1)+"/"+str(len(site))+"] site: "+ site[i]
		var['site']=site[i]
		file = open(os.getcwd()+"/no-extencions/"+site[i])
		for line in file:
			if CleanLog(line) not in templognoex:
				templognoex[CleanLog(line)]= 1
			else: 
				templognoex[CleanLog(line)]+= 1
		var['statlog_noextension']=templognoex.items()
		file = open(os.getcwd()+"/Ghostery+ublock/"+site[i])
		for line in file:
			if CleanLog(line) not in templogext:
				templogext[CleanLog(line)] = 1
			else:
				templogext[CleanLog(line)]+= 1
		var['statlog_withextension']=templogext.items()
		for key in templognoex.keys():
			if key not in templogext:
				var['onlyused_func_noextension'].append(key)
		for key in templogext.keys():
			if key not in templognoex:
				var['onlyused_func_withextension'].append(key)
		for func in intrumented_function.keys():
			if func not in templognoex:
				if func not in templogext:
					var['unused_function'].append(func)
		for key in templognoex.keys():
			for k , v in d.iteritems():
				if(key in v):
					Standardtemp.append(k) #standard of this function
			if(len(Standardtemp)>1):#if multiple standard x file+method check one by one
				for i in range(len(Standardtemp)):
					if(Standardtemp[i] not in tempStandardnoext):
						tempStandardnoext.append(Standardtemp[i])
			else:
				if(Standardtemp[0] not in tempStandardnoext):
					tempStandardnoext.append(Standardtemp[0])
			Standardtemp = []
		for i in range(len(tempStandardnoext)):#insert standard found in log without extension in standard used
			if(tempStandardnoext[i] not in var['Standard_used']):
				var['Standard_used'].append(tempStandardnoext[i])
		Standardtemp = []
		for key in templogext.keys():
			for k,v in d.iteritems():
				if(key in v):
					Standardtemp.append(k)
			if(len(Standardtemp)>1):
				for i in range(len(Standardtemp)):
					if(Standardtemp[i]not in tempStandardext):
						tempStandardext.append(Standardtemp[i])
			else:
				if(Standardtemp[0] not in tempStandardext):#exclude duplicated standard in tempstandardext
					tempStandardext.append(Standardtemp[0])
			Standardtemp = []
		for i in range(len(tempStandardext)):
			if(tempStandardext[i] not in var['Standard_used']):
				var['Standard_used'].append(tempStandardext[i])
		for key in d.keys(): #check stardard used from the target file
			if(key not in var['Standard_used']):
				var['Standard_unused'].append(key)
		for s in range(len(tempStandardnoext)): #check standard used only without extension
			if(tempStandardnoext[s] not in tempStandardext):
				var['Standard_usedonlynoextension'].append(tempStandardnoext[s])
		for g in range(len(tempStandardext)):
			if(tempStandardext[g] not in tempStandardnoext):
				var['Standard_usedonlywithext'].append(tempStandardext[g])
		database_site.append(var)
		templogext.clear()
		templognoex.clear()
		del tempStandardext[:]
		del tempStandardnoext[:]
		del Standardtemp[:]
	file = open('database_site.json','a')
	json.dump(database_site,file)
	file.close()

def Statistic():
	data={}
	with open('data.json') as f:
			data=json.load(f)
	tempfuncalwaysused = []
	tempfunzionemaiusate=[]
	tempstandardnonusati=[]
	tempstandardalwaysused=[]
	tempfuncrangewithext={}
	tempfuncrangenoext={}
	tempfunzionemaiusate=data[0]['unused_function'] #first unused set checked all over the site
	modifyfuncneverused = []
	tempstandardnonusati=data[0]['Standard_unused'] #first unused set of standard to check all over the site
	modifystand = []
	tempstandardalwaysused = data[0]['Standard_used']
	modifystandalwaysused = []
	for i in range(len(data[0]['statlog_withextension'])):
		if(data[0]['statlog_withextension'][i][0] not in tempfuncalwaysused):
			tempfuncalwaysused.append(data[0]['statlog_withextension'][i][0])
	for i in range(len(data[0]['statlog_noextension'])):
		if(data[0]['statlog_noextension'][i][0] not in tempfuncalwaysused):
			tempfuncalwaysused.append(data[0]['statlog_noextension'][i][0])
	modifyfuncalwaysused = []
	for u in range(len(data)):
		print "["+str(u+1)+"/"+str(len(data))+"] site: "+ data[u]['site']
		for i in range(len(data[u]['statlog_withextension'])):
			if(data[u]['statlog_withextension'][i][0] in tempfunzionemaiusate): #trovata funz usata
				if(data[u]['statlog_withextension'][i][0] not in modifyfuncneverused): #controllo non sia gia inserita in modifyfuncneverused
					modifyfuncneverused.append(data[u]['statlog_withextension'][i][0])
		for i in range(len(data[u]['statlog_noextension'])):
			if(data[u]['statlog_noextension'][i][0]in tempfunzionemaiusate):
				if(data[u]['statlog_noextension'][i][0] not in modifyfuncneverused):
					modifyfuncneverused.append(data[u]['statlog_noextension'][i][0])
		for i in range(len(data[u]['Standard_used'])):
			if(data[u]['Standard_used'][i] in tempstandardnonusati):
				if(data[u]['Standard_used'][i] not in modifystand):
					modifystand.append(data[u]['Standard_used'][i])
		for i in range(len(tempstandardalwaysused)):
			if(tempstandardalwaysused[i] not in data[u]['Standard_used']):
				if(tempstandardalwaysused[i] not in modifystandalwaysused):
					modifystandalwaysused.append(tempstandardalwaysused[i])	
		listtemp = []
		for item in data[u]['statlog_withextension']:
			listtemp.append(item[0])
		for i in range(len(tempfuncalwaysused)):
			if(tempfuncalwaysused[i] not in listtemp):
				if(tempfuncalwaysused[i] not in modifyfuncalwaysused):
					modifyfuncalwaysused.append(tempfuncalwaysused[i])
		listtemp = []
		for item in data[u]['statlog_noextension']:
			listtemp.append(item[0])
		for i in range(len(tempfuncalwaysused)):
			if(tempfuncalwaysused[i] not in listtemp):
				if(tempfuncalwaysused[i] not in modifyfuncalwaysused):
					modifyfuncalwaysused.append(tempfuncalwaysused[i])
		for i in range(len(data[u]['statlog_withextension'])):
			if(data[u]['statlog_withextension'][i][0] not in tempfuncrangewithext.keys()):
				freq=[]
				freq.append(data[u]['statlog_withextension'][i][1])
				freq.append(data[u]['statlog_withextension'][i][1])
				func=""
				func=data[u]['statlog_withextension'][i][0]
				tempfuncrangewithext[func]=freq
			else:
				if(tempfuncrangewithext[data[u]['statlog_withextension'][i][0]][0]>data[u]['statlog_withextension'][i][1]):
					tempfuncrangewithext[data[u]['statlog_withextension'][i][0]][0]=data[u]['statlog_withextension'][i][1]
				if(tempfuncrangewithext[data[u]['statlog_withextension'][i][0]][1]<data[u]['statlog_withextension'][i][1]):
					tempfuncrangewithext[data[u]['statlog_withextension'][i][0]][1]=data[u]['statlog_withextension'][i][1]
		for i in range(len(data[u]['statlog_noextension'])):
			if(data[u]['statlog_noextension'][i][0] not in tempfuncrangenoext.keys()):
				freq=[]
				freq.append(data[u]['statlog_noextension'][i][1])
				freq.append(data[u]['statlog_noextension'][i][1])
				func=""
				func=data[u]['statlog_noextension'][i][0]
				tempfuncrangenoext[func]=freq
			else:
				if(tempfuncrangenoext[data[u]['statlog_noextension'][i][0]][0]>data[u]['statlog_noextension'][i][1]):
					tempfuncrangenoext[data[u]['statlog_noextension'][i][0]][0]=data[u]['statlog_noextension'][i][1]
				if(tempfuncrangenoext[data[u]['statlog_noextension'][i][0]][1]<data[u]['statlog_noextension'][i][1]):
					tempfuncrangenoext[data[u]['statlog_noextension'][i][0]][1]=data[u]['statlog_noextension'][i][1]
	for i in range(len(modifyfuncneverused)):
		if(modifyfuncneverused[i] in tempfunzionemaiusate):
			tempfunzionemaiusate.remove(modifyfuncneverused[i])
	for i in range(len(modifystand)):
		if(modifystand[i] in tempstandardnonusati):
			tempstandardnonusati.remove(modifystand[i])
	for i in range(len(modifyfuncalwaysused)):
		if(modifyfuncalwaysused[i] in tempfuncalwaysused):
			tempfuncalwaysused.remove(modifyfuncalwaysused[i])
	for i in range(len(modifystandalwaysused)):
		if(modifystandalwaysused[i] in tempstandardalwaysused):
			tempstandardalwaysused.remove(modifystandalwaysused[i])
	tup={'funcneverused':tempfunzionemaiusate,
		'funcalwaysused':tempfuncalwaysused,
		'funcrangewithoutext':tempfuncrangenoext,
		'funcrangewithext':tempfuncrangewithext,
		'standardneverused':tempstandardnonusati,
		'standardalwaysused':tempstandardalwaysused}
	file = open('statistic.json','a')
	json.dump(tup,file)
	file.close()


def main():
	Statistic()

	'''
	function()
	'''
main()

