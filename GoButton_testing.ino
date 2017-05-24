/*
 GoButton:
 This sketch sends a "Go" command to an ETC EOS console when a switch is switched

 The test is based on a lamp switch, and will send the command every time the state 
 of the switch changes.  You will need to modify the logic to implement momentary switch
 operation.

 created 2016.01.02
 by John Newman

 This code is in the public domain.
 */

// #define DEBUG 1

#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h>         // UDP library from: bjoern@cs.stanford.edu 12/30/2008

#include <OSCMessage.h>

// Enter a MAC address and IP address for your Arduino below.
// This will be used as a fallback if DHCP fails
byte myMac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
byte myIp[] = {192, 168, 0, 105};
byte myDns[] = {192, 168, 0, 1};
byte myGateway[] = {192, 168, 0, 1};
byte mySubnet[] = {255, 255, 254, 0};

// Enter OSC recipient's info
byte EosIp[] = {192, 168, 0, 26};  // IP Address of EOS Console
unsigned int EosPort = 8000;        // OSC Rx port in EOS Show Settings

// Enter port to listen for OSC strings
unsigned int myRxPort = 8001;      // local port to listen on

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

// physical pin assignments
const byte triggerCount = 4;
const byte trigger[] = {2, 7, 8, 9};
const byte R = 6;
const byte G = 5;
const byte B = 3;
const byte led[] = {R, G, B};

// buttonState and lastState, used to determine the switch being flipped
byte triggerState[] = {0, 0, 0, 0};
byte lastState[] = {0, 0, 0, 0};
// byte triggerArmed[] = {0, 0, 0}; // uncomment when arming is implemented
byte triggerArmed[] = {1, 1, 1, 1}; // set connected switches to 1, others 0

int currentCue = 0;   // used to easily arm inputs at specific points in a show

void setup() {
  
  #ifdef DEBUG
  // initialize serial communications for troubleshooting
  Serial.begin(9600);
  #endif

  // not using SD card - explicitly disable
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  
  // initialize input pin states
  for (byte l=0; l<triggerCount; ++l){
    pinMode(trigger[l], INPUT_PULLUP);
    triggerState[l] = digitalRead(trigger[l]);
    lastState[l] = triggerState[l];
    #ifdef DEBUG
    Serial.print("Trigger ");
    Serial.print(l);
    Serial.print(" state: ");
    Serial.println(triggerState[l]);
    #endif
  }

	// initialize LED pin states and flash in turn  
  for (byte l=0; l<3; ++l){
    pinMode(led[l], OUTPUT);
    digitalWrite(led[l], HIGH);
    delay(100);
    digitalWrite(led[l], LOW);
    delay(100);
  }
  
  // start the Ethernet and UDP:
  #ifdef DEBUG
  Serial.println("Trying DHCP");
  #endif

  if (Ethernet.begin(myMac) == 0) {
    #ifdef DEBUG
    Serial.println(F("Failed to configure Ethernet using DHCP"));
    #endif
    
    Ethernet.begin(myMac, myIp, myDns, myGateway, mySubnet);  //static config
  } else {
    digitalWrite(B, HIGH);
    delay(50);
    digitalWrite(B, LOW);
    delay(75);
    digitalWrite(B, HIGH);
    delay(50);
    digitalWrite(B, LOW);
    delay(75);
    digitalWrite(B, HIGH);
    delay(50);
    digitalWrite(B, LOW);
  }
  
  #ifdef DEBUG
  Serial.print("Ethernet configuration complete.  IP: ");
  Serial.println(Ethernet.localIP());
  #endif

  // Set up OSC listening port
  if(!Udp.begin(myRxPort)){
    #ifdef DEBUG
    Serial.println(F("Failed opening UDP Socket"));
    #endif
    failure;
  }

  #ifdef DEBUG
  Serial.println("Setup complete");
  #endif

  digitalWrite(R, HIGH);
  delay(50);
  digitalWrite(R, LOW);
  delay(75);
  digitalWrite(G, HIGH);
  delay(50);
  digitalWrite(G, LOW);
  delay(75);
  digitalWrite(B, HIGH);
  delay(50);
  digitalWrite(B, LOW);
  delay(75);
  digitalWrite(R, HIGH);
  delay(50);
  digitalWrite(R, LOW);
  delay(75);
  digitalWrite(G, HIGH);
  delay(50);
  digitalWrite(G, LOW);
  delay(75);
  digitalWrite(B, HIGH);
  delay(50);
  digitalWrite(B, LOW);
}

void loop() {

  /* Arming code
   *  if we have a packet
   *    if packet displays the current cue
   *      switch cases for arming triggers
   */
   

  for(byte l=0; l<triggerCount; ++l){
    // read the state of the trigger:
    triggerState[l] = digitalRead(trigger[l]);
 
    if (triggerState[l] != lastState[l]) {
      #ifdef DEBUG
      Serial.print("Trigger ");
      Serial.print(l);
      Serial.print(" state changed to ");
      Serial.println(triggerState[l]);
      #endif

      if (triggerArmed[l]) {
        // send command to console
        OSCMessage msg("/eos/key/go 0");
        #ifdef DEBUG
        Serial.println("Message content created");
        #endif
    
        Udp.beginPacket(EosIp, EosPort);
        #ifdef DEBUG
        Serial.println("Packet begun");
        #endif
        msg.send(Udp);
        #ifdef DEBUG
        Serial.println("Message added");
        #endif
        if(!Udp.endPacket()){
          #ifdef DEBUG
          Serial.println(F("Failed ending packet"));
          #endif
        }
        #ifdef DEBUG
        Serial.println("Packet ended");
        Serial.println("Message sent");
        #endif
    
        msg.empty();
        #ifdef DEBUG
        Serial.println("Buffer flushed");
        #endif
    
        // give feedback
        #ifdef DEBUG
        Serial.println("Giving feedback");
        #endif
        sendBlink(l);
      }
      // update lastState to current triggerState
      lastState[l] = triggerState[l];
    }
  }
  delay(5);
}

void sendBlink(int l){
/*
  for(l; l>0; --l){
    digitalWrite(G, HIGH);
    delay(50);
    digitalWrite(G, LOW);
    delay(50);
  }
*/
  
  digitalWrite(G, HIGH);
  delay(100);
  digitalWrite(G, LOW);
}

void failure(){
   for(;;){
    digitalWrite(led[R], HIGH);
    delay(500);
    digitalWrite(led[R], LOW);
    delay(500);
   }
}



