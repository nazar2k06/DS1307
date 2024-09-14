#include <DS1307.h>

#define SET_TIME_FLAG true
#define GMT 3
#define HOUR 21
#define MINUTE 9
#define SECOND 0
#define DAY 14
#define MONTH 9
#define YEAR 2024

Clock clk;
DS1307 rtc;

void setup() {
  Serial.begin(9600);

  Serial.println("Begin");
  switch(rtc.begin()) {
  case 0:
    Serial.println("OK");
    break;
  case 1:
    Serial.println("Read request error");
    break;
  case 2:
    Serial.println("Receive error");
    break;
  case 3:
    Serial.println("On error");
    break;
  }

  if (SET_TIME_FLAG) {
    Serial.println("\nSet time");
    clk.setTime(GMT, HOUR, MINUTE, SECOND, DAY, MONTH, YEAR);
    
    switch(rtc.setTime(clk.getTime(0)) ) {
    case 0:
      Serial.println("OK");
      break;
    case 1:
      Serial.println("ERR");
      break;
    }
  }
  else {
    Serial.println("\nRead time");
    clk.setTime(0, rtc.getTime());
  }
}

void loop() {
  Serial.println("\nStatus");
  
  switch (rtc.status()) {
  case 0:
    Serial.println("OK");
    break;
  case 1:
    Serial.println("Read request error");
    break;
  case 2:
    Serial.println("Receive error");
    break;
  case 3:
    Serial.println("On error");
    break;
  }

  clk.printDate(GMT);
  delay(2000);
}
