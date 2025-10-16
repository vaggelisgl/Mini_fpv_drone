#include <SPI.h>
#include <RF24.h>
#define CSN 10
#define MOSI 11
#define MISO 12
#define SCK 13
#define CE 7
RF24 radio(CE,CSN);
const byte address[2][6]={"00001","00010"};
bool mode=0; //0-recieve 1-transmit 
#define size 32
uint8_t buffer_tx[size+1];
uint8_t buffer_rx[size+1];
uint8_t counter=0;
void makePayload(uint8_t i);
float arr[6]={0,0,0,0,0,0};  
float val[5]={0,0,0,0,0};
float ax=arr[0],ay=arr[1],az=arr[2],gx=arr[3],gy=arr[4],gz=arr[5];

union{
  float f;
  uint8_t byte[4];
}u;

void setup() {
  buffer_tx[size]=buffer_rx[size]='\n';
  Serial.begin(115200);
  if(!radio.begin()){Serial.println("radio not responding");}
  else{Serial.println("all is good");}
  radio.setPALevel(RF24_PA_LOW);
  radio.setPayloadSize(size);
  radio.openReadingPipe(0,address[0]);
  radio.startListening();
  radio.setAutoAck(true);
  radio.enableAckPayload();
  for(int i=0;i<size;i++){buffer_tx[i]=0;}
  buffer_tx[size]='\n';
  
  
  
}

void loop() {
  unsigned long int timer=millis();
  if(Serial.available()){ 
    makePayload(counter);int i=0;
  while(buffer_tx[i]!='\n')Serial.println(int(buffer_tx[i++])); //remove later
  }

  if(radio.available()){
  radio.read(&buffer_rx,size);
  
  decode();
  delay(10);
  } radio.writeAckPayload(0, &buffer_tx, sizeof(buffer_tx)); //variables on ax,ay,az,gx,y,gz (raw)
  
  // Serial.print("ax:");
  // Serial.print(ax);
  // Serial.print(',');
  // Serial.print("ay:");
  // Serial.print(ay);
  // Serial.print(',');
  // Serial.print("az:");
  // Serial.print(az);
  // Serial.print(',');
  Serial.print("gx:");  
  Serial.print(gx);
  Serial.print(',');
  Serial.print("gy:");
  Serial.print(gy);
  Serial.print(',');    //temp ,should have filtered data through comp filter for pitch,roll,yaw, (file->accl.ino).
  Serial.print("gz:");
  Serial.print(gz);
  Serial.print(',');
  Serial.print("UL:");
  Serial.print(90);
  Serial.print(',');
  Serial.print("LL:");
  Serial.println(-90);
  

 

}
void decode(){
  int k=1;          
   while(k<21){
    for(int i=k;i<k+4;i++){u.byte[i-k]=buffer_rx[i];}
   arr[(k-1)/4]=u.f; k+=4; 
   }
   ax=arr[0],ay=arr[1],az=arr[2],gx=arr[3],gy=arr[4],gz=arr[5];

}

void makePayload(uint8_t i){
  buffer_tx[0]=i + (i<26? 65 : 71);
  int j=1;
  Serial.println("INPUT with spaces FR=...,FL=...,BR=...,BL=..., DELAY..."); //temp
  
  while(Serial.available()){
   int value=Serial.parseInt();
   buffer_tx[j++]=value/256;  
   buffer_tx[j++]=value%256;}
   for(int k=j;k<size;k++) buffer_tx[k]=0;

  buffer_tx[size]='\n';
  i++;
  while(Serial.available())Serial.read();
}
