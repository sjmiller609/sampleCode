import requests
import json
from pprint import pprint
import re
import time
import sys
#getdata = requests.get(geturl)
#pprint (vars(getdata))
from bs4 import BeautifulSoup
from geopy.geocoders import Nominatim

if len(sys.argv) != 4:
	print(sys.argv[0]+" <item> <location> <num items>")
	exit()

#get list of product IDs
item = sys.argv[1].replace(" ","+")
geturl = "http://www.cvs.com/search/N-0?searchTerm="+item+"&navNum="+sys.argv[3]
response = requests.get(geturl)
page = str(BeautifulSoup(response.content,"html.parser"))

def getURL(page):
    """

    :param page: html of web page (here: Python home page) 
    :return: urls in that page 
    """
    start_link = page.find("href")
    if start_link == -1:
        return None, 0
    start_quote = page.find('"', start_link)
    end_quote = page.find('"', start_quote + 1)
    url = page[start_quote + 1: end_quote]
    return url, end_quote

urls = []
while True:
	url, n = getURL(page)
	page = page[n:]
	if url:
		urls.append(url)
	else:
		break

itemlist = []
skuidlist = []

for i in range(0,len(urls)):
	m = re.search('/shop/.*/.*/.*/(.*)-skuid-(\d{6})',urls[i])
	if m and m.group(2) not in skuidlist:
		itemlist.append(m.group(1))
		skuidlist.append(m.group(2)) 

geolocator = Nominatim()
location = geolocator.geocode(sys.argv[2])
print((location.latitude,location.longitude))


posturl = "http://www.cvs.com/rest/bean/cvs/catalog/CvsBohServiceHandler/storeInventoryValues"

dicts = []

print('loading initial inventory...')
for i in range(0,len(skuidlist)):
	time.sleep(2)
	productId = skuidlist[i]
	postdata = {'productId': productId, 'productSPUlnd': 'true','favstore':'NULL','geolatitude':str(location.latitude),'geolongitude':str(location.longitude)}
	inv = requests.post(posturl,data=postdata)
	dict = {}
	jsons = inv.json()['atgResponse']
	for j in range(0,len(jsons)):
		temp = jsons[j]
		if(temp['Qty'] == ''):
			temp['Qty'] = '0'
		dict[temp['storeAddress']] = temp['Qty']
	dicts.append(dict)
	print(str(100*i/len(skuidlist))+"%")

while True:
	for j in range(0,len(skuidlist)):
		#delay between requests
		print('3 seconds...')
		time.sleep(3)
		productId = skuidlist[j]
		postdata = {'productId': productId, 'productSPUlnd': 'true','favstore':'NULL','geolatitude':str(location.latitude),'geolongitude':str(location.longitude)}
		inv = requests.post(posturl,data=postdata)
		jsons = inv.json()['atgResponse']
		for i in range(0,len(jsons)):
			temp = jsons[i]
			if(temp['Qty'] == ''):
				temp['Qty'] = '0'
			if(dicts[j][temp['storeAddress']] != temp['Qty']):
				print("was: "+dicts[j][temp['storeAddress']]+" now: "+temp['Qty'])
				sold = int(dicts[j][temp['storeAddress']]) - int(temp['Qty'])
				print(temp['storeAddress']+" sold "+str(sold) + " of item " +itemlist[j])
				dicts[j][temp['storeAddress']] = temp['Qty']
#parser = json.loads(inv.json())
#json.dumps(inv.json(),indent=4, separators=(',', ': '))
