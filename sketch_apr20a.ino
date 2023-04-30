#include "DHT.h"
#define DHT11PIN 16
#define RELAY_PIN 12
float humis[10];// = new float[10];
float temps[10];// = new float[10];
float increaseWater;
int ind;
DHT dht(DHT11PIN, DHT11);


void setup()
{
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  ind= -1;
  increaseWater = 0;
/* Start the DHT11 Sensor */
  dht.begin();
}

void loop()
{
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();

  ind ++;
  if(ind > sizeof(humis)){
    ind =0 ;
  }
  humis[ind] = humi;
  temps[ind] = temp;

  float avgHumi = 0;
  for(int i = 0; i < sizeof(humis); i++)
    avgHumi += humis[i];
  avgHumi /= sizeof(humis) + increaseWater;

  float avgTemp = 0;
  for(int i = 0; i < sizeof(temps); i++)
    avgTemp += temps[i];
  avgTemp /= sizeof(temps);

  
  if(avgHumi < 60 && avgTemp < 30){
    digitalWrite(RELAY_PIN, HIGH);
    increaseWater++;
  }

  if(avgHumi > 80 || avgTemp >= 30){
    digitalWrite(RELAY_PIN, LOW);
    increaseWater = 0;
  }


  Serial.print("Temperature: ");
  Serial.print(avgTemp);
  Serial.print("ºC ");
  Serial.print("Humidity: ");
  Serial.println(avgHumi);
  delay(1000);
}