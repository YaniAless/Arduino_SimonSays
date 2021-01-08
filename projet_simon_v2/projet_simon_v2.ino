#include <SoftwareSerial.h>


#define RED_LED 3
#define BLUE_LED 4
#define GREEN_LED 5
#define YELLOW_LED 6
#define BTN_ONOFF = 8;
#define RING_PIN = 7;

// Bluetooth
SoftwareSerial BTModule(10, 11); // RX, TX
String msg = "";
  
// Normal music notes (Hz)
const int DO = 1047;
const int RE = 1174;
const int MI = 1319;
const int FA = 1397;
const int SOL = 1568;
const int LA = 1760;
const int SI = 1976;

// Low music notes (Hz)
const int DOLOW = 261;
const int RELOW = 293;
const int MILOW = 329;
const int FALOW = 349;
const int SOLLOW = 392;
const int LALOW = 440;
const int SILOW = 493;

int redLedButtonState = 1;
int blueLedButtonState = 1;
int greenLedButtonState = 1;
int yellowLedButtonState = 1;

int winStreak = 0;
int colorTab[50] = {};
int numberOfValues = 0;

bool start = false;

void setup()
{
  Serial.begin(9600);  
  
  // LED  
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  
  pinMode(BTN_ONOFF, INPUT_PULLUP); // Turn ON/OFF Btn
  
  // Ring
  pinMode(RING_PIN, OUTPUT); 
  
  randomSeed(analogRead(0));
  
  fillLedOrderTab();

}


void fillLedOrderTab(){
  winStreak = 0;
  for(int i = 0; i < 4; i++){
    int rand = random(1,5); // 1 to 4
    colorTab[i] = rand;
  }
  numberOfValues = 4;
}


void loop()
{ 
  
  if(digitalRead(BTN_ONOFF) == 0){
    start = !start;
    Serial.println(start);
    if(start) {
      startSound();
      delay(500);
    } else {
      stopSound();
    }
  }
  
  if(start) {
    showMeThePath();
  
    int userAnswerTab[50] = {};

    for(int i = 0; i < numberOfValues; i++) {
      int response = getButtonPinPushed();
      if(response != colorTab[i]) {
        failureeee();
        fillLedOrderTab();
        break;
      }
    }
    if(start){
      success();
    }    
  } else {
      tone(ringPin, DOLOW, 400);
      delay(500);
      tone(ringPin, RE, 400);
      delay(500);
      tone(ringPin, MILOW, 400);
      delay(500);
      tone(ringPin, FA, 400);
      delay(500);
      tone(ringPin, SOLLOW, 400);
      delay(500);
      tone(ringPin, LA, 400);
      delay(500);
      tone(ringPin, SILOW, 400);
      delay(500);
      tone(ringPin, DO, 400);
      delay(500);
    }
}

void success() {
  winStreak++;
  colorTab[numberOfValues] = random(1,5);
  numberOfValues++;
  winSound();
}

void failureeee() {
  Serial.println("You have failed.");
  Serial.print("You have made a win streak of ");
  Serial.println(winStreak);
  lossSound();
  start = false;
  delay(1000);
}

void showMeThePath() {  
  for(int i = 0; i < numberOfValues; i++) {
    lightOn(colorTab[i]);
    delay(250);
  }
}

void lightOn(int value) {
  if(value == 1) {
    digitalWrite(BLUE_LED, HIGH);
    tone(RING_PIN,DO,300);
  	delay(300);
    digitalWrite(BLUE_LED, LOW);
  } else if(value == 2) {
    digitalWrite(RED_LED, HIGH);
    tone(RING_PIN,RE,300);
  	delay(300);
    digitalWrite(RED_LED, LOW);
  } else if(value == 3) {
    digitalWrite(GREEN_LED, HIGH);
    tone(RING_PIN,MI,300);
  	delay(300);
    digitalWrite(GREEN_LED, LOW);
  } else {
    digitalWrite(YELLOW_LED, HIGH);
    tone(RING_PIN,SOL,300);
  	delay(300);
    digitalWrite(YELLOW_LED, LOW);
  }
}

int getLedPinByValue(int value) {
  if(value == 1) {
    return RED_LED;
  } else if(value == 2) {
    return BLUE_LED;
  } else if(value == 3) {
    return GREEN_LED;
  } else {
   	return YELLOW_LED; 
  }
}

void readBluetoothInputs() {
  while(BTModule.available()) {
    msg = BTModule.readString();
    if(msg == "r"){
      msg = "";
      redLedButtonState = 0
    } else if(msg == "b"){
      msg = "";
      blueLedButtonState = 0;
    } else if(msg == "g"){
      msg = "";
      greenLedButtonState = 0;   
    } else if(msg == "y"){
      msg = "";
      yellowLedButtonState = 0;     
    }    
  }  
}

int getButtonPinPushed() {

  // Cette partie ne passe pas trop bien meme en mettant 0 au lieu de 1
  while( redLedButtonState == 1 && blueLedButtonState == 1 && 
        greenLedButtonState == 1 && yellowLedButtonState == 1) {
    readBluetoothInputs();
  }  
  
  if(redLedButtonState == 0) {
    lightOn(1);
    delay(250);
    return 1;
  } else if(blueLedButtonState == 0) {
    lightOn(2);
    delay(250);
    return 2;
  } else if(greenLedButtonState == 0) {
    lightOn(3);
    delay(250);
    return 3;
  } else if(yellowLedButtonState == 0){
    lightOn(4);
    delay(250);
   	return 4; 
  }
  
} 

void startSound(){
  tone(ringPin,SOL,700);
  delay(500);
  tone(ringPin,LA,700);
  delay(500);
  tone(ringPin,SI,300);
  delay(1000);
}

void stopSound(){
  tone(ringPin,MILOW,700);
  delay(500);
  tone(ringPin,RELOW,700);
  delay(500);
  tone(ringPin,DOLOW,700);
  delay(1000);
}

void lossSound(){
  tone(ringPin,MILOW,700);
  delay(500);
  tone(ringPin,RELOW,700);
  delay(500);
  tone(ringPin,DOLOW,700);
  delay(500);
}

void winSound(){
  tone(ringPin,SOL,500);
  delay(500);
  tone(ringPin,SI,700);
  delay(1000);
}
