#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <arduino.h>

#define I2C_SDA_PIN 6
#define I2C_SCL_PIN 7

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int ledPin = 25;

int loop_count = 0;

void logoshow(int count)
{
    display.clearDisplay();

    display.setTextSize(1);              // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setCursor(0, 0);             // Start at top-left corner
    display.println(F("MaPie RP2040 OLED"));
    display.setCursor(0, 20); // Start at top-left corner
    display.print(F("TEST DEMO #"));
    display.println(count);
    display.println(F(""));
    display.println(F("Hello, Makerfabs!"));
    display.display();
    //delay(1000);
}

void setup()
{
    // declare pin to be an output:
  pinMode(ledPin, OUTPUT);
  //Wire.begin(I2C_SDA_PIN,I2C_SCL_PIN);
  //Wire.Wire(I2C_SDA_PIN,I2C_SCL_PIN);
  
  Wire.begin();
  //MbedI2C.setClock(100000UL);
  Wire.setClock(100000UL);
  Serial.begin(115200);
  Serial.println("nI2C Scanner 100Khz");
/**  
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  delay(1000);
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    Wire.beginTransmission(address);
    delay(100);
    error = Wire.endTransmission();
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
      nDevices++;
      delay(1000);
    }
    else if (error==4)
    {
      Serial.print("Unknow error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices founded");
  else
    Serial.println("done!");
  delay(5000);           // wait 5 seconds for next scan
**/
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3C for 128x32
      Serial.println(F("SSD1306 allocation failed"));
      for (;;)
        ; // Don't proceed, loop forever
  }
  Serial.println("SSD1306 found");

  display.clearDisplay();

  for(int16_t x=0; x<display.width(); x+=1) //SCREEN_WIDTH
  {
    // The INVERSE color is used so rectangles alternate white/black
    //display.fillRect(i, i, display.width()-i*2, display.height()-i*2, SSD1306_INVERSE);
    //display.fillRect(i, i, display.width(), display.height(), SSD1306_INVERSE);
    for(int16_t y=0; y<display.height(); y+=1) //  
    {
      display.drawPixel(x, y, SSD1306_WHITE);
      //display.display(); // Update screen with each newly-drawn rectangle
    }
    delay(1);
  }
  display.display();
  delay(2000);
  display.clearDisplay();  
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1); // Draw 2X-scale text
  display.setCursor(2, 0);
  display.println("MaPie RP2040");
  display.println("0.96\" OLED");
  display.display();

}
void loop()
{
  Serial.print("Serial #");
  Serial.println(loop_count);
  loop_count++;
  logoshow(loop_count);
  digitalWrite(ledPin, (loop_count%2));

  delay(1000);

}
