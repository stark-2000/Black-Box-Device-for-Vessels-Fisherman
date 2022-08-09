
// TODO: implement a clearing mechanism for SIM Memory of Messages. when your sim storage is full,
// your GET will not work.

// TODO: Change the _SS_MAX_RX_BUFF to 256 to increase RX buffer.
// in SoftwareSerial.h
// Connect GPS Tx to Rx1 of pro mini
// VCC to 3.3v
// GND to GND
// Connect GSM Rx to 3
// Tx to 2
// GND to GND
// Before uploading, disconnect the TX of GPS

#include <TinyGPS.h>
#include <SoftwareSerial.h>

// Initialize software serial to communicate with GSM module
SoftwareSerial mySerial(2, 3); //TX,RX wrt GSM Module

//Initialize GPS to communicate with GPS module
TinyGPS gps;

// Variable to hold latitude data to be sent to server.
float latitude;

// Variable to hold longitude data to be sent to server.
float longitude;

// Configure your Device ID here, This will be root of this Device info
char DEVICEID[12] = "Car4364726";
char vesseltype[10] = "Cargo";
char country[10] = "India";
char IMOnumber[10] = "9611618";
char MMSInumber[12] = "563073800";
char Latitude[20] = "12.962682723";
char Longitude[20] = "80.211357116";
char Size[10] = "292 / 32m";
char GT[10] = "50698";
char VesselName[10] = "Titanic";
char OwnerName[10] = "Stark";
char Speed[10] = "0 kn";
char Course[10] = "354.6";
char Status[10] = "At Anchor";
char Distancefromcoast[10] = "7 NM";
char Tamperproof[12] = "Compromised";
char Tamperproof1[12] = "In Tact";
char emerg[12] = "Emergency";
char backup[40] = "External Power Supply Disconnected";
char backup1[40] = "Running on External power supply";

//SOS Switch:
int S1 = 12;
int val = 0;
int Prs (int x) { return digitalRead(x) == LOW; }
int del = 350;
int flag1 = 0;
int emer = 0;

//LDR:
const int ldrPin = A0;
int flag = 0;

// Prototype for gpsdump method
void gpsdump(TinyGPS &gps);

/* 
Function name: setup
Parameter name: None
P.Datatype: Not Applicable
P.Description: None
F.Description: Executes only once. Initialization is done here in this method.
*/

void setup() 
{
pinMode(ldrPin, INPUT);
pinMode(11, INPUT);
pinMode(S1, INPUT_PULLUP);
mySerial.begin(9600); // Setting the baud rate of GSM Module
Serial.begin(9600); // Setting the baud rate of Serial Monitor (Arduino)
delay(10000);

// Establish HTTP connection in GSM module.
setupHttpInit();
}

/* 
Function name: loop
Parameter name: none
P.Datatype: Not applicable
P.Description: None
F.Description: Main loop of the software
*/

void loop() 
{
eme(); //SOS
  
//LDR:  
int ldrStatus = analogRead(ldrPin);
if (ldrStatus > 200) 
{
flag = 1;
}
//LDR over


bool newdata = false;
unsigned long start = millis();
// Every 5 seconds we print an update
while (millis() - start < 5000)
{
eme(); //SOS
sendDataToServer(DEVICEID);
}

} //closing void loop

/* 
Function name: waitUntilResponse
Parameter name: delayMs
P.Datatype: int
P.Description: Delay until next event. Should be specified in milliseconds.
F.Description: Hold Execution until the GSM Module sends reponse.
*/

void waitUntilReponse(int delayMs)
{
// Delay in ms
while (mySerial.available() < 0) 
      { delay(delayMs); }
while (mySerial.available() > 0) 
      { Serial.write(mySerial.read()); }
}

/* 
Function name: setupHttpInit
Parameter name: None
P.Datatype : Not Applicable
P.Description: None
F.Description: Setting up HTTP and Bearer configuration for sending HTTP GET request.
*/

void setupHttpInit() 
{

//Serial.println(“Waiting for GSM to get ready”);
//wait till gsm responds ok
waitUntilReponse(500);

//Serial.println(“GSM is ready proceed to send commands”);
// Sending APN Settings

/*mySerial.write("AT+SAPBR =0,1\r\n");
delay(500);
waitUntilReponse(500);
delay(3000);
*/   
mySerial.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r\n"); //Setting the connection type to GPRS
delay(1000);
waitUntilReponse(500);

mySerial.println("AT+SAPBR=3,1,\"APN\",\"airtelgprs.com\"\r\n"); //Set the APN to “airtelgprs.com” for airtel or "www" for vodafone
delay(1000);
waitUntilReponse(500);

mySerial.println("AT+SAPBR=1,1\r\n"); //Enabling the GPRS
delay(3000);
waitUntilReponse(500);

//If Arduino switches off but GSM is on or some error occurs in initialising HTTP, the following AT command rectifies the error..
mySerial.write("AT+HTTPTERM\r\n"); //If HTTP mode already enabled, it would turn off and would be turned on in the next AT command (AT+HTTPINIT) 
delay(300);
waitUntilReponse(500);
delay(3000);

mySerial.println("AT+HTTPINIT\r\n"); //Enabling the HTTP mode
delay(300);
waitUntilReponse(500);
eme();
}

