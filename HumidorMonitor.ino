#include "config.h"                   // Adafruit IO configuration for IoT
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2                      // Digital pin connected to the DHT sensor (maps to pin 4 on the ESP8266)
#define DHTTYPE    DHT11              // DHT 11 (sensor type)

int delayMillis = 30 * 1000;          // interval in milliseconds between sensor readings (30 * 1000 = 30 seconds)

DHT_Unified dht(DHTPIN, DHTTYPE);

AdafruitIO_Feed *humidortemp = io.feed("Humidor Temperature");                          // temperature data sent to feed
AdafruitIO_Feed *humidorhumid = io.feed("Humidor Humidity");                            // humidity data sent to feed

void setup() {
  //
  // Initialize sensor
  dht.begin();
  sensor_t sensor;
  
  Serial.begin(9600);
  Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  //
  // Initialize Adafruit IO connection
  Serial.print("Connecting to Adafruit IO");
  io.connect();                                     // connect to io.adafruit.com
  while((io.status() < AIO_CONNECTED)){   // wait for a connection or until timeout
    Serial.print(".");
    delay(500);
  } 
  Serial.println();
  Serial.println(io.statusText());
}

void loop() {
  io.run();                   // always fired top of loop to keep connected and respond to Adafruit IO actions
  delay(delayMillis);         // 30 second interval
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
    humidortemp->save("999");
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
  humidortemp->save(event.temperature);
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
    humidorhumid->save("999");
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
    humidorhumid->save(event.relative_humidity);
  }
}
