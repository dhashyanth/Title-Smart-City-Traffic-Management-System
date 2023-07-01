#include<TimerOne.h>
#include <Servo.h>
#include <SPI.h>
#include <mfrc522.h>
#define SS_PIN 53
#define RST_PIN 5
byte readCard[4];
String tag[] = {"F360561A","03B063A2","42C1F339","3450CE30"};  // Replace this with the UID of your tag!!!

String tagID = "";
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

volatile int e = 0;

// traffic lights - R,Y,G
int traffic1[] = {22,23,24};
int traffic2[] = {30,31,32};
int traffic3[] = {38,39,40};
int traffic4[] = {46,47,48};

volatile int trig[] = {12,11,9,7};
volatile int echo[] = {13,10,8,6};

volatile float distance[4] = {0,0,0,0};
// volatile int pos[3] = {-45,0,45};
volatile long duration;

int distance_required = 20; 

float threshold = 20;

// int e[] = {0,0,0,0};
int yellowdelay = 2000;
int greendelay = 6000;
int reddelay = 3000;
int temp = 0;

void setup() {

  // put your setup code here, to run once:
  Serial.begin(115200);
  Timer1.initialize(1000000000);
  // Timer1.attachInterrupt(detection);
  for(int i=0;i<=2;i++){
    pinMode(traffic1[i],OUTPUT);
    pinMode(traffic2[i],OUTPUT);
    pinMode(traffic3[i],OUTPUT);
    pinMode(traffic4[i],OUTPUT);
  }

  for(int i=0;i<=3;i++){
    pinMode(trig[i],OUTPUT);
    pinMode(echo[i],INPUT);
  }
  
  SPI.begin(); // SPI bus
  mfrc522.PCD_Init(); // Initialise MFRC522
  // Serial.println("Please scan your RFID TAG");
  

}


void loop() {
  // traffic light management in lane
  // Serial.println(threshold);
  detection();
  if(threshold>=distance[0] || tagID==tag[0]){
    Serial.println("LANE 1 is ON!!");
    trafficlane1();
  } 
  else if(threshold>=distance[1] || tagID==tag[1]){
    Serial.println("LANE 2 is ON!!");
    trafficlane2();
  }
  
  else if(threshold>=distance[2] || tagID==tag[2]){
    Serial.println("LANE 3 is ON!!");
    trafficlane3();
  }
  
  else if(threshold>=distance[3] || tagID==tag[3]){
    Serial.println("LANE 4 is ON!!");
    trafficlane4();
    
  }
  else{
    if (temp == 0){
        Serial.println("LANE 1 round is ON!!");
          trafficlane1();
          temp = temp + 1;
      }
    else if(temp == 1){
        Serial.println("LANE 2 round is ON!!");
          trafficlane2();
          temp = temp + 1;
      }
    else if (temp == 2){
      Serial.println("LANE 3 round is ON!!");
          trafficlane3();
          temp = temp + 1;
      }
    else if (temp>2){
      Serial.println("LANE 4 round is ON!!");
          trafficlane4();
    temp = 0;
      }
 }
 
 tagID = "" ;
}

void detection(){
  //lane 1,2,3,4 ultrasonic detection of vehicle
  
  for(int i=0 ; i<4;i++){
    distance[i] = 0;
    digitalWrite(trig[i],LOW);
    delayMicroseconds(2);
    digitalWrite(trig[i],HIGH);
    delayMicroseconds(10);
    digitalWrite(trig[i],LOW);
    duration = pulseIn(echo[i],HIGH);
    distance[i] += (duration * 0.0343)/2;
    
    Serial.print("S");
    Serial.print(i+1);
    Serial.print(": ");
    Serial.print(distance[i]);
    Serial.print("cm   ");
    Serial.flush();
    Serial1.print("\n");
    
    } 
  Serial.println("");
  readID(); 
}

void trafficlane1(){
  //lane1
  reset_lanes();
  digitalWrite(traffic1[0],LOW);
  digitalWrite(traffic1[2],HIGH);
  delay(greendelay);
  digitalWrite(traffic1[2],LOW);
  digitalWrite(traffic1[1],HIGH);
  delay(yellowdelay);
  digitalWrite(traffic1[0],HIGH);

}
void trafficlane2(){
  //lane2
  reset_lanes();
  digitalWrite(traffic2[0],LOW);
  digitalWrite(traffic2[2],HIGH);
  delay(greendelay);
  digitalWrite(traffic2[2],LOW);
  digitalWrite(traffic2[1],HIGH);
  delay(yellowdelay);
  digitalWrite(traffic2[0],HIGH);

}
void trafficlane3(){
  //lane3
  reset_lanes();
  digitalWrite(traffic3[0],LOW);
  digitalWrite(traffic3[2],HIGH);
  delay(greendelay);
  digitalWrite(traffic3[2],LOW);
  digitalWrite(traffic3[1],HIGH);
  delay(yellowdelay);
  digitalWrite(traffic3[0],HIGH);

}
void trafficlane4(){
  //lane4
  reset_lanes();
  digitalWrite(traffic4[0],LOW);
  digitalWrite(traffic4[2],HIGH);
  delay(greendelay);
  digitalWrite(traffic4[2],LOW);
  digitalWrite(traffic4[1],HIGH);
  delay(yellowdelay);
  digitalWrite(traffic4[0],HIGH);

}
bool ev(int e){
  while(readID()){
    for(int i;i<4;i++){
      if(i!=e){
        if(tagID==tag[i]){
          Serial.println(tagID);
          return true;
        }
      }
    }
    return false;

  }
}

bool vehicles_other_lanes(int i){
  bool flag = false;
  for(int y=0; y<=3; y++){
    if(i!=y){
      flag|=distance[y]<threshold;
    }
  }
  return flag;
}
bool readID(){
    //Check if a new tag is detected or not. If not return.
    if ( ! mfrc522.PICC_IsNewCardPresent())
    {
      Serial.println("No id");
      return false;
    }
    //Check if a new tag is readable or not. If not return.
    if ( ! mfrc522.PICC_ReadCardSerial())
    {
      Serial.println("Wrong id");
      return false;
    }
    Serial.println("yes");
    if(tagID==tag[0]){
    Serial.println("LANE 1 is ON!!");
    trafficlane1();
  } 
  else if(tagID==tag[1]){
    Serial.println("LANE 2 is ON!!");
    trafficlane2();
  }
  
  else if(tagID==tag[2]){
    Serial.println("LANE 3 is ON!!");
    trafficlane3();
  }
  
  else if(threshold>=distance[3] || tagID==tag[3]){
    Serial.println("LANE 4 is ON!!");
    trafficlane4();
    
  }
    tagID = "";
    // Read the 4 byte UID
    for ( uint8_t i = 0; i < 4; i++)
    {
      //readCard[i] = mfrc522.uid.uidByte[i];
      tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Convert the UID to a single String
    }
    tagID.toUpperCase();
    mfrc522.PICC_HaltA(); // Stop reading
    return true;
}

void reset_lanes(){
  for(int i=1;i<3;i++){
    digitalWrite(traffic1[i],LOW);  
    digitalWrite(traffic2[i],LOW);   
    digitalWrite(traffic3[i],LOW);
    digitalWrite(traffic4[i],LOW);
  }
  
  digitalWrite(traffic1[0],HIGH); 
  digitalWrite(traffic2[0],HIGH);   
  digitalWrite(traffic3[0],HIGH); 
  digitalWrite(traffic4[0],HIGH);
  
}