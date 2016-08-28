#!/usr/bin/env python
#author: steven miller
#creation date: 08/22/2016
#last editted: 08/28/2016
#craiglist search tool

import requests
import re
import smtplib
from time import sleep
from ghost import Ghost

#get and validate user input
def get(regex,query,error_msg):
    temp = False
    while True:
        temp = re.match(regex,input(query))
        print()
        if temp:
            return temp.group(0)
        else:
            print(error_msg)

#returns list of titles
def look_for_new_posts(term,url,zip,dist):
    query_string = url
    query_string += '/search/sso?query='
    query_string += term.replace(' ','+')
    query_string += '&sort=date'
    query_string += "&search_distance="+dist
    query_string += "&postal="+zip
    script = 'elems=document.getElementsByClassName("hdrlnk");arr=[];for(var i = 0;i<elems.length;i++){arr.push(elems[i].innerHTML);};arr;'
    script2 = 'elems=document.getElementsByClassName("hdrlnk");arr=[];for(var i = 0;i<elems.length;i++){arr.push(elems[i].href);};arr;'
    ghost = Ghost()
    titles = []
    urls = []
    with ghost.start() as sess:
        page, res = sess.open(query_string)
        page, res = sess.wait_for_page_loaded()
        titles, res = sess.evaluate(script)
        urls, res = sess.evaluate(script2)
    return titles, urls

#prints out titles its given
def print_titles(titles,urls):
    print(chr(27) + "[2J")
    print()
    if titles:
        #clears screen
        print("found "+str(len(titles))+" links")
        print()
        for i in range(1,len(titles)):
            print(titles[len(titles)-i])
        print("\nabove in reverse chronological order: newest at bottom")
        print("Most recent:")
        print(titles[0])
        print(urls[0])
    else:
        print("no matches found.")
    print()
    

#prints a footer for the view
def print_tail(seconds):
    print()
    print("this list will be updated every minute")
    print("and an email will be sent if there is something new")
    print("last updated "+str(seconds)+" seconds ago")

def send_email(email,body,subject):
    server = smtplib.SMTP('mail.twc.com', 587)
    server.starttls()
    server.login("sjmiller609@twc.com", "35711622")
    msg = "From: "+"sjmiller609@twc.com\n"
    msg += "To: "+email+"\n"
    msg += "Subject: " + subject+"\n\n"
    msg += body
    server.sendmail("sjmiller609@twc.com", email, msg)
    server.quit()

def save(titles,filetail):
    with open(".last_titles"+filetail+".txt","w") as f:
        for title in titles:
            f.write(title.rstrip()+"\n")

def load(filetail):
    result = []
    filename = ".last_titles"+filetail+".txt"
    print("opening file: "+filename)
    with open(filename,"r") as f:
        for line in f:
            line = line.rstrip()
            if not line: continue
            result.append(line)
    print("returning old titles")
    return result

zip = get("(\d{5})","your zip code?: ","not a properly formatted zip (5 digits)")
print("using zip: " + str(zip))
miles = get("(\d{1,2})","miles away to search?: ","pick a number 0-99")
print("miles away: "+str(miles))
email = input("email?: ")
print("\nusing email: "+email)
search_term = get("([\w\s]{1,25})","enter search term: ","not a properly formatted search term")
print("using search term: "+search_term)

#create a new session
session = requests.Session()

#search Google for "<city name> + craigslist"
response = session.get("https://www.google.com/search?num=1&safe=off&espv=2&q="+zip+"+craigslist")
if not response:
	print("could not query Google")

#filter the one result
url = re.search('(https://.{1,20}craigslist\.org)', response.text)
if not url:
	print("could not find craigslist for that city")
	quit()

#get approval of the one result
print("Does the following url look ok?")
print(url.group())
yn = ""
while(yn!="y" and yn!="n"):
    yn = input("y/n: ")
    print("")
if(yn!="y"):
    yn = input("please input your desired url or Q to cancel: ")
    print("")
    if(yn=="Q" or yn=="q"):
        quit()
    url = yn
else:
    url = url.group(0)
print("using url: "+url)


last_titles = []
titles = []
try:
    last_titles = load(zip+miles+search_term)
    for t in last_titles:
        print(t)
except:
    print("file not found")
    last_titles, urls = look_for_new_posts(search_term,url,zip,miles)
    save(last_titles,zip+miles+search_term)
    print_titles(last_titles,urls)
    print_tail(0)

counter = 0
while True:
    sleep(5)
    #do this once every 60 seconds => 5*12 = 60
    if(counter % 12 == 0):
        #get new titles and urls
        titles, urls = look_for_new_posts(search_term,url,zip,miles)
        #if most recent title is not the same as the most recent last time
        if(len(titles) > 0 and len(last_titles)>0 and titles[0] != last_titles[0]):
            #there is at least one new title
            print("!!!!!!!!!!NEW LISTING!!!!!!!!!!!!")
            body = ""
            i = 0
            while((len(titles)>i) and (titles[i] != last_titles[0])):
                body += titles[i]+": "+urls[i]+"\n"
                i+=1
            send_email(email,body,"new craigslist posts: "+search_term)
        save(titles,zip+miles+search_term)
        last_titles = list(titles)
    pass
    print_titles(titles,urls)
    print_tail(5*(counter % 12))
    counter+=1

quit()

