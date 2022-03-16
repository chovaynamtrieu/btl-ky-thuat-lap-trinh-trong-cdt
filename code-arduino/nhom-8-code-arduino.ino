#include <Servo.h>
Servo myservo_1;
Servo myservo_2;

#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8

String x="B";
int G, R, B;//biến lưu giá trị R, G, B của quả bóng
int numB=0, numW=0, numR=0;// biến lưu số quả bóng mỗi màu

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

  // Setting frequency-scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  

  myservo_1.attach(9);
  myservo_2.attach(10);
  Serial.begin(9600);
}

void loop() {
if(Serial.available()){
  x=Serial.readString();
  if(x=="B"){
    delay(1000);
    myservo_1.write(143);
    delay(2000);
    myservo_1.write(100);
    delay(100);
    
      // Setting red filtered photodiodes to be read
      digitalWrite(S2,LOW);
      digitalWrite(S3,LOW);
      // Reading the output frequency
      R = pulseIn(sensorOut, LOW);
      delay(200);
    
      // Setting Green filtered photodiodes to be read
      digitalWrite(S2,HIGH);
      digitalWrite(S3,HIGH);
      // Reading the output frequency
      G = pulseIn(sensorOut, LOW);
      delay(200);
    
      // Setting Blue filtered photodiodes to be read
      digitalWrite(S2,LOW);
      digitalWrite(S3,HIGH);
      // Reading the output frequency
      B = pulseIn(sensorOut, LOW);
      delay(200);

    //chuyen servo 2 to pos new
    if(G>60 &R>50&R<60){
      myservo_2.write(50);
      numR=numR+1;
      delay(200);//red
    }
    if(G<60&&R<50){
      myservo_2.write(90);
      numW=numW+1;
      delay(200);//white
    }
    if(R>60&&G>65){
      myservo_2.write(125);
      numB=numB+1;
      delay(200);//black
    }
    Serial.println("R"+String(numR)+"W"+ String(numW)+"B"+ String(numB));
    //chuyen servo 2 to 50 do va tha bong
    myservo_1.write(50);
    delay(200);
} else if (x=="D") {delay(1000);}
  else if (x=="L") {
    numR=0; numW=0; numB=0;
    Serial.println("R"+String(numR)+"W"+ String(numW)+"B"+ String(numB));}
}    
}
