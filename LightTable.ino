#include <Adafruit_SleepyDog.h>
//#include <Adafruit_NeoPxl8.h>
//#include <MemoryFree.h>
#include <SPI.h>
#include <Adafruit_NeoPixel.h>

#include <WiFiNINA.h>
//#include <WiFi101.h>
#include <WiFiUDP.h>
#include "arduino_secrets.h" 
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;		// your network SSID (name)
char pass[] = SECRET_PASS;    	// your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;	// the WiFi radio's status


long prevWifiCheckMillis = 0;
long wifiInterval = 300000; // Check the wifi connection every minutes



//struct EvenDataPacket{
//  uint32_t counter;
//  float batteryVoltage;
//  uint8_t cubeID;
//  uint8_t side;
//} eventData;

// Dont put this on the stack:
//uint8_t eventBuffer[sizeof(eventData)];
//uint8_t from;
//uint8_t len = sizeof(eventData);


/*
// -------------- UDP Ethernet Variables ----------------------

#define SPIWIFI       SPI  // The SPI port
#define SPIWIFI_SS    13   // Chip select pin
#define ESP32_RESETN  12   // Reset pin
#define SPIWIFI_ACK   11   // a.k.a BUSY or READY pin
#define ESP32_GPIO0   10

IPAddress ip(10, 10, 212, 201);		// will change when moved to new VLAN
//IPAddress ip(192, 168, 0, 101);		// will change when moved to new VLAN
unsigned int localPort = 8888;      // local port to listen on

byte gateway[] = {10, 10, 212, 5}; // was .5 or .15
byte subnet[] = {255, 255, 255, 0};
byte dnsServer[] = {10, 10, 100, 20}; // byte dnsServer[] = {10, 10, 100, 20};
IPAddress splunkIp(10, 10, 100, 150);
unsigned int splunkPort = 4514;
String pingHost = "10.10.212.129";
int pingResult;

// buffers for receiving and sending data
char packetBuffer[255]; 							//buffer to hold incoming packet,
const char  AckBuffer[] = "acknowledged";       	// a string to send back
const char  DenyBuffer[] = "Error, no release.";    // a string to send back
const char pigDrop[] = "pigDrop"; 					// command string to compare rx to
const char cEquals[] = "c=";
const char tEquals[] = " t=";
const char sEquals[] = " s=";
const char gEquals[] = " g=";
const char bEquals[] = " b=";
const char fEquals[] = " f=";

char  ReplyBuffer[] = "acknowledged";       // a string to send back


WiFiUDP Udp;


void setupWiFi()
{
	//Configure pins for Adafruit ATWINC1500 Feather

	// Set up the pins!
	WiFi.setPins(SPIWIFI_SS, SPIWIFI_ACK, ESP32_RESETN, ESP32_GPIO0, &SPIWIFI);

	// check for the WiFi module:
	while (WiFi.status() == WL_NO_MODULE) {
		Serial.println("Communication with WiFi module failed!");
		// don't continue
		delay(1000);
	}
	String fv = WiFi.firmwareVersion();
	Serial.println(fv);
	if (fv < "1.0.0") {
		Serial.println("Please upgrade the firmware");
		while (1) delay(10);
	}
	Serial.println("Firmware OK");

	// print your MAC address:
	byte mac[6];
	WiFi.macAddress(mac);
	Serial.print("MAC: ");
	printMacAddress(mac);


	// attempt to connect to WiFi network:
	while ( status != WL_CONNECTED) {
		Serial.print(F("Attempting to connect to WPA SSID: "));
		Serial.println(ssid);
		
		WiFi.config(ip, dnsServer, gateway, subnet); 
		// Connect to WPA/WPA2 network:
		status = WiFi.begin(ssid, pass);

		// wait 10 seconds for connection:
		for (int i=0; i>10; i++) {
			delay(1000);
			Watchdog.reset();
		}
	}

	// you're connected now, so print out the data:
	Serial.print(F("You're connected to the network"));
	printCurrentNet();
	printWiFiData();

	Udp.begin(localPort);
	Udp.beginPacket(splunkIp, splunkPort);
	Udp.write("c=7 reconnect=1");
	Udp.endPacket();


}
*/

// NeoPixel Setup--------------------------------------------------------------

