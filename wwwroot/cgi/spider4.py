#! /bin/python
#! coding utf-8

import urllib.request
import osfrom bs4
import BeautifulSoup

import requests
from bs4 import BeautifulSoup

def getSrc(url):
	r = requests.get(url)
	r.encoding = r.apparent_encoding
	html = r.text
	#print(r.text)
	#print(soup.prettify())
	#for tag in soup.find_all('img'):
	#	print(tag)

	soup = BeautifulSoup(html,'html.parser', from_encoding='utf-8')
	links = []
	for content in result: s = content['src']
	    if s is None:
	        continue
	    else:
	        links.append(s)
	print("共有"+str(len(links))+"张图片")
	return links

def save(path,links=[]):
	if(len(links)==0):
		print('没有符合条件的图片')
		return
	if not os.path.exists(path):
	    os.makedirs(path)
	i = 0
	for link in links:
	    i+=1
	    filename =path+'/photo'+str(i)+'.jpg'
	    with open(filename,'w'):
	        urllib.request.urlretrieve(link,filename)
	        print('正在下载：'+str(link))
	
	print("图片下载完成")

def doCrew(url,path='path'):
    links=getSrc(url)
    save(path,links)

if __name__ == "__main__":
	url = 'http://tieba.baidu.com/p/5698856079'
	path='photo'
	doCrew(url,path)