/* 
Function name: sendDataToServer
Parameter name: deviceId
P.Datatype: int
P.Description: Device ID to be sent to server for whose GPS Coordinates is being Uploaded.
F.Description: Setting up HTTP and Bearer configuration for sending HTTP GET request.
*/

void sendDataToServer(char deviceId[12]) 
{
mySerial.println("AT+HTTPINIT=?\r\n"); //Enabling HTTP mode
delay(300);
waitUntilReponse(500);

mySerial.println("AT+HTTPPARA=\"CID\",1\r\n"); //Setting up the HTTP bearer profile identifier
delay(300);

/*The below part is for When GSM Switches off but Arduino is on, Checks for ERROR response from software serial to AT+HTTPPARA command
  and runs setupHttpInit() fxn to setup GPRS and initialise HTTP
*/

const byte numChars = 50;
char receivedChars[numChars];
boolean newData1 = false;
char rc;
static byte ndx = 0;
char endMarker = '\n';
while (mySerial.available() > 0) 
      {
        rc = mySerial.read();
        if (rc != endMarker) 
            {
              receivedChars[ndx] = rc;
              ndx++;
              if (ndx >= numChars) 
              {ndx = numChars - 1;}
            }
        else 
            {
              receivedChars[ndx] = '\0'; // terminate the string
              ndx = 0;
              newData1 = true;
            }
        }
if (newData1 == true) 
          {
            Serial.println(receivedChars);
            if (receivedChars[0] == 'E' && receivedChars[1] == 'R')
                {
                  Serial.println("Rectifying Error...");
                  setupHttpInit();
                }
            newData1 = false;
          }
delay(1000);

mySerial.write("AT+HTTPPARA=\"URL\",\"http://ferrysmart1.000webhostapp.com/Ferry_Smart_Data.php?deviceid="); //Setting the url to the address of the webpage to be accessed
mySerial.print(deviceId);

mySerial.write("&vesseltype=");
mySerial.print(vesseltype);

mySerial.write("&country=");
mySerial.print(country);

mySerial.write("&IMONumber=");
mySerial.print(IMOnumber);

mySerial.write("&MMSINumber=");
mySerial.print(MMSInumber);

mySerial.write("&VesselName=");
mySerial.print(VesselName);

mySerial.write("&OwnerName=");
mySerial.print(OwnerName);

mySerial.write("&Size=");
mySerial.print(Size);

mySerial.write("&GT=");
mySerial.print(GT);

mySerial.write("&lat=");
mySerial.print(Latitude); 

mySerial.write("&lon=");
mySerial.print(Longitude);

mySerial.write("&Distancefromcoast=");
mySerial.print(Distancefromcoast);

mySerial.write("&speed=");
mySerial.print(Speed);

mySerial.write("&course=");
mySerial.print(Course);

mySerial.write("&Status=");
mySerial.print(Status);

if(flag==0)
{
mySerial.write("&Tamperproof=");
mySerial.print(Tamperproof1);
}

if(flag==1)
{
mySerial.write("&Tamperproof=");
mySerial.print(Tamperproof);
flag = 0;
}

if(digitalRead(11)==1)
{
mySerial.write("&backup=");
mySerial.print(backup);
}

if (digitalRead(11)==0)
{
mySerial.write("&backup=");
mySerial.print(backup1);
}

if(emer==1)
{
mySerial.write("&emergency=");
mySerial.print(emerg);
emer = 0;
}

mySerial.write(":80\"\r\n");
delay(500);
waitUntilReponse(500);

mySerial.write("AT+HTTPACTION=0\r\n"); //Starting the HTTP GET session
delay(3000);
waitUntilReponse(500);

mySerial.write("AT+HTTPREAD=0,10000\r\n"); //Acknowledgement for reading the data stored
delay(500);
waitUntilReponse(500);

}

/* 
Function name: gpsdump
Parameter name: &gps
P.Datatype : TinyGPS
P.Description: Pointer to the gps global variable that contains the GPS data.
F.Description: writes GPS data to latitude(Global variable), longitude(Global variable).
*/

void eme()
{
     if(Prs(S1)==1) 
     {
     emer = 1;
     delay(del);
     }
}
    
void gpsdump(TinyGPS &gps)
{
unsigned long age;
gps.f_get_position(&latitude, &longitude, &age); //function which gets lat and lon from tinygps library
}
