#!/usr/bin/env ruby
#this is the server
=begin
Documentation: server.rb
server.rb is the server-side code that handles the server itself. the only other file that runs server side is the ruby that does the web scraping intermittently.

Created: 9/29 by Steven Miller 
Modified: 9/29 by Steven Miller
Modified: 10/04 by Dalton Flanagan	
Modified: 10/05 by Allen Yu
Modified: 10/06 by Steven Miller
Modified: 10/07 by Dalton Flanagan
=end
require 'mechanize'
require 'net/http'
require 'json'
require_relative 'event'
require_relative 'scrape'
require_relative 'get_json'

DELAY_MINUTES = 5 # How long to wait in-between scrapes, longer is friendlier

#-----------------------------------------------------#
# Variables shared between threads
# Created: 9/29 by Steven Miller
# Modified: 10/6 by Steven Milling (Adding logging)
#-----------------------------------------------------#
class Shared
	def initialize()
		@done= false
		@donemutex = Mutex.new
		@urls = []
		@urlsmutex = Mutex.new
		@logmutex = Mutex.new
	end
	def done
		@donemutex.lock
		result = @done
		@donemutex.unlock
		return result
	end
	def quit
		@donemutex.lock
		@done = true
		@donemutex.unlock
	end
	def pushurl(url)
		@urlsmutex.lock
		@urls.push(url)
		@urlsmutex.unlock
	end
	def popurl
		@urlsmutex.lock
		result = @urls.pop if @urls.length > 0
		@urlsmutex.unlock
		return result
	end
	def urlcount
		@urlsmutex.lock
		result = @urls.length
		@urlsmutex.unlock
		return result
	end

	# Stuff sent here will be saved in log file
	# Creadted: 9/29 by Steven Miller
	# Modified: 10/6 by Steven Miller (Moved to shared)
	def logthis(input)
		@logmutex.lock
		file = File.open('./serverlog','a')
		write = "#{Time.now}\t#{input}\n"
		file.write(write)
		puts write
		@logmutex.unlock
	end
end


#----------------------------------------
# Scraper thread: constantly get the JSON
# Created: 9/29 by Steven Miller
# Modified: 10/6 by Allen Yu (get_json was implemented differently )
#----------------------------------------
def launchScraper(shared)
	Thread.new{
		url = shared.popurl
		loop do
			break if shared.done
			name = get_json(url)
			sleep 60*DELAY_MINUTES
		end
	}
end

#----------------------------------------
# Methods needed for main
# Created: 9/29 by Steven Miller
# Modified: 10/6 by Steven Miller (Added ".each")
#----------------------------------------
def getScrapingURLs(shared)
	File.open("urls.txt").each do |line|
		shared.pushurl(line.chomp)
	end
end

#----------------------------------------
# Converts a request into the path of the 
# https://practicingruby.com/articles/implementing-an-http-file-server
# Above url used as reference
# Crated: 9/29 by Steven Miller
# Modified: 10/6 by Steven Miller (Debugging)
# Modified: 10/7 by Dalton Flanagan (trimmed security / all files public)
#----------------------------------------
def filerequested(request)
	request_uri = request.split(" ")[1]
	path = "."+URI.unescape(URI(request_uri).path)
	path = "./index.html" if path == "./"
	return path
end

#----------------------------------------
# Created: 10/6 Steven
#----------------------------------------
def contenttype(path)
	hash = Hash.new('text/plain')
	hash = {'html'=>'text/html','json'=>'application/json'}
	hash[File.extname(path).split(".").last]
end

#----------------------------------------
# Main server thread
# Created: 9/29 by Steven Miller
# Modified: 10/6 by Steven Miller
# Modified: 10/8 by Dalton Flanagan (refactoring)
#----------------------------------------
# Initialized shared variables
shared = Shared.new

# Get the urls we will scrape
getScrapingURLs(shared)
# Launch a thread for each url to scrape
numurls = shared.urlcount
for i in 0...numurls
	launchScraper(shared)
end

server = TCPServer.new('localhost',1337)
puts "ready for client connections!"

# https://practicingruby.com/articles/implementing-an-http-file-server
# Above url used as reference for http response
loop do
	Thread.start(server.accept) do |socket|
		puts "Client connected on socket #{socket}"
		# First line of request
		request = socket.gets
		# Log request
		shared.logthis(request)
		# Get path from request
		path = filerequested(request)
		if File.exist?(path) && !File.directory?(path)
			File.open(path,"rb") do |file|
				# Send header
				puts "Sending #{path} to client"
				socket.print "HTTP/1.1 200 OK\r\n" +
					   "Content-Type: #{contenttype(file)}\r\n" +
					   "Content-Length: #{file.size}\r\n" +
					   "Connection: close\r\n"
				# Separate header from body
				socket.print "\r\n"
				# Send body
				IO.copy_stream(file,socket)
				shared.logthis("sent #{path} to client")
			end
		else
			message = "File not found\n"
			# Send header
			socket.print "HTTP/1.1 404 Not Found\r\n" +
						 "Content-Type: text/plain\r\n" +
						 "Content-Length: #{message.size}\r\n" +
						 "Connection: close\r\n"
			# Separate header from body
			socket.print "\r\n"
			# Send body
			socket.print message	
			shared.logthis("404 sent to client")
		end
		socket.close
	end
end
