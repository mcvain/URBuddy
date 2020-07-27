#include <WiFiNINA.h>

const char WEBSITE[] = "api.pushingbox.com"; //pushingbox API server
const String devid = "***"; //device ID on Pushingbox for our Scenario

const char* MY_SSID = "***";  // change SSID and password as per necessary to WiFi or hotspot, network band 2.4Ghz
const char* MY_PWD =  "***";

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)

int sensorPin = 2;                    // pin number for sensor input and interrupt designation
byte sensorInterrupt = 2;
volatile unsigned int pulseCount = 0;
const float mL_per_pulse = 0.6621;    // calibrated volume per pulse
int flowrate_mL = 0;
int total_vol_mL = 0;
int volData = 0;

unsigned long currentMillis = 0;

void setup()
{

  // Initialize a serial connection for reporting values to the host
  Serial.begin(9600);

  while (!Serial)
  {
  ; // wait for serial port to connect. Needed for native USB port only
  }
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
  Serial.println("WiFi shield not present");
  // don't continue:
  while (true);
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED)
  {
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(MY_SSID);
  //Connect to WPA/WPA2 network.Change this line if using open/WEP network
  status = WiFi.begin(MY_SSID, MY_PWD);

  // wait 10 seconds for connection:
  delay(10000);
  }

  Serial.println("Connected to wifi");
  Serial.println(WiFi.status());

  // Set up the status LED line as an output
  pinMode(sensorPin, INPUT);
  // digitalWrite(sensorPin, HIGH);
  attachInterrupt(digitalPinToInterrupt(sensorInterrupt), pulseCounter, RISING);
}

/**
   Main program loop
*/
void loop()
{
  // the millis timer is counting in the period between attach and detach interrupt
  if((millis() - currentMillis) > 5000)  // does volume calculations every 5s, can be increased to 5 mins as the Arduino still keeps track of pulses in the meantime
  {
    detachInterrupt(digitalPinToInterrupt(sensorInterrupt));      //noInterrupts()
  
    Serial.print("Pulses per second: ");
    Serial.println(pulseCount);
    flowrate_mL = pulseCount * mL_per_pulse;
    Serial.print("mL per second: ");
    Serial.println(flowrate_mL);
    total_vol_mL = flowrate_mL + total_vol_mL;
    Serial.print("total volume is ");
    Serial.println(total_vol_mL);
    Serial.println();
  
    //note millis() run on interrupts, meaning when detachInterrupt is called, millis is not incrementing
    currentMillis = millis();
  
    if (total_vol_mL - volData > 0 && flowrate_mL == 0)     // condition to check for end of session before sending data, can be improved with timer
    {
      volData = total_vol_mL;                               // changes previous volume data to new volume
      
      // Check if any reads failed and exit early (to try again).
      if (isnan(volData))
      {
      Serial.println("Failed to read from sensor!");
      return;
      }
      
      Serial.println("\nSending Data to Server...");
      // if you get a connection, report back via serial:
      WiFiClient client;  //Instantiate WiFi object, can scope from here or Globally
      client.flush();
      client.stop(); // Close connection to free socket. Without this,connection will hang after 4 sent entries
      //API service using WiFi Client through PushingBox then relayed to Google
      if (client.connect(WEBSITE, 80))
      {
      client.print("GET /pushingbox?devid=" + devid + "&volData=" + (String) volData);
    
      // HTTP 1.1 provides a persistent connection, allowing batched requests
      // or pipelined to an output buffer
      client.println(" HTTP/1.1");
      client.print("Host: ");
      client.println(WEBSITE);
      client.println("User-Agent: MKR1000/1.0");
      //for MKR1000, unlike esp8266, do not close connection
    
      client.println();
      Serial.println("\nData Sent");
     }
    }
    pulseCount = 0; // Reset the pulse counter so we can start incrementing again
    attachInterrupt(digitalPinToInterrupt(sensorInterrupt), pulseCounter, RISING);
  }
}

/*
  Interrupt Service Routine
*/
void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
}
