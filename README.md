# Arduino-Sensor-Display
This gathers temp, humidity and measure the distance in centimeters to objects in front of the sensor.  Outputs to LCD Display along with current time (RTC).

## Setup ##
Components and Connections:
   *  Arduino MEGA2560 Controller Board
   *  Ultrasonic Sensor Module (distance) - Pins 5-6 PWM
   *  DHT11 Sensor Module (humidity) - Pin D2 PWM
   *  Thermistor (temperature) - Pin A0
   *  Real Time Clock (RTC) Module (current time) - Pins SDA SCL
   *  LCD1602 module (display output) - Pins 7-12 PWM

Required libraries:
   * HC-SR04 (Ultrasonic Sensor)
   * DS3231 (RTC)
   * DHT (Humidity Sensor)
   * LiquidCrystal (Display)
    
## References ##
* https://www.elegoo.com/download/ - Download Arduino IDE 
* [Docs and Examples](https://www.elegoo.com/tutorial/Elegoo%20The%20Most%20Complete%20Starter%20Kit%20for%20MEGA%20V1.0.2019.01.29.zip)

