/* Temperature and humitidy measurement using the BME280 sensor. Data transmitted over CAN-Bus
 * 
 * www.skpang.co.uk
 * 
 * 1.0 Aug 2018
 * For use with Teensy CAN-Bus Breakout board:
 * http://skpang.co.uk/catalog/teensy-canbus-breakout-board-include-teensy-32-p-1507.html
 * 
 * and BME280 breakout:
 * http://skpang.co.uk/catalog/adafruit-bmp280-i2c-or-spi-barometric-pressure-altitude-sensor-p-1454.html
 * 
 * Require BME280 library from:
 * https://github.com/bolderflight/BME280
 */

#include <FlexCAN.h>
#include "BME280.h"

static CAN_message_t msg;

BME280 bme(Wire,0x77);

uint16_t  CAN_ID =  0x700;    // CAN-Bus ID

float temp;
uint8_t bytes[sizeof(float)];

void setup() {
  
  Can0.begin(500000); //Set CAN speed to 500kbps

  delay(1000);
  Serial.println(F("BME280 CAN Bus Tx test"));

  if (bme.begin() < 0) {
    Serial.println("Error communicating with sensor, check wiring and I2C address");
    while(1){}
  }
}


void loop() {
 
  bme.readSensor();  //Read the sensor

  Serial.print(bme.getPressure_Pa(),6);
  Serial.print("\t");
   
  temp = bme.getTemperature_C();

  memcpy(bytes, &temp, sizeof (temp)); //Convert the temperature in float into 4 bytes
  Serial.print(temp,2);
  Serial.print("\t");
  
  msg.len = 8;
  msg.id = CAN_ID;
  msg.buf[0] = bytes[0];
  msg.buf[1] = bytes[1];
  msg.buf[2] = bytes[2];
  msg.buf[3] = bytes[3];
  msg.buf[4] = 0;
  msg.buf[5] = 0;
  msg.buf[6] = 0;
  Can0.write(msg);                      //Transmit the CAN data out
    
  temp = bme.getHumidity_RH();
  memcpy(bytes, &temp, sizeof (temp)); //Convert the humidity in float into 4 bytes
  Serial.print(temp,2);
  Serial.print("\t");
  
  msg.len = 8;
  msg.id = CAN_ID +1;
  msg.buf[0] = bytes[0];
  msg.buf[1] = bytes[1];
  msg.buf[2] = bytes[2];
  msg.buf[3] = bytes[3];
  msg.buf[4] = 0;
  msg.buf[5] = 0;
  msg.buf[6] = 0;
 
  Can0.write(msg);                       //Transmit the CAN data out
  Serial.println(" ");
  delay(1000);
}
