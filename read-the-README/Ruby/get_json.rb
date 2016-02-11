#!/usr/bin/env ruby
# Created: 10/8 Dalton Flanagan

#----------------------------------------
# Creates JSON file based on url and returns JSON file name
# Created: 9/29 by Steven Miller (empty method)
# Modified: 10/5 by Allen Yu (Implemented method to create JSON file)
# Modified: 10/6 by Allen Yu (Added dept)
# Modified: 10/9 by Eric Yu (Added Comments)
#-----------------------------------------
def get_json(url)
	# Get the events from scraping 
	events = scrape(url)
	name = (((url.split ".")[0]).split "/").pop
	# Modify the file of the department
	File.open("API/#{name}.json", "w") do |f|
	f.write("{\n")
	f.write("\t\"events\": [\n")
	# Add an object for every event
	while(!events.empty?)
		curr = events.pop
		f.write("\t\t{\n")
		f.write("\t\t\t \"title\":\"#{curr.title}\",\n")
		f.write("\t\t\t \"timedate\":\"#{curr.timedate}\",\n")
		f.write("\t\t\t \"desc\":\"#{curr.desc}\",\n")
                f.write("\t\t\t \"dept\":\"#{curr.dept}\"\n")
		f.write("\t\t},\n") if !events.empty?
		f.write("\t\t}\n") if events.empty?
	end
	f.write("\t]\n")
	f.write("}")
	end
	return name #10/6 Steven
end
