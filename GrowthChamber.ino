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


// ***Not sure if we still need the next two lines.***
const byte OC1A_PIN = 9; 
const byte OC1B_PIN = 10;

const word PWM_FREQ_HZ = 25000;                  // Adjust this value to adjust the frequency of the fan.
const word TCNT1_TOP = 16000000/(2*PWM_FREQ_HZ); // DO NOT MODIFY!!! Used in the setPwmDuty function to set OC1RA value.

GyverOLED<SSH1106_128x64,OLED_BUFFER, OLED_SPI, CS, DC, RES> oled; // Allows pins to properly communicate to display.

DHT11 dht(DHT); // Declares sensor name as type DHT11 and define "dht."

float hum;         // Declares humidity.
float temp;        // Declares temperature.
float minHum = 0;  // Declares minimun humidity.
float maxHum = 0;  // Declares maximum humidity.
float minTemp = 0; // Declares minimun temperature.
float maxTemp = 0; // Declares maximum temperature.

int32_t time_off = 0;
int32_t time_on = 0;

// Setup Function*
void setup() {
  oled.init();        // Initializes oled display.
  Serial.begin(9600); // Begins serial monitor for any troubleshooting/testing.
  while(!Serial);     // Waits until the serial monitor is opened by the user.
  oled.home();        // Sets the cursor to the home position on the OLED display.
  oled.update();      // Updates the OLED display to show any changes made.

  pinMode (6, OUTPUT);   // NEW ***
  pinMode (5, OUTPUT);   // NEW ***
  digitalWrite (6, LOW); // NEW ***
  digitalWrite (7, LOW); // NEW ***

  pinMode(OC1A_PIN, OUTPUT);

  // Clears Timer1 control and count registers.
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  setPwmDuty(100); // Change this value 0-100 to adjust duty cycle/fan speed.

  // ***Display (SSH106) does not like next 3 lines. So, I commented them out for now. -Jay***
  //TCCR1A |= (1 << COM1A1) | (1 << WGM11);
  //TCCR1B |= (1 << WGM13) | (1 << CS10);
  //ICR1 = TCNT1_TOP;

}

// Loop function*
void loop(void) {
  oled.home(); // Sets cursor to home position.

  // Checks sensor readings in serial monitor.
  Serial.print(dht.readTemperature());
  Serial.print(dht.readHumidity());
  
  float converted = 0.00; // Declares and define "coverted."

  hum = dht.readHumidity();    // Defines "hum."
  temp= dht.readTemperature(); // Defines "temp."
  minHum = 60;                 // Defines minimum humidity.
  maxHum = 90;                 // Defines maximun humidity.
  minTemp = 20;                // Defines minimum temperature.
  maxTemp = 22;                // Defines maximun temperature.

  oled.println("TEMPERATURES:"); // Displays "TEMPERATURES:" on SSH106.

  // Displays temperature in celsius.
  oled.println(" ");
  oled.print("Celsius: ");
  oled.println(temp);

  // Fahrenheit conversion.
  // T(°F) = T(°C) × 9/5 + 32
  converted = ( temp * 1.8 ) + 32;

  // Displays temperature in fahrenheit.
  oled.print("Fahrenheit: ");
  oled.println(converted);
  oled.println(" ");
  oled.println("----------------------");

  // Displays humidity percentage.
  oled.println(" ");
  oled.print("Humidity: ");
  oled.print(hum);
  oled.println("%");

  oled.update(); // Transfer internal memory to the display.

  // If humidity is below the minimum, turn on humidifier.
  if (hum < minHum) {
    digitalWrite (5, HIGH);
  }
  // If humidity is above the maximum, turn off humidifier and turn on the fan.
  if (hum > maxHum) {
    digitalWrite (5, LOW);
    digitalWrite (6, HIGH);
  }

  // If temperature is below the minimum, turn off the fan.
  if (temp < minTemp) {
    digitalWrite (6, LOW);
  }
  
  // If temperature is above the maximum, turn on the fan.
  if (temp > maxTemp) {
    digitalWrite (6, HIGH);
  }

  // ADD LOGIC TO TURN ON FAN IF ITS BEEN ON FOR TOO LONG AND VICE-VERSA.

  delay(1000);  
}

// setPwmDuty function
void setPwmDuty(byte duty) {
//  OCR1A = (word) (duty*TCNT1_TOP)/100 - 1; // dont remember why we commented this out lol
}