// Which pin on the Arduino is connected to the NeoPixels?
#define NEO_PIN_1       9 // On Trinket or Gemma, suggest changing this to 1
#define NEO_PIN_2       5

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 63 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel wallPixels(NUMPIXELS, NEO_PIN_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel endPixels(NUMPIXELS, NEO_PIN_2, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 50 // Time (in milliseconds) to pause between pixels

long colorChangeInterval = 10000;
long prevColorChangeMillis = 0;
int colorListPosition = 0;

const uint32_t red = wallPixels.Color(255, 0, 0);
const uint32_t rose = wallPixels.Color(255, 0, 128);
const uint32_t magenta = wallPixels.Color(255, 0, 255);
const uint32_t violet = wallPixels.Color(128, 0, 255);
const uint32_t blue = wallPixels.Color(0, 0, 255);
const uint32_t azure = wallPixels.Color(0, 128, 255);
const uint32_t cyan = wallPixels.Color(0, 255, 255);
const uint32_t springgreen = wallPixels.Color(0, 255, 128);
const uint32_t green = wallPixels.Color(0, 255, 0);
const uint32_t chartreuse = wallPixels.Color(128, 255, 0);
const uint32_t yellow = wallPixels.Color(255, 255, 0);
const uint32_t orange = wallPixels.Color(255, 128, 0);

const int colorList[] = {red, rose, magenta, violet, blue, azure, cyan, springgreen, green, chartreuse, yellow, orange};

// Returns the Red component of a 32-bit color
uint8_t Red(uint32_t color)
{
    return (color >> 16) & 0xFF;
}

// Returns the Green component of a 32-bit color
uint8_t Green(uint32_t color)
{
    return (color >> 8) & 0xFF;
}

// Returns the Blue component of a 32-bit color
uint8_t Blue(uint32_t color)
{
    return color & 0xFF;
}

void colorFade(Adafruit_NeoPixel strip, uint8_t r, uint8_t g, uint8_t b, uint8_t wait) {
  for(uint16_t i = 0; i < strip.numPixels(); i++) {
      uint8_t startR, startG, startB;
      uint32_t startColor = strip.getPixelColor(i); // get the current colour
      startB = startColor & 0xFF;
      startG = (startColor >> 8) & 0xFF;
      startR = (startColor >> 16) & 0xFF;  // separate into RGB components

      if ((startR != r) || (startG != g) || (startB != b)){  // while the curr color is not yet the target color
        if (startR < r) startR++; else if (startR > r) startR--;  // increment or decrement the old color values
        if (startG < g) startG++; else if (startG > g) startG--;
        if (startB < b) startB++; else if (startB > b) startB--;
        strip.setPixelColor(i, startR, startG, startB);  // set the color
        strip.show();
        // delay(1);  // add a delay if its too fast
      }
      delay(1000);
  }
}

void setup() 
{
	delay(1000);
	wallPixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  endPixels.begin();
	Serial.begin(9600);
	delay(5000);
	//while (!Serial);

	//pinMode(WIFI_SHIELD_CS, OUTPUT);

/*	pinMode(ESP32_RESETN,OUTPUT);
	digitalWrite(ESP32_RESETN,LOW);
	delay(500);
	digitalWrite(ESP32_RESETN,HIGH);
	setupWiFi();

	Serial.print(F("Splunk IP: "));
	Serial.println(splunkIp);
	Serial.print(F("Splunk Port: "));
	Serial.println(splunkPort);
	Serial.print(F("Pinging "));
	Serial.print(pingHost);
	Serial.print(F(": "));

	
	pingResult = WiFi.ping(pingHost);

	if (pingResult >= 0) {
		Serial.print(F("SUCCESS! RTT = "));
		Serial.print(pingResult);
		Serial.println(F(" ms"));
	} else {
		Serial.print(F("FAILED! Error code: "));
		Serial.println(pingResult);
	}

	Serial.println(F("Sending reboot=1"));
	Udp.beginPacket(splunkIp, splunkPort);
	Udp.write("c=0 reboot=1");
	Udp.endPacket();
  */

	int countdownMS = Watchdog.enable(4000);
	Serial.print(F("Enabled the watchdog with max countdown of "));
	Serial.print(countdownMS, DEC);
	Serial.println(F(" milliseconds!"));
	Serial.println();
 
} // END SETUP //

void loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis - prevColorChangeMillis >= colorChangeInterval) {
    if (colorListPosition < 11) {
      colorListPosition++;
    } else {
      colorListPosition = 0;
    }
    
    //for(uint16_t i = 0; i < strip.numPixels(); i++) {
      uint8_t startR, startG, startB, endR, endG, endB;
      uint32_t startColor = endPixels.getPixelColor(0); // get the current colour
      startB = startColor & 0xFF;
      startG = (startColor >> 8) & 0xFF;
      startR = (startColor >> 16) & 0xFF;  // separate into RGB components
      endB = colorList[colorListPosition] & 0xFF;
      endG = (colorList[colorListPosition] >> 8) & 0xFF;
      endR = (colorList[colorListPosition] >> 16) & 0xFF;  // separate into RGB components

      if ((startR != endR) || (startG != endG) || (startB != endB)){  // while the curr color is not yet the target color
        if (startR < endR) startR++; else if (startR > endR) startR--;  // increment or decrement the old color values
        if (startG < endG) startG++; else if (startG > endG) startG--;
        if (startB < endB) startB++; else if (startB > endB) startB--;
        endPixels.fill(startR, startG, startB);  // set the color
        wallPixels.fill(startR, startG, startB);  // set the color
        endPixels.show();
        wallPixels.show();
        delay(100);  // add a delay if its too fast
      }
      //delay(100);
  //}

    
    //endPixels.fill(colorList[colorListPosition]);
    //wallPixels.fill(colorList[colorListPosition]);
    //endPixels.show();
    //wallPixels.show();
    prevColorChangeMillis = millis();
  }
  Watchdog.reset();
} // END LOOP //

