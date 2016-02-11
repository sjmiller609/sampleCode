# Project 3
### Web Scraping
* This was a group project for CSE 3901: Web Apps

### How to Run
* Our Ruby script requires the `mechanize` gem. Make sure this is installed via `ruby gem mechanize`.
* To try out the project, run the server.rb file. This will start the server threads, which include the TCP GET server and the web scraper threads. Scraped data from the server will be stored in .json files in the API directory. 
* These json files are updated continuously as the server runs. They are named based on the what departments' calendar the data came from.
* When the server is started on your machine, you may try out the front end by entering http://localhost:1337/ in a browser.

### Roles
* Overall Project Manager: Kevin Payravi
* Coding Manager: Steven Miller
* Testing Manager: Eric Yu
* Documentation: Dalton Flanagan

### Contributions
Please list who did what for each part of the project.
Also list if people worked together (pair programmed) on a particular section.
* **Kevin Payravi** - jsonProcessor (JavaScript file to parse JSON API and output data to index.html) / Styling
* **Steven Miller** - Setting up the server
* **Eric Yu** - Frontend (index.html and styling)
* **Dalton Flanagan** - Ruby scraper
* **Allen Yu** - Creating JSON templates / Adjusting the Ruby scraper

### Description
This project scrapes several Ohio State Engineering Departments' calendars for upcoming events. These calendars are listed [here](https://github.com/cse3901-osu-2015au-34634/web-scraping-the-frameworkers/blob/master/urls.txt).
* The actual web scraping part of the project is in a function called "scrape" in server.rb.
* This project includes a server that scrapes calendar event data from the web then formats it in json files. The server serves these files to javascript embedded client-side and event dpata is rendered.
* The server is listening on port 1337 for GET requests.
* To try out the project, run the server.rb file. This will start the server threads, which include the TCP GET server and the web scraper threads. Scraped data from the server will be stored in the API directory in .json files.
* These json files are updated continuously as the server runs. They are named based on the what departments' calendar the data came from.
* When the server is started on your machine, you may try out the front end by entering http://localhost:1337/ in a browser. The server is a simple file server that services GET requests.
* The default page, index.html, has embedded JavaScript that requests .json files for rendering in the client's browser.

