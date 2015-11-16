#!/usr/bin/env ruby
# This file imports components under test and runs Unit and Integration testing on them
# Created: 10/8 Dalton Flanagan

require 'mechanize'
require_relative 'event'

require_relative 'scrape'
require_relative 'get_json'

### Unit Testing

# scrape() Unit Tests
puts "--- BME --- \n" + scrape("https://bme.osu.edu/events/").to_s + "\n----------------\n" 
puts "--- CBE --- \n" + scrape("https://cbe.osu.edu/events/").to_s + "\n----------------\n" 
puts "--- CSE --- \n" + scrape("https://cse.osu.edu/events/").to_s + "\n----------------\n" 
puts "--- ECE --- \n" + scrape("https://ece.osu.edu/events/").to_s + "\n----------------\n" 
puts "--- MAE --- \n" + scrape("https://mae.osu.edu/events/").to_s + "\n----------------\n" 
puts "--- MSE --- \n" + scrape("https://mse.osu.edu/events/").to_s + "\n----------------\n" 

# get_json() Unit Tests
get_json("https://bme.osu.edu/events/")
get_json("https://cbe.osu.edu/events/")
get_json("https://cse.osu.edu/events/")
get_json("https://ece.osu.edu/events/")
get_json("https://mae.osu.edu/events/")
get_json("https://mse.osu.edu/events/")
