#! /bin/python
#! coding utf-8

############## get all html page #######################################
import requests
url = "http://image.baidu.com/search/index?tn=baiduimage&ct=201326592&lm=-1&cl=2&ie=gb18030&word=%CD%BC%C6%AC&fr=ala&ala=1&alatpl=adress&pos=0&hs=2&xthttps=000000"
url = "https://www.baidu.com"
url = "https://www.tohomh123.com/yirenzhixia/"
url = "https://www.tohomh123.com/sanniancha/1.html"
r = requests.get(url)
#print(r.status_code)
#print(r.encoding)
#print(r.apparent_encoding)
r.encoding = r.apparent_encoding
#print(r.text)
demo = r.text

############# find all img link #######################################
from bs4 import BeautifulSoup
soup = BeautifulSoup(demo,"html.parser")
#print(soup.prettify())
#print("===============================================")
# print(soup.find_all('a'))
#for tag in soup.find_all('img'):
#	print(tag)
# 
# for tag in soup.find_all('a','mnav'):
# 	print(tag)
# 
# for tag in soup.find_all(True):
# 	print(tag.name)

############### save effect img link ######################################
links = []
for content in soup.find_all('img'):
	s = content['src']
	links.append(s)


############### download img from Web to local ######################################
import os
localxx = []
root =  "/home/hzq/project/http/wwwroot/html/"
i = 0
str = ""
for link in links:
	#print(str + bytes(i) + ": " + link)
	tmp = link.split('.')[-1]
	if tmp == "jpg" or tmp == "png" or tmp == "jpeg":
		path = root + bytes(i) + "." + link.split('.')[-1]
		tmp_path = bytes(i) + "." + link.split('.')[-1]
		i = i + 1
		localxx.append(tmp_path)
		#print(path)
		try:
			if not os.path.exists(root):
				os.mkdir(root)
			#if not os.path.exists(path):
			r = requests.get(link)
			#print(r.status_code)
			with open(path,'wb') as f:
				f.write(r.content)
				f.close()
		except:
			print("")
	

############## put local img to standard output ######################################
for local in localxx:
	print("<img style=\"float: left;width: 200px;margin: 20px;\" src=\"" + local +"\"/>")

