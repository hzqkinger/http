#! /bin/python
#! coding utf-8

import os
import urllib

print os.environ.get('PATH')
print os.getenv('PATH')
print("==============================")

url = "url=https://www.tohomh123.com/sanniancha/1.html"
print url.split('=')[-1]

print("==============================")
url_encode = "https%3A%2F%2Fwww.tohomh123.com%2Fsanniancha%2F1.html"
print urllib.unquote(url_encode).decode('utf-8', 'replace').encode('gbk', 'replace')
