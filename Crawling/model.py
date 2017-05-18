import os.path,json

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
	for i in range(len(site)):
		var ={
		'site':'',
		'statlog_noext':{},
		'statlog_ext':{},
		'unused_function':[],
		'onlyused_func_noext':[],
		'onlyused_func_ext':[],
		}
		print "["+str(i+1)+"/"+str(len(site))+"] site: "+ site[i]
		var['site']=site[i]
		file = open(os.getcwd()+"/no-extencions/"+site[i])
		for line in file:
			if CleanLog(line) not in templognoex:
				templognoex[CleanLog(line)]= 1
			else: 
				templognoex[CleanLog(line)]+= 1
		var['statlog_noext']=templognoex.items()
		file = open(os.getcwd()+"/Ghostery+ublock/"+site[i])
		for line in file:
			if CleanLog(line) not in templogext:
				templogext[CleanLog(line)] = 1
			else:
				templogext[CleanLog(line)]+= 1
		var['statlog_ext']=templogext.items()
		for key in templognoex.keys():
			if key not in templogext:
				var['onlyused_func_noext'].append(key)
		for key in templogext.keys():
			if key not in templognoex:
				var['onlyused_func_ext'].append(key)
		for func in intrumented_function.keys():
			if func not in templognoex:
				if func not in templogext:
					var['unused_function'].append(func)
		database_site.append(var)
		templogext.clear()
		templognoex.clear()
	file = open('database_site.json','a')
	json.dump(database_site,file)
	file.close()

def main():
	function()

main()

