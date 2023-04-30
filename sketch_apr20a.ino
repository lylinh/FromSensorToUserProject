#include "DHT.h"
#define DHT11PIN 16
#define RELAY_PIN 12

// cache data in 10s
float humis[10];
float temps[10];


float increaseWater; // this variable fake huminity increase over time
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
  float humi = dht.readHumidity();    // collect data of humidity from sensor
  float temp = dht.readTemperature(); // collect data of temperature from sensor

  // remove the oldest value in cache
  ind ++;
  if(ind > sizeof(humis)){
    ind = 0;
  }
  
  
  humis[ind] = humi;
  temps[ind] = temp;

  
  // average value of humidity in 10 seconds. Which is make decision
  float avgHumi = 0;
  for(int i = 0; i < sizeof(humis); i++)
    avgHumi += humis[i];
  avgHumi /= sizeof(humis) + increaseWater;

  // average value of temperature in 10 seconds. Which is make decision
  float avgTemp = 0;
  for(int i = 0; i < sizeof(temps); i++)
    avgTemp += temps[i];
  avgTemp /= sizeof(temps);

  
  if(avgHumi < 60 && avgTemp < 30){
    // enable pump
    digitalWrite(RELAY_PIN, HIGH);
    // fake data to increase humidity 
    increaseWater++;
  }

  if(avgHumi > 80 || avgTemp >= 30){
    // diable pump
    digitalWrite(RELAY_PIN, LOW);
    // remove fake data
    increaseWater = 0;
  }

  // Log detail information
  Serial.print("Temperature: ");
  Serial.print(avgTemp);
  Serial.print("ºC ");
  Serial.print("Humidity: ");
  Serial.println(avgHumi);
  delay(1000);
}
