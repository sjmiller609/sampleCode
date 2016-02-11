#!/usr/bin/env ruby
# Created: 10/8 Dalton Flanagan

#-----------------------------------------------------#
# scrape will scrape an OSU College of Engineering
# department's list of events and parse them in an
# array of Event objects
#
# Created: 10/4 by Dalton Flanagan
# Modified: 10/4 by Dalton Flanagan (Added nil handling)
# Modified: 10/4 by Dalton Flanagan (Added desc scraping)
# Modified: 10/6 by Allen Yu (Added department variable)
# Modified: 10/7 by Dalton Flanagan (refactored dept parse into regex)
#-----------------------------------------------------#
def scrape(url)
	# Create a Mechanize scraper and retrieve the requested url
	scraper = Mechanize.new
	page = scraper.get(url)

	# Find all events on the page by CSS selector
	results = page.search ".osu-widget-article"

	# Get the department from the URL
	dept = /(\w+).osu.edu/.match(url)[1]

	# Map the HTML snippets into event objects
	events = results.map{ |r| Event.new(r.at(".osu-title"), r.at(".osu-date"), r.at(".osu-teaser"), dept) }
end

