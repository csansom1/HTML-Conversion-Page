Name: Connor Sansom                   Student ID: 0871933
Date: 22/11/2016                      Course: CIS*2750
Assignment: A4

***********
Compiling:
***********

- To compile A4: "make"
- Required files are a4.c, db.c, listio.c, listio.h

NOTE: a4 and db MUST be compiled on the virtual machine

***********
Running:
***********

- Required files are a4, liblistio.a, a4.py, listio.h, files directory, a4.php, db
  - All must be in the active directory on the web server, with all required files inside
- The permissions must be set to +rx for executables and +rwx for directories
  - if that doesn’t work, try "chmod -R 777 <active directory>"
- Load web page in browser

NOTE: It is strongly recommended that DB Store only be used for files that are 2KB or smaller, anything larger should be stored using Convert.

- a4 converts text files into html and either stores them in a database or creates the html file
  - to create an html file, type: ./a4 <path to file> <any string>
    - eg: ./a4 test.txt 1
    - the contents of the last string doesn’t matter, its existence indicates that an html file is to be created
  - to store on the database, type: ./a4 <path to file>
    - eg: ./a4 test.txt
    - this also prints the file name and contents to stdout

- db must be run by typing "./db" followed by a command, use "-help" flag to see all commands

***********
Known Limitations and Flaws:
***********

- processStrings(-) only adds final HTML tags for headers that contain exactly 1 string (this is how text files are read in)
- Files may only be uploaded one at a time
- .info files must be uploaded BEFORE CONVERSION for formatting to be applied
- After clicking the links and using the back button, the browser may ask for resubmission 
  - Either click the back button until the webpage is displayed or retype the URL
- Files stored on the database are displayed on the current page
  - Refresh to return to the main page
