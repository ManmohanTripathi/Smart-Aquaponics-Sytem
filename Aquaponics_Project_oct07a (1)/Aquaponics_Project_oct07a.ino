#include "arduino_secrets.h"
/* 
  The following variables are automatically generated and updated when changes are made to the Thing

  CloudTemperatureSensor temperature;
  CloudPercentage water_level;
  bool feeder;
  bool pump;

*/

#include "thingProperties.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#include <ESP32Servo.h>

#define ONE_WIRE_BUS 2
float calibration_value = 21.34 - 1.5;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;
 
float ph_act;

int emptyTankDistance = 15 ;  //Distance when tank is empty
int fullTankDistance =  2 ;  //Distance when tank is full
#define TRIGPIN    27  //D27
#define ECHOPIN    26  //D26
int servoPin = 13;
const int LED = 5;
Servo myservo; 
int relay1 =4;
int relay2 =15;
int pos = 0;
float duration;
float distance;
int   waterLevelPer;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  Serial.println("Dallas Temperature IC Control Library Demo");
  sensors.begin();
   pinMode(ECHOPIN, INPUT);
  pinMode(TRIGPIN, OUTPUT);
  myservo.attach(servoPin, 1000, 2000);
  pinMode(relay1,OUTPUT);
  pinMode(relay2,OUTPUT);
delay(1000);
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  onFeederChange();
  onPumpChange();
  
 for(int i=0;i<10;i++) 
 { 
 buffer_arr[i]=analogRead(35);
 delay(30);
 }
 for(int i=0;i<9;i++)
 {
 for(int j=i+1;j<10;j++)
 {
 if(buffer_arr[i]>buffer_arr[j])
 {
 temp=buffer_arr[i];
 buffer_arr[i]=buffer_arr[j];
 buffer_arr[j]=temp;
 }
 }
 }
 avgval=0;
 for(int i=2;i<8;i++)
 avgval+=buffer_arr[i];
 float volt=(float)avgval*3.3/4096.0/6; 
  ph = -5.70 * volt + calibration_value;
 
  ArduinoCloud.update();
  measureDistance();
  Serial.println("Requesting temperatures...");
  sensors.requestTemperatures(); 
  Serial.println("DONE");
  float tempf = 0;
  int c = 0;
  for(int i = 0;i<10;i++){   
  float tempC = sensors.getTempCByIndex(0);
  if (tempC != DEVICE_DISCONNECTED_C)
  {
    tempf = tempC+tempf;
    c++; 
  }
  else
  {
    Serial.println("Error: Could not read temperature data");
    continue;
  }
  }
  temperature = tempf/c;
  if(temperature<25){
    digitalWrite(LED, HIGH);
  delay(1000);
  }
  else{
    digitalWrite(LED, LOW);
  delay(1000);

  }
  Serial.print("Temperature for the device  is: ");
  Serial.println(temperature);
  Serial.println("pH Val: ");
 Serial.print(ph);
 delay(1000);
}
void onFeederChange()  {
  if( feeder == 1){
    for (pos = 0; pos <= 180; pos += 1) { 
		myservo.write(pos);    
		delay(15);             
	}
    for (pos = 180; pos >= 0; pos -= 1) { 
		myservo.write(pos);    
		delay(15);             
	}
  }
  else if(feeder == 0){
    myservo.write(180);
    delay(15);
  }
  
}

void measureDistance(){
  
  digitalWrite(TRIGPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(20);
  digitalWrite(TRIGPIN, LOW);
  duration = pulseIn(ECHOPIN, HIGH);
  distance = ((duration / 2) * 0.343)/10;
   water_level = map((int)distance ,emptyTankDistance, fullTankDistance, 0, 100);
  Serial.print("Distance : ");
  Serial.println(distance);
Serial.print("Water Level : ");
  Serial.println( water_level);
   delay(1000);
}
void onPumpChange()  {
  // Add your code here to act upon Pump change
 if((water_level<100 || water_level==100)  &&(water_level>0 || water_level==0  ) ){
    if(pump == 3){
    digitalWrite(relay1,HIGH);
    digitalWrite(relay2,HIGH);
    delay(5000);
  }
   else {
     if((water_level<90 || pump ==1)){
    digitalWrite(relay1,LOW);
    delay(5000);
  }
  if((water_level>90 || pump == 2)){
    digitalWrite(relay2,LOW);
    delay(5000);
  }
   }
 }
}