/*void readUDP()
{
//	selectWiFi();
	// if there's data available, read a packet
	int packetSize = Udp.parsePacket();
	if (packetSize)
	{
		// read the packet into packetBufffer
		Udp.read(packetBuffer, 255);
		Serial.println(F("Contents:"));
		Serial.println(packetBuffer);
	
		if(memcmp_P(packetBuffer, pigDrop, 7) == 0)
		{
			Serial.println(F("Pig Drop Command Recieved - Release the Pig Eggs!"));
//			selectRadio();
//			sendDispenseEvent();
			
//			selectWiFi();
			// send a reply, to the IP address and port that sent us the packet we received
			Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
			Udp.write(AckBuffer, sizeof(AckBuffer));
			Udp.endPacket();
	
			// send a "pigDrop" message to Splunk
			if (Udp.beginPacket(splunkIp, splunkPort) == 1) {
				Serial.println(F("Packet begun"));
			} else {
				Serial.println(F("ERROR BEGINNING PACKET!"));
			}
			Udp.write(pigDrop, sizeof(pigDrop));
			
			//Udp.write("A", 1);
			
			if(Udp.endPacket() == 1) {
				Serial.println(F("Packet ended/sent successfully"));
			} else {
				Serial.println(F("ERROR ENDING/SENDING PACKET!"));
			}

		}


		// Added by Jeff to clear out buffer.
		for (int ii = 0; ii < 255; ii++) // changed max from 24 to 64 5/9/19 Jeff
		{
			packetBuffer[ii] = 0;
		}


	}
}*/


/*
// ftoa from http://www.ars-informatica.ca/eclectic/ftoa-convert-a-floating-point-number-to-a-character-array-on-the-arduino/
void ftoa(float f, char *str, uint8_t precision) {
  uint8_t i, j, divisor = 1;
  int8_t log_f;
  int32_t int_digits = (int)f;             //store the integer digits
  float decimals;
  char s1[12];

  memset(str, 0, sizeof(str));  
  memset(s1, 0, 10);

  if (f < 0) {                             //if a negative number 
    str[0] = '-';                          //start the char array with '-'
    f = abs(f);                            //store its positive absolute value
  }
  log_f = ceil(log10(f));                  //get number of digits before the decimal
  if (log_f > 0) {                         //log value > 0 indicates a number > 1
    if (log_f == precision) {              //if number of digits = significant figures
      f += 0.5;                            //add 0.5 to round up decimals >= 0.5
      itoa(f, s1, 10);                     //itoa converts the number to a char array
      strcat(str, s1);                     //add to the number string
    }
    else if ((log_f - precision) > 0) {    //if more integer digits than significant digits
      i = log_f - precision;               //count digits to discard
      divisor = 10;
      for (j = 0; j < i; j++) divisor *= 10;    //divisor isolates our desired integer digits 
      f /= divisor;                             //divide
      f += 0.5;                            //round when converting to int
      int_digits = (int)f;
      int_digits *= divisor;               //and multiply back to the adjusted value
      itoa(int_digits, s1, 10);
      strcat(str, s1);
    }
    else {                                 //if more precision specified than integer digits,
      itoa(int_digits, s1, 10);            //convert
      strcat(str, s1);                     //and append
    }
  }

  else {                                   //decimal fractions between 0 and 1: leading 0
    s1[0] = '0';
    strcat(str, s1);
  }

  if (log_f < precision) {                 //if precision exceeds number of integer digits,
    decimals = f - (int)f;                 //get decimal value as float
    strcat(str, ".");                      //append decimal point to char array

    i = precision - log_f;                 //number of decimals to read
    for (j = 0; j < i; j++) {              //for each,
      decimals *= 10;                      //multiply decimals by 10
      if (j == (i-1)) decimals += 0.5;     //and if it's the last, add 0.5 to round it
      itoa((int)decimals, s1, 10);         //convert as integer to character array
      strcat(str, s1);                     //append to string
      decimals -= (int)decimals;           //and remove, moving to the next
    }
  }
}


void printWiFiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  printMacAddress(mac);

}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  printMacAddress(bssid);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}
*/
