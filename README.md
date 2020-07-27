# URBuddy
Wireless take-home uroflowmetry device

## Context
For diagnosis of certain urinary disorders, such as LUTS and nocturnal polyuria, doctors need patients to record a Frequency-Volume chart (FVC) or a bladder diary over several days. 
Patient compliance then becomes an issue, especially with the elderly (for whom these conditions are most prevalent anyway). 

The voiding volume and the timestamps of each urination event are of primary interest here, because this information generates the FVC. 
URBuddy is a take-home uroflowmetry device that can be installed on a home toilet and virtually requires no setup or maintenance as long as the device is powered on. 
Once activated, the device continuously measures fluid flow rate in the toilet and updates voiding events (volume and timestamp) on a Google Sheet in real-time.
In some cases, flow rate may also be desired. The device uses flow rate to calculate voiding volume and therefore flow rate information is hidden but available. 

The simplicity of the device inherently eliminates chances of patient noncompliance. 

## Usage
The code was developed and tested on the Arduino UNO WiFi Rev2 and the Arduino MKR1000 WiFi. 
For Arduino boards with no inbuilt WiFi, consider connecting an ESP8266 microchip (tested) or equivalent.

In the Arduino sketch, modify the following sections accordingly before uploading:
```c++
#include <WiFiNINA.h>

const char WEBSITE[] = "api.pushingbox.com"; //pushingbox API server
const String devid = "***"; //device ID on Pushingbox for our Scenario

const char* MY_SSID = "***";  // change SSID and password as per necessary to WiFi or hotspot, network band 2.4Ghz
const char* MY_PWD =  "***";
```
and
```c++
int sensorPin = 2;                    // pin number for sensor input and interrupt designation
byte sensorInterrupt = 2;
```
and, if using the MKR1000, this line may be commented out: 
```c++
client.stop(); // Close connection to free socket. Without this, connection will hang after 4 sent entries
```
then modify the JavaScript file(s) with target Google Sheet ID before deploying as a web app:
```javascript
... // for example,
  else {
    var id = '***'; // Google spreadsheet ID
    var sheet = SpreadsheetApp.openById(id).getSheetByName("Sheet2");
...
```

The device now detects and records urination events automatically. 

## Notes
The live project included the actual hardware (3D printed funnel with a screw-on for the YF-S402B small-volume Hall effect flowmeter and latches for securing onto toilet) as well as a custom Android mobile app which served as a user interface for accessing data, adding manual entries (in case of urination events outside the house), and visualizing the data (for clinicians' use). These are not shown. This project was done in collaboration with Changi General Hospital, Singapore.
