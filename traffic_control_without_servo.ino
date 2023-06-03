#include<TimerOne.h>

// traffic lights - R,Y,G
int traffic1[] = {22,23,24};
int traffic2[] = {30,31,32};
int traffic3[] = {38,39,40};
int traffic4[] = {46,47,48};

volatile int trig[] = {12,11,9,7};
volatile int echo[] = {13,10,8,6};

volatile int distance[4] = {0,0,0,0};
volatile long duration;

int distance_required = 20;

int yellowdelay = 1500;
int greendelay = 1000;
int reddelay = 1000;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Timer1.initialize(100000000);
  Timer1.attachInterrupt(detection);
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

}


void loop() {
  //roundrobin();
  // traffic light management in lane
  if(distance_required>=distance[0]){
    
    trafficlane1();
  }
  
  if(distance_required>=distance[1]){
    trafficlane2();
  }
  
  if(distance_required>=distance[2]){
    trafficlane3();
  }
  
  if(distance_required>=distance[3]){
    trafficlane4();
  }

}



void detection(){
  //lane 1,2,3,4 ultrasonic detection of vehicles

  for(int i=0 ; i<4;i++){
    // int x=3;
    // while(x>0){
    //   x=x-1;
    digitalWrite(trig[i],LOW);
    delayMicroseconds(2);
    digitalWrite(trig[i],HIGH);
    delayMicroseconds(10);
    digitalWrite(trig[i],LOW);
    duration = pulseIn(echo[i],HIGH);
      
    // }
    distance[i] = (duration * 0.0343)/2;
    
    Serial.print("S");
    Serial.print(i+1);
    Serial.print(": ");
    Serial.print(distance[i]);
    Serial.print("cm   ");
    Serial.flush();
    // Serial1.print(distance[i]);
    Serial1.print("\n");
  } 
  Serial.println("");
}
void trafficlane1(){
  //lane1
  clear();
  Serial.println("LANE 1 is ON!!");
  digitalWrite(traffic1[0],LOW);
  digitalWrite(traffic1[2],HIGH);
  delay(greendelay);
  if(vehicles_other_lanes(0)){
    digitalWrite(traffic1[2],LOW);
    digitalWrite(traffic1[1],HIGH);
    delay(yellowdelay);
  }
  
}
void trafficlane2(){
  //lane2
  clear();
  Serial.println("LANE 2 is ON!!");
  digitalWrite(traffic2[0],LOW);
  digitalWrite(traffic2[2],HIGH);
  delay(greendelay);
  if(vehicles_other_lanes(1)){
    digitalWrite(traffic2[2],LOW);
    digitalWrite(traffic2[1],HIGH);
    delay(yellowdelay);
  }
  
}
void trafficlane3(){
  //lane3
  clear();
  Serial.println("LANE 3 is ON!!");
  digitalWrite(traffic3[0],LOW);
  digitalWrite(traffic3[2],HIGH);
  delay(greendelay);
  if(vehicles_other_lanes(2)){
    digitalWrite(traffic3[2],LOW);
    digitalWrite(traffic3[1],HIGH);
    delay(yellowdelay);
  }
  
}
void trafficlane4(){
  //lane4
  clear();
  Serial.println("LANE 4 is ON!!");
  digitalWrite(traffic4[0],LOW);
  digitalWrite(traffic4[2],HIGH);
  delay(greendelay);
  if(vehicles_other_lanes(3)){
    digitalWrite(traffic4[2],LOW);
    digitalWrite(traffic4[1],HIGH);
    delay(yellowdelay);
  }

}

bool vehicles_other_lanes(int i){
  bool flag = false;
  for(int y=0; y<=3; y++){
    if(i!=y){
      flag|=distance[y]<distance_required;
    }
  }
  return flag;
}

void clear(){
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