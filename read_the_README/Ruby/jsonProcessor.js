/*
jsonProcessor.js makes GET requests to the JSON files in the API.
It formats and sorts the data, and then outputs the information
to index.html every time the "submit" button if clicked by the client.

Created: 10/6 by Kevin Payravi
Modified: 10/7 by Kevin Payravi (Re-wrote method to get JSON files, format them, organize them, and output to index.html)
Modified: 10/7 by Kevin Payravi (Adding comments)
*/

/*
getData() is the primary function of jsonProcessor.js.
It takes in the data through GET requests to the API, organizes it, and outputs to index.html.

Created: 10/6 by Kevin Payravi
Motified: 10/7 Kevin Payravi (Re-wrote method to get JSON files, format them, organize them, and output to index.html)
*/
function getData() {
	// Declare variables:
	var calendarNames = ["bme", "cbe", "cse", "ece", "mae", "mse"];
	var checkedCalendars = [];
	var tempEvents = [];
	var events = [];
	
	// Iterate through calendar names; see which calendars have been checked by the client on index.html:
	for(i = 0; i < calendarNames.length; i++) {
		if(document.getElementById(calendarNames[i]).checked) {
			checkedCalendars.push(calendarNames[i]);
		}
	}

	// Start iterating through calendars:
	for(j = 0; j < checkedCalendars.length; j++) {
		// Make GET call to JSON files on server:
		var xmlHttp = new XMLHttpRequest();
		xmlHttp.open("GET", "API/" + checkedCalendars[j] + ".json", false);
		xmlHttp.send(null);

		// Store JSON temporarilly in jsonResponse:
		var jsonResponse = JSON.parse(xmlHttp.responseText);
		
		// Set tempEvents equal to the JSON object:
		tempEvents = jsonResponse.events.slice();
		
		// Convert start date in JSON file to YYYY-MM-DD format for sorting; push the tempEvent array onto the master events array:
		for(k = 0; k < tempEvents.length; k++) {
			tempEvents[k].convertedDate = convertDate(tempEvents[k].timedate.substring(0, tempEvents[k].timedate.indexOf(',') + 6));
			events.push(tempEvents[k]);
		}
	}

	// Sort events by date:
	events.sort(function(a, b) {
		return new Date(a.convertedDate) - new Date(b.convertedDate);
	});

	// Format info for each event for output to index.html:
	var outputString = "";
	for(k = 0; k < events.length; k++) {
		// Get the event's department and title:
		outputString = outputString + "<b>" + events[k].dept.toUpperCase() + " - " + events[k].title + "</b><br />"; 
		// If the event has a description, get it:
		if(events[k].desc != "") {
			outputString = outputString + events[k].desc + "<br />";
		}
		// Add the event's start date:
		outputString = outputString + events[k].timedate + "<br /><br />";
	}
	
	// If no calendar has been selected by the client, say so; otherwise, output!
	if(events.length == 0) {
		document.getElementById("events").innerHTML = "No calendars selected! Please select at least one above.";
	} else {
		document.getElementById("events").innerHTML = outputString;
	}
}

/*
convertDate(date) takes "date" in the form "MMM DD, YYYY", converts it into YYYY-MM-DD format, and returns it.
The purpose of this is to make the dates sortable for outputting at index.html.

Created: 10/7 by Kevin Payravi
*/
function convertDate(date) {
	// Grab month, day, and year info from date string found in online calendars:
	month = date.substring(0, 3);
	day = date.substring(4, 6);
	day = day.replace(/\,/g,"");
	year = date.substring(date.indexOf(",") + 2);

	// Convert date info into proper format (YYYY-MM-DD):
	var months = ["Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"];
	month = months.indexOf(month) + 1;
	if(month < 10) {
		month = '0' + month;
	}
	if(day < 10) {	
		day = '0' + day;
	}
	
	// Format date into single string with dashes:
	var formattedDate = year + '-' + month + '-' + day;
	
	return formattedDate;
}
