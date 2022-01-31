


/*

Main sau id: 2

lat/long
35.053327
-85.048468


train.jpeckham.com:5000
 */
#include <SPI.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h" 
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;                        // your network SSID (name)
char pass[] = SECRET_PASS;                        // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                                 // your network key Index number
int status = WL_IDLE_STATUS;                      // the Wifi radio's status
int trainstate = 0;
const char server[] = "train.jpeckham.com";
String postData;

WiFiClient client;
//int counter = 0;

void setup() {
  pinMode(2, INPUT); 
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WEP network, SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, keyIndex, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  

  // once you are connected :
  Serial.print("You're connected to the network");
  printCurrentNet();
  printWifiData();
  
}

void loop() {
  // check the network connection once every 5 seconds:
  // look into replacing with millis(), this is problematic (delay shuts down the arduino)
  printCurrentNet();
  // collect the state of the train
  trainstate = digitalRead(2);
  // send the train info to the JSON form
  postJSON(trainstate);
  // print the state of the train to debug
  Serial.println(trainstate);
  // wait 10 seconds to not DDOS the server
  delay(10000); // 10 seconds

}

void postJSON(int STATE){
  // increment the debug counter
  counter = counter+1;
  String response;
  Serial.println("making POST request");
  client.connect(server, 5000);
  Serial.println("connected");
  // connect to the server at the right port
  
  client.print("POST /state/2 HTTP/1.1 \n");
  client.print("Content-Type: application/json\n");
  client.print("Content-Length: 11\n");
  // the first half of the json form
  
  client.println();
 
 //depending on the status of the train, send the other half of the form
  if(STATE == 1){
    client.print("{\"state\":1}\n");
    
    Serial.println("we sent 1");
    Serial.println("count: "+String(counter));
    
  }
  else if (STATE == 0){
    client.print("{\"state\":0}\n");
    
    client.println();
    Serial.println("we sent 0");
    Serial.println(String(counter));
  }
}

//the remainder of the code is debug code from the example code this project is named after. feel free to ignore
void printWifiData() {
  // print your board's IP address:
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
