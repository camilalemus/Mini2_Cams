// Adafruit IO Publish & Subscribe Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/

// this int will hold the current count for our sketch
int count = 0;
String flag_verde;
String flag_rojo;
String tempe;

// Track time of last published messages and limit feed->save events to once
// every IO_LOOP_DELAY milliseconds.
//
// Because this sketch is publishing AND subscribing, we can't use a long
// delay() function call in the main loop since that would prevent io.run()
// from being called often enough to receive all incoming messages.
//
// Instead, we can use the millis() function to get the current time in
// milliseconds and avoid publishing until IO_LOOP_DELAY milliseconds have
// passed.
#define IO_LOOP_DELAY 5000
unsigned long lastUpdate = 0;

// set up the 'counter' feed
AdafruitIO_Feed *LED_VERDE = io.feed("GREEN_LED");
AdafruitIO_Feed *LED_ROJO = io.feed("RED_LED");
AdafruitIO_Feed *SENSOR = io.feed("BMP280");

void setup() {

  // start the serial connection
  Serial.begin(9600);
  Serial2.begin(9600,SERIAL_8N1,16,17);

  // wait for serial monitor to open
  while(! Serial);
  while(! Serial2);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  LED_VERDE->onMessage(handleMessage1);
  LED_ROJO->onMessage(handleMessage2);

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  LED_VERDE->get();
  LED_ROJO->get();

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
//
//    LED_VERDE->save(count);
//    LED_ROJO->save(count);
//
//    if (Serial2.read() == '#'){
//     tempe = 'Temperatura= ';
//     tempe = tempe + Serial2.read();
//     tempe = tempe + Serial2.read();
//     tempe = tempe + char(Serial2.read());
//     tempe = tempe + Serial2.read();
//     tempe = tempe + Serial2.read();
//     tempe = tempe + char(167)+'C';
//    }

      //SENSOR -> save(tempe);

      delay(100);
      //Serial.print(tempe);
      Serial.print(Serial2.read());
}

// this function is called whenever a 'counter' message
// is received from Adafruit IO. it was attached to
// the counter feed in the setup() function above.
void handleMessage1(AdafruitIO_Data *data) {

  flag_verde = data->value();
  if (flag_verde == "ON"){
     Serial.print("ON");
     Serial2.write('A');
  }
  else if (flag_verde == "OFF") {
     Serial.print("OFF");
     Serial2.write('B');
  }

}

void handleMessage2(AdafruitIO_Data *data) {

  flag_rojo = data->value();
    if (flag_rojo == "ON"){
     Serial.print("ON");
     Serial2.write('C');
    }
    else if (flag_rojo == "OFF") {
     Serial.print("OFF");
     Serial2.write('D');
    }

}
