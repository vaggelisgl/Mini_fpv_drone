#include <SPI.h>
#include <RF24.h>
#include <Adafruit_MPU6050.h>
#include <Wire.h>
#define FL 6
#define FR 9
#define BL 5
#define BR 3
#define SDA A4
#define SCL A5
#define CSN 10
#define MOSI 11
#define MISO 12
#define SCK 13
#define CE 7
Adafruit_MPU6050 mpu;
RF24 radio(CE,CSN);
const byte address[2][6]={"00001","00010"};
bool mode=0; //0-recieve 1-transmit 
#define size 32
uint8_t buffer_tx[size+1];
uint8_t buffer_rx[size+1];
uint8_t counter=0;

union {
  float f;
  uint8_t byte[4];
} u;
void makePayload(uint8_t i);

void setup() {
  buffer_rx[size]=buffer_tx[size]='\n';
  Serial.begin(115200);
    if (!mpu.begin()) {
    for(int i=0;i<size;i++) buffer_tx[i]='\n';
    }
  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  if(!radio.begin()){Serial.println("radio not responding");}
  else{Serial.println("all is good");}
  radio.setPALevel(RF24_PA_LOW);
  radio.setPayloadSize(size);
  radio.openWritingPipe(address[0]);
  radio.stopListening();
  radio.setAutoAck(true);
  radio.enableAckPayload();

  pinMode(FL,OUTPUT);
  digitalWrite(FL,0);
  pinMode(FR,OUTPUT);
  digitalWrite(FR,0);
  pinMode(BL,OUTPUT);
  digitalWrite(BL,0);
  pinMode(BR,OUTPUT);
  digitalWrite(BR,0);
  for(int i=0;i<size;i++){buffer_tx[i]=0;}
}

int val[5]={0,0,0,0,0};
bool motoron=0;
unsigned long int timer=millis();
void loop() {
  //transmit
  radio.flush_tx();
  makePayload(counter);
  if(radio.write(&buffer_tx,sizeof(buffer_tx))){
    if(radio.isAckPayloadAvailable()){
      radio.read(buffer_rx,sizeof(buffer_rx));
      decode();
    }
  };

 
  
  
  //output
  if(millis()-timer>val[4]) {motoron=!motoron; timer=millis();}
  if(motoron){
  analogWrite(FL,val[0]);
  analogWrite(FR,val[1]);
  analogWrite(BR,val[2]);
  analogWrite(BL,val[3]);}     //temp
  else{
  analogWrite(FL,0);
  analogWrite(FR,0);
  analogWrite(BR,0);
  analogWrite(BL,0);}
  

}

void makePayload(uint8_t i){
  sensors_event_t a, g, temp;
  int k=1;
  mpu.getEvent(&a, &g, &temp);
  buffer_tx[0]=i + (i<26? 65 : 71);
  u.f=a.acceleration.x;
  buffer_tx[1]=u.byte[0];buffer_tx[2]=u.byte[1];buffer_tx[3]=u.byte[2];buffer_tx[4]=u.byte[3];
  u.f=a.acceleration.y;
  buffer_tx[5]=u.byte[0];buffer_tx[6]=u.byte[1];buffer_tx[7]=u.byte[2];buffer_tx[8]=u.byte[3];
  u.f=a.acceleration.z;
  buffer_tx[9]=u.byte[0];buffer_tx[10]=u.byte[1];buffer_tx[11]=u.byte[2];buffer_tx[12]=u.byte[3];
  u.f=g.gyro.x;
  buffer_tx[13]=u.byte[0];buffer_tx[14]=u.byte[1];buffer_tx[15]=u.byte[2];buffer_tx[16]=u.byte[3];
  u.f=g.gyro.y;
  buffer_tx[17]=u.byte[0];buffer_tx[18]=u.byte[1];buffer_tx[19]=u.byte[2];buffer_tx[20]=u.byte[3];
  u.f=g.gyro.z;
  buffer_tx[21]=u.byte[0];buffer_tx[22]=u.byte[1];buffer_tx[23]=u.byte[2];buffer_tx[24]=u.byte[3];
  for(int j=25;j<32;j++){buffer_tx[j]=0;}
  i++;
}
void decode(){
  val[0]=int(buffer_rx[1])*255+int(buffer_rx[2]);
  val[1]=int(buffer_rx[3])*255+int(buffer_rx[4]);
  val[2]=int(buffer_rx[5])*255+int(buffer_rx[6]);
  val[3]=int(buffer_rx[7])*255+int(buffer_rx[8]);
  val[4]=int(buffer_rx[9])*255+int(buffer_rx[10]);
}
