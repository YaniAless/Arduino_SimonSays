// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"

#define DHTPIN 2     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

float* humidityTab = new float[10];
float* celsiusTab = new float[10];
float* fahrenheitTab = new float[10];
int i = 0;
  
void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  int j = 0;
  float humidityMoy = 0;
  float celciusMoy = 0;
  float fahrenMoy = 0;

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  } else {
    
    humidityTab[i%10] = h;
    celsiusTab[i%10] = t;
    fahrenheitTab[i%10] = f;
    
    if(i > 9) {
      
      for( j = 0; j < 10; j++) {
        humidityMoy += humidityTab[j];
        celciusMoy += celsiusTab[j];
        fahrenMoy += fahrenheitTab[j];
      }

      Serial.print(F("\n Moyenne glissante de la température "));
      Serial.print(celciusMoy / 10);
      Serial.print(F("\n Moyenne glissante de l'humidité "));
      Serial.print(humidityMoy / 10);
      
      Serial.println(F(""));
      
      // Serial.println(i); index i
      float medianeH = triTab(humidityTab);
      float medianeT = triTab(celsiusTab);
      float medianeHF = triTab(fahrenheitTab);

      Serial.print(F("Mediane de la température "));
      Serial.println(medianeT);
      Serial.print(F("Mediane de l'humidité "));
      Serial.println(medianeH);
      
    }
    i++;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  
  if(h > humidityTab[(i-1) % 10]) {
    
      Serial.println(F(" Humidité en augmentation\n"));
      
   } else if(h == humidityTab[(i-1) % 10] )  {
    
      Serial.println(F(" Humidité stable \n"));
      
   } else {
    
      Serial.println(F(" Humidité en baisse\n"));
      
   }
  if(t > celsiusTab[(i-1) % 10]) {
    
    Serial.println(F(" Température en augmentation\n"));
    
  } else if(t == celsiusTab[(i-1) % 10] )  {

    Serial.println(F(" Température stable\n"));
    
  } else {
    
    Serial.println(F(" Température en baisse\n"));
    
  }
  
  if(f > fahrenheitTab[(i-1) % 10]) {
    
    Serial.println(F(" Température en Fahrenheit augmentation\n"));
    
  } else if(f == fahrenheitTab[(i-1) % 10] )  {
    
    Serial.println(F(" Température en Fahrenheit stable \n"));
    
  } else {
    
    Serial.println(F(" Température en Fahrenheit en baisse\n"));
    
  }
}

float triTab(float* tab) {  
      float* newTab = tab;
      
      for(i = 0; i < 10; i ++) {
        
        float tmp = tab[i];
        
        if(i == 0) {
          
          newTab[i] = tmp;
        } else {
          
          int j;
          for(j = 0; j < i - 1; j++) {
            
            if(newTab[j] > tmp) {
              
              float tmp2 = newTab[j];
              newTab[j] = tmp;
              tmp = tmp2;
            }
          }
          newTab[i] = tmp;
        }
      }
      return newTab[5];
}
