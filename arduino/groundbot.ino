
#include <Arduino.h>

#define PinEastA 6
#define PinEastB 7
#define PinEast_en 5
#define PinWestA 8
#define PinWestB 9
#define PinWest_en 10

String message = "";
bool stringComplete = false;
const int COMMAND_LENGTH = 10;

int speedEast;
int speedWest;

double distance_reading;

void split(String messages, String *splitMessages, char delimiter=',');
void actuatorLoop();
void sensorLoop();

void setup() {
  Serial.begin(9600);

  while(!Serial){
   // // Waiting for a connection to be generated, needed for delays caused by USB
  }

  
  pinMode(PinEastA,OUTPUT);
  pinMode(PinEastB,OUTPUT);
  pinMode(PinEast_en,OUTPUT);
  pinMode(PinWestA,OUTPUT);
  pinMode(PinWestB,OUTPUT);
  pinMode(PinWest_en,OUTPUT);

}

void loop() {
  digitalWrite(PinEastB,HIGH);
  digitalWrite(PinEastA,LOW);
  digitalWrite(PinWestA,HIGH);
  digitalWrite(PinWestB,LOW);

  actuatorLoop();
}  

void actuatorLoop(){
  // If we have recieved a full message, actuate based upon it
  if(stringComplete){
    String splitMessages[COMMAND_LENGTH];
    for(int i = 0; i < COMMAND_LENGTH; i++){
      splitMessages[i] = "";
    }
    split(message, splitMessages,',');

    // We've already used this message
    message = "";
    stringComplete = false;

     speedEast = splitMessages[0].toInt();
     speedWest = splitMessages[1].toInt(); 

     if ((speedEast >=0 && speedEast <=255) && (speedWest >=0 && speedWest <=255)){
      analogWrite(PinEast_en,speedEast);
      analogWrite(PinWest_en,speedWest);
     }
     else Serial.print("ERROR: Enter a speed between 0 and 255");
  }
    
}    

void split(String messages, String* splitMessages,  char delimiter){
  int indexOfComma = 0;
  for(int i = 0; messages.indexOf(delimiter, indexOfComma) > 0; i++){
    int nextIndex = messages.indexOf(delimiter, indexOfComma+1);
    String nextMessage;

    // The first message doesn't have an initial comma, so account for that.
    if(indexOfComma == 0){
      indexOfComma = -1;
    }
    if(nextIndex == -1){
      nextMessage = messages.substring(indexOfComma+1);
    }else{
      nextMessage = messages.substring(indexOfComma+1, nextIndex);
    }
    splitMessages[i] = nextMessage;
    indexOfComma = nextIndex;
  }
}

//this function takes any input serial to a string buffer
//runs without any delay() calls, and thus tries to achieve maximum loop_rate as possible
void serialEvent(){
  
  if(stringComplete){
    message="";
    stringComplete = false;
    //this is for complete reading of a string, we clear message and counter
  }

  while (Serial.available()){
    //get the new byte from serial
    char inputChar = (char) Serial.read();
    message += inputChar;
    //if the incoming character is a newline, set a flag
    if (inputChar == '\n')
    {
      stringComplete = true;
      return;
    }
    //no return statement outside since we canot miss anything
  }
}
