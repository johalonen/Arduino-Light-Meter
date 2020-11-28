#include <SPI.h>
#include <Adafruit_TCS34725.h> // by Adafruit
#include <Wire.h> // by Arduino
#include "Adafruit_GFX.h" // by Adafruit
#include "Adafruit_PCD8544.h" // by Adafruit
#include <BH1750FVI.h> // by PeterEmbedded

Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 3.3V DC
   Connect GROUND to common ground */

uint8_t ADDRESSPIN = 13;
BH1750FVI::eDeviceAddress_t DEVICEADDRESS = BH1750FVI::k_DevAddress_H;
BH1750FVI::eDeviceMode_t DEVICEMODE = BH1750FVI::k_DevModeContHighRes;

BH1750FVI LightSensor(ADDRESSPIN, DEVICEADDRESS, DEVICEMODE);


/* Initialise with default values (int time = 2.4ms, gain = 1x) */

/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

void setup(void) {
  Serial.begin(9600);

  /* analogWrite(11,220); // PWM of LCD backlight */

  LightSensor.begin(); // BH1750
  display.begin();  // Display code
  tcs.begin();
  display.setContrast(50);
  /* delay(1000); */
  display.clearDisplay(); // clears the screen and buffer
  display.setTextSize(1);
  display.setTextColor(BLACK);
  /* delay(1000); */

  if (tcs.begin()) {
    display.setCursor(5, 15);
    display.println("LighMeter");
    display.setCursor(5, 25);
    display.println("@theonetruejorma");
    display.println("");
    display.display();
    Serial.println("Found TCS34725");
    delay(2000);
  }

  else {
    display.println("No TCS34725 found");
    Serial.println("No TCS34725 found");
    delay(2000);
    while (1);
  }
}

void loop(void) {

  uint16_t r, g, b, c, colorTemp, lux, lux1;
  tcs.getRawData(&r, &g, &b, &c); //&bitwise
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);

  lux1 = LightSensor.GetLightIntensity();

  display.clearDisplay(); // clears the screen and buffer
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(colorTemp, DEC);
  display.print(" K");

  display.setCursor(0, 10);
  display.print("Lux: ");
  display.print(lux, DEC);
  /* display.print(" lx"); */

  display.setCursor(0, 20);
  display.print("R:");
  display.print(r, DEC);

  display.setCursor(40, 20);
  display.print("G:");
  display.print(g, DEC);

  display.setCursor(0, 30);
  display.print("B:");
  display.print(b, DEC);

  display.setCursor(40, 30);
  display.print("C:");
  display.print(c, DEC);
  /* display.setTextSize(1); // 1 is constant */

  display.setCursor(0, 40);
  display.print("BV lux: ");
  display.print(lux1, DEC);

  display.display();

  /* for testing */
  Serial.print("BV Lux: "); Serial.print(lux1, DEC); Serial.print(" ");

  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");
}
