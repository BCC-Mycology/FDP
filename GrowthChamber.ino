// libraries:
#include <DHT11.h>     // Sensor (DHT11) library
#include <GyverOLED.h> // Display (SSH106) library
#include <charMap.h>
#include <icons_7x7.h>
#include <icons_8x8.h>

#define MOSI 11 // MOSI to 11th pin on Arduino.
#define CLK 13  // CLK to 13th pin on Arduino.
#define DC 9    // DC to 9th pin on Arduino.
#define CS 10   // CS to 10th pin on Arduino.
#define RES 8   // RES to 8th pin on Arduino.
#define DHT 2   // DHT to 2nd pin on Arduino.

#define HIGH_T 80
#define LOW_T 60
#define HIGH_H 96
#define LOW_H 88

#define MAX_TEMP 99         // Defines maximum temperature for chamber.
#define MIN_TEMP 80         // Defines minimum temperature for chamber.
#define MAX_HUM 99          // Defines maximum temperature for chamber.
#define MIN_HUM 80          // Defines minimum temperature for chamber.
#define FAN_TIME_ON 30000   // Defines amount of time fan will be on each time it is triggered to turn on.
#define FAN_TIME_OFF 600000 // Defines maximum amount of time fan is allowed to be off.


// 7-3-42
const byte OC1A_PIN = 9;
const byte OC1B_PIN = 10;

const word PWM_FREQ_HZ = 25000;                  // Adjust this value to adjust the frequency of the fan.
const word TCNT1_TOP = 16000000/(2*PWM_FREQ_HZ); // DO NOT MODIFY!!! Used in the setPwmDuty function to set OC1RA value.

GyverOLED<SSH1106_128x64,OLED_BUFFER, OLED_SPI, CS, DC, RES> oled; // Allows pins to properly communicate to display.

DHT11 dht(DHT); // declare sensor name

float hum;  // Declares humidity.
float temp; // Declares temperature.

int32_t time_off = 0;
int32_t time_on = 0;

void setup() {
  oled.init();        // Initialize oled display.
  Serial.begin(9600); // Begin serial monitor for any troubleshooting/testing.
  while(!Serial);     // Wait until the serial monitor is opened by the user.
  oled.home();        // Set the cursor to the home position on the OLED display.
  oled.update();      // Update the OLED display to show any chnages made

pinMode(OC1A_PIN, OUTPUT);

// Clear Timer1 control and count registers.
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

setPwmDuty(100); // Change this value 0-100 to adjust duty cycle/fan speed.

TCCR1A |= (1 << COM1A1) | (1 << WGM11);
TCCR1B |= (1 << WGM13) | (1 << CS10);
ICR1 = TCNT1_TOP;
}

void loop(void) {
  oled.home(); // sets cursor

  // check serial monitor readings
  Serial.print(dht.readTemperature());
  Serial.print(dht.readHumidity());

  // hold name for c converted to f 
  float converted = 0.00; 

  // declare hum & temp value
  hum = dht.readHumidity(); 
  temp= dht.readTemperature();

  // temperature display
  oled.println("TEMPERATURES:");

  // display temp. in celsius 
  oled.println(" ");
  oled.print("Celsius: ");
  oled.println(temp);

  // fahrenheit conversion
  // T(°F) = T(°C) × 9/5 + 32
  converted = ( temp * 1.8 ) + 32;

  // display temp. in fahrenheit
  oled.print("Fahrenheit: ");
  oled.println(converted);
  oled.println(" ");
  oled.println("----------------------");

  // display humidity percentage
  oled.println(" ");
  oled.print("Humidity: ");
  oled.print(hum);
  oled.println("%");

  // transfer internal memory to the display
  oled.update(); 


  if (temp > MAX_TEMP){
    // turn on fan
  }
  
  if (hum > MAX_HUM){
    // turn on fan
    // turn off humidifier
  }

  if (hum > MAX_HUM){
    // turn off fan
    // turn on humidifier
    fan_time = millis() - fan_time;
  }

  if (time_off > FAN_TIME_OFF){
    // turn fan on
  }

  if (time_on > FAN_TIME_ON){
    // turn fan off
  }

  delay(200);  
}

//setPwmDuty(0); //Change this value 0-100 to adjust duty cycle
void setPwmDuty(byte duty) {
  OCR1A = (word) (duty*TCNT1_TOP)/100 - 1;
}
