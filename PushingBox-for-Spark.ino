////
//
// General code from http://www.pushingbox.com for Spark Core v1.0
//
////

  /////////////////
 // MODIFY HERE //
/////////////////
// Your secret DevID from PushingBox.com. You can use multiple DevID  on multiple Pin if you want
const char * DEVID1 = "Your_DEVID_here";         // Scenario: "The mailbox is open"

// Numeric Pin where you connect your switch
uint8_t pinDevid1 = D0;                     // Example: the mailbox switch is connect to the Pin D0

// Debug mode
boolean DEBUG = true;
  //////////////
 //   End    //
//////////////

int LED = D7;                               // Built in LED
const char * serverName = "api.pushingbox.com";   // PushingBox API URL
boolean pinDevid1State = false;             // Save the last state of the Pin for DEVID1
TCPClient client;

void setup() {
    Serial.begin(9600);                     // Start the USB Serial
    pinMode(pinDevid1, INPUT_PULLUP);       // sets pin as Input PullUp, using the internal resistor. Connect PIN to GND.
    pinMode(LED, OUTPUT);                   // sets LED as output
    delay(1000);

    // Prints out the network parameters over Serial
    if(DEBUG){
        Serial.print("SSID: ");
        Serial.println(Network.SSID());
        Serial.print("Core IP: ");
        Serial.println(Network.localIP());    
        Serial.print("Gateway: ");
        Serial.println(Network.gatewayIP());
        Serial.print("Mask: ");
        Serial.println(Network.subnetMask());
        Serial.print("WiFi RSSI: ");
        Serial.println(Network.RSSI());
    }
}

void loop() {

    ////
    // Listening for the pinDevid1 state
    ////
    if (digitalRead(pinDevid1) == HIGH && pinDevid1State == false) // switch on pinDevid1 is ON 
    {
        if(DEBUG){Serial.println("pinDevid1 is HIGH");}
        pinDevid1State = true;
        // Sending request to PushingBox when the pin is HIGH
        sendToPushingBox(DEVID1);
    }
    if (digitalRead(pinDevid1) == LOW && pinDevid1State == true) // switch on pinDevid1 is OFF
    {
        if(DEBUG){Serial.println("pinDevid1 is LOW");}
        pinDevid1State = false;
        // Sending request to PushingBox when the pin is LOW
        //sendToPushingBox(DEVID2);                               // Here you can run an other scenario by creating a DEVID2 variable
    }
    
}

void sendToPushingBox(const char * devid)
{
    digitalWrite(LED, HIGH);          // sets the LED on
    Serial.print("closing... ");
    client.stop();
    if(DEBUG){Serial.print("connecting... ");}
    if (client.connect(serverName, 80)) {
        if(DEBUG){Serial.println("connected");}
        client.print("GET /pushingbox?devid=");
        client.print(devid);
        client.println(" HTTP/1.1");
        client.print("Host: ");
        client.println(serverName);
        client.println("User-Agent: Spark");
        //client.println("Connection: close");
        client.println();
        client.flush();
        if(DEBUG){
            Serial.print("sent! - RSSI:");
            Serial.println(Network.RSSI());
            Serial.println("");
        }
        digitalWrite(LED, LOW);           // sets the LED off
    } 
    else{
        digitalWrite(LED, HIGH);          // sets the LED on. If always on, last connexion was failed
        if(DEBUG){Serial.println("connection failed");}
    }
}