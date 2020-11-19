#include <iostream>         // for the input/output
#include <stdlib.h>         // for the getenv call
#include <sys/sysinfo.h>    // for the system uptime call
#include <cgicc/Cgicc.h>    // the cgicc headers
#include <cgicc/CgiDefs.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>
#include "GPIO.h"
#define BUTTON_GPIO "/sys/class/gpio/gpio46/"
using namespace std;
using namespace cgicc;
using namespace exploringBB;

void writeGPIO(string filename, string value){
   fstream fs;
   string path(BUTTON_GPIO);
   fs.open((path + filename).c_str(), fstream::out);
   fs << value;
   fs.close();
}

int main(){
   GPIO button(46);
   Cgicc form;                      // the CGI form object
   string cmd;                      // the Set LED command
   writeGPIO("direction", "in");
   writeGPIO("mode", "gpio_pu");
   // get the state of the form that was submitted - script calls itself
   bool isStatus = form.queryCheckbox("status");
   form_iterator it = form.getElement("cmd");  // the radio command
   if (it == form.getElements().end() || it->getValue()==""){
      cmd = "off";                     // if it is invalid use "off"
   }
   else { cmd = it->getValue(); }      // otherwise use submitted value
   char *value = getenv("REMOTE_ADDR");    // The remote IP address
 
   // generate the form but use states that are set in the submitted form
   cout << HTTPHTMLHeader() << endl;       // Generate the HTML form
   cout << html() << head() << title("CPE 422/522 Reading a Pushbutton as Digital Input") << head() << endl;
   cout << body() << h1("CPE 422/522 Reading a Pushbutton as Digital Input") << endl;
   cout << body() << h2("Reading a Pushbutton on gpio 46 via a Web Browser") << endl;
   cout << "<form action=\"/cgi-bin/read_button.cgi\" method=\"POST\">\n";
   cout << "<input type=\"submit\" value=\"Read Button\" />";
   cout << "</div></form>";
   cout << body() << h2("Push Button State") << endl;
   
 
   // process the form data to change the LED state
   if (button.getValue() == HIGH) cout << body() << h3("Button Not Pressed") << endl;              // turn on
   else if (button.getValue() == LOW) cout << body() << h3("Button Pressed") << endl;     // turn off
   else cout << "<div> Invalid command! </div>";        // not possible
   cout << "<div> The CGI REMOTE_ADDR value is " << value << "</div>";
   cout << body() << html();
   return 0;
}
