#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <dht.h>

#define Dht11_pin 4 //change to: const int Dht11_pin = 4;
#define RE 8 //same
#define DE 7 // same

const byte ph[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};
const byte nitro[] = {0x01,0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const byte phos[] = {0x01,0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte pota[] = {0x01,0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};

 

int moisture_pin = A0;
float soil_moisture;
float air_temperature;
float air_humidity;
float soil_ph;
byte nitrogen_value, phosphorous_value, potassium_value;
 

byte values[11];

SoftwareSerial mod(2, 3);
SoftwareSerial arduino_to_nodemcu(5,6);

dht DHT;



 
void setup() {
  arduino_to_nodemcu.begin(9600);
  Serial.begin(9600);
  mod.begin(9600);
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);

  delay(500);
}
 
void loop() {
 StaticJsonBuffer<1000> jsonBuffer;
 JsonObject& sensors_data = jsonBuffer.createObject();

   // soil moisture value
  //soil_moisture = analogRead(moisture_pin);
  soil_moisture = 133;
  // air temperature and humidity
  DHT.read11(Dht11_pin);
 
  //air_temperature = DHT.temperature;
  air_temperature = 37;
  
  air_humidity = DHT.humidity;
  //air_humidity = 40;

  // ph soil value
  soil_ph = get_ph();
  
  // NPK values
  
  nitrogen_value = get_nitrogen();
  delay(250);
  phosphorous_value = get_phosphorous();
  delay(250);
  potassium_value = get_potassium();
  delay(250);
  

  sensors_data["soil_moisture"] = soil_moisture;
  sensors_data["air_temperature"] = air_temperature;
  sensors_data["air_humidity"] = air_humidity;
  sensors_data["soil_ph"] = soil_ph;
  sensors_data["nitrogen_value"] = nitrogen_value;
  sensors_data["phosphorous_value"] = phosphorous_value;
  sensors_data["potassium_value"] = potassium_value;
  Serial.print("soil_moisture : ");
  Serial.println(soil_moisture);

 // send data to nodemcu

  sensors_data.printTo(arduino_to_nodemcu);


  delay(10000);
}







float get_ph(){
  
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  if (mod.write(ph, sizeof(ph)) == 8)
  {
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for (byte i = 0; i < 11; i++)
    {
      values[i] = mod.read();
      Serial.print(values[i], HEX); 
    }
    Serial.println();
  }

  return  float(values[4]) / 10;

}

byte get_nitrogen(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(mod.write(nitro,sizeof(nitro))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    for(byte i=0;i<7;i++){
    values[i] = mod.read(); // lire toute la trame.
    }

  }
  return values[4];
}

byte get_phosphorous(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(mod.write(phos,sizeof(phos))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    for(byte i=0;i<7;i++){
    values[i] = mod.read();
    }
  }
  return values[4];
}


byte get_potassium(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(mod.write(pota,sizeof(pota))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    for(byte i=0;i<7;i++){
    values[i] = mod.read();
    }
  }
  return values[4];
}
