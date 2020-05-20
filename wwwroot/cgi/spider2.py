#! /bin/python
#! coding utf-8

#####################################################
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
from bs4 import BeautifulSoup
soup = BeautifulSoup(demo,"html.parser")
#print(soup.prettify())
print("===============================================")
# print(soup.find_all('a'))
for tag in soup.find_all('img'):
	print(tag)
# 
# for tag in soup.find_all('a','mnav'):
# 	print(tag)
# 
# for tag in soup.find_all(True):
# 	print(tag.name)
