//Resources for this project drawn from https://www.hackster.io/Aritro and Queen's FEAS 
//Modified by Nicole Ooi


#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

#define SER 8 
#define RCLK 5 
#define SRCLK 6 
//Look up table for common-anode seven-segment displays
// Each bit in a byte represents a pin (A-G, DP) on the 7 Segment Display
//Most Significant Bit = Pin DP, Least Significant Bit = A
byte digit[10] = {B11000000, B11111001, B10100100, 
                //   "0"         "1"        "2"
                  B10110000, B10011001, B10010010, B10000010, 
                //   "3"        "4"        "5"        "6"
                  B11111000, B10000000, B10010000}; 
                //   "7"        "8"        "9"        

void setup() { 
  // Setup Code only runs once: 

    //set up for RFID
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  
  //Define SER, SRCLK, RCLK pins as outputs for display 
  pinMode(SER, OUTPUT); 
  pinMode(RCLK, OUTPUT); 
  pinMode(SRCLK, OUTPUT); 

} 

 // Main code here, to run repeatedly:

void loop() { 

 //RFID Reader Part
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "87 87 A0 59" || content.substring(1) == "36 4B 04 85") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    delay(3000);
  }
 
 else   {
    Serial.println(" Access denied");
    delay(3000);
  }
 
 //Display Part
    if (content.substring(1) == "87 87 A0 59"){ //if it's blue
    int displayed = 1;
    digitalWrite(RCLK, LOW);  //hold last byte received
    shiftOut(SER, SRCLK, MSBFIRST, digit[displayed]); //start transmitting bits corresponding to Pins A-G and DP
    digitalWrite(RCLK, HIGH); //enable parallel output from register of new byte
    delay(10);               //one second delay to observe output
 
  }

else if (content.substring(1) == "36 4B 04 85"){ //if it's white
    int displayed = 2;
    digitalWrite(RCLK, LOW);  //hold last byte received
    shiftOut(SER, SRCLK, MSBFIRST, digit[displayed]); //start transmitting bits corresponding to Pins A-G and DP
    digitalWrite(RCLK, HIGH); //enable parallel output from register of new byte
    delay(10);               //one second delay to observe output
}

else { //if it's neither
    int displayed = 0;
    digitalWrite(RCLK, LOW);  //hold last byte received
    shiftOut(SER, SRCLK, MSBFIRST, digit[displayed]); //start transmitting bits corresponding to Pins A-G and DP
    digitalWrite(RCLK, HIGH); //enable parallel output from register of new byte
    delay(10);               //one second delay to observe output
}
}
