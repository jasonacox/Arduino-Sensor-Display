/*
   Temp, Humidity, Time and Ultrasound Distance Sensor with Display

   This gathers temp, humidity and measure the distance in centimeters to objects in front of the sensor.
   Outputs to LCD Display along with current time (RTC).

   Components and Connections:
     Arduino MEGA2560 Controller Board
     Ultrasonic Sensor Module (distance) - Pins 5-6 PWM
     DHT11 Sensor Module (humidity) - Pin D2 PWM
     Thermistor (temperature) - Pin A0
     Real Time Clock (RTC) Module (current time) - Pins SDA SCL
     LCD1602 module (display output) - Pins 7-12 PWM

  Required libraries:
    HC-SR04 (Ultrasonic Sensor)
    DS3231 (RTC)
    DHT (Humidity Sensor)
    LiquidCrystal (Display)
    
*/
// includes
#include "SR04.h"
#include <dht_nonblocking.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <DS3231.h>

// static values
#define TRIG_PIN 6 
#define ECHO_PIN 5
#define TEMP_PIN 0
#define DHT_SENSOR_TYPE DHT_TYPE_11

// globals
long count = 0;
float dhttemp = 0.0;
float dhthum = 0.0;
static const int DHT_SENSOR_PIN = 2;
SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);
long a;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );
DS3231 clock;
RTCDateTime dt;
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  uint8_t SpecialChar [8] = { 0x00, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00, 0x00 };
  lcd.createChar(0, SpecialChar);
  lcd.setCursor(0, 0);
  lcd.print("-SENSOR DISPLAY-");
  lcd.setCursor(0, 1);
  lcd.print("Distance & Temp");
  Serial.println( "SENSOR DISPLAY - Running" );
  Serial.println("Initialize RTC module");
  // Initialize DS3231
  clock.begin();
  // Uncomment next line to set date based on complie time values
  // clock.setDateTime(__DATE__, __TIME__);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED
  delay(1000);
}


/*
   DHT11 - Poll for temp and humidity, keeping the state machine alive.  Returns
   true if a measurement is available.
*/
static bool measure_environment( float *temperature, float *humidity )
{
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every four seconds. */
  if ( millis( ) - measurement_timestamp > 3000ul )
  {
    if ( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return ( true );
    }
  }

  return ( false );
}

/* main Arduino Loop

    Uses count to limit calls to DHT11
*/
void loop() {
  count++;
  if (count > 1000) count = 1;

  // init variables
  int tempReading = analogRead(TEMP_PIN);
  double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
  tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
  float tempC = tempK - 273.15;            // Convert Kelvin to Celcius
  float tempF = (tempC * 9.0) / 5.0 + 32.0; // Convert Celcius to Fahrenheit
  float temperature;
  float humidity;
  char buf[40], buf2[40], buf3[40]; // temp storage for LCD output

  if (((count % 100) == 0) || count == 1) {
    // poll DHT11 for temp and humidity
    digitalWrite(LED_BUILTIN, HIGH);  // flash LED to indicate reading
    delay(100);
    // Measure temperature and humidity - true, then a measurement is available.
    while ( measure_environment( &temperature, &humidity ) != true ); /// wait till we have an answer
    dhttemp = temperature;
    dhthum = humidity;
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }

  // distance from Ultrasonic Sensor Module
  a = sr04.Distance();

  // real time clock
  dt = clock.getDateTime();

  // send data to display
  lcd.setCursor(0, 0);
  sprintf(buf, "D=%4dcm       ", a);  // distance
  lcd.print(buf);

  lcd.setCursor(0, 1);
  sprintf(buf, "T=%s%cF H=%s%%", dtostrf(tempF, 5, 1, buf2), 223, dtostrf(dhthum, 3, 0, buf3)); // temp and humidity
  lcd.print(buf);

  lcd.setCursor(11, 0);
  sprintf(buf, "%02d:%02d", dt.hour, dt.minute);  // current time
  lcd.print(buf);

  // for fun - a spinner
  lcd.setCursor(9, 0);
  switch (count % 4) {
    case 0: lcd.print("|"); break;
    case 1: lcd.print("/"); break;
    case 2: lcd.print("-"); break;
    case 3: lcd.print(char(0)); break;  // special char for backslash
  }

  delay(250);
}
