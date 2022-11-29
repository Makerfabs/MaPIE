import machine
import time
from ssd1306 import SSD1306_I2C

sda=machine.Pin(6)
scl=machine.Pin(7)

def main():
  time.sleep(2)
  i2c=machine.I2C(1,sda=sda, scl=scl, freq=400000)
  for dev in i2c.scan():
      print('%#x'%dev)
  
  lcd = SSD1306_I2C(128,64,i2c)
  
  i = 0;


  while True:
      lcd.fill(0)
      lcd.text("Helloworld:",0,0)
      text = "num:" + str(i)
      i += 1
      lcd.text(text,0,10)
      lcd.show()
      time.sleep(2)

if __name__ == "__main__":
    main()