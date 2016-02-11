#!/usr/bin/env ruby
# Created: 10/8 Dalton Flanagan

#-----------------------------------------------------#
# The Event class represents a calendar event with a 
# title and timedate (when the event is)
#
# Created: 10/4 by Dalton Flanagan
# Modified: 10/4 by Dalton Flanagan (Added nil handling)
# Modified: 10/4 by Dalton Flanagan (Added desc member)
# Modified: 10/6 by Allen Yu (Added department)
#-----------------------------------------------------#
class Event
	def initialize(what_event, when_event, more, department)
		@title = what_event.text unless what_event == nil
		@timedate = when_event.text unless when_event == nil
		@desc = more.text unless more == nil
		@dept = department
	end

	attr_accessor :title
	attr_accessor :timedate
	attr_accessor :desc
	attr_accessor :dept
end
