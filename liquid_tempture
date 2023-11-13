import machine, onewire, ds18x20, time
from machine import Pin, I2C
import sh1106

i2c = I2C(scl=Pin(22), sda=Pin(21), freq=400000)
display = sh1106.SH1106_I2C(128, 64, i2c, Pin(16), 0x3c)
display.sleep(False)
display.fill(0)
display.text('Connected', 0, 0, 1)
display.show()

ds_pin = machine.Pin(17)
ds_sensor = ds18x20.DS18X20(onewire.OneWire(ds_pin))

roms = ds_sensor.scan()
print('Found DS devices: ', roms)

start_time = time.time()
while True:
  ds_sensor.convert_temp()
  time.sleep_ms(750)
  for rom in roms:
    tempture = ds_sensor.read_temp(rom)
    current_time = time.time() - start_time
    print(current_time, "\n", tempture)
    
    display.fill(0)
    display.text(str(current_time), 5, 5, 1)
    display.text(str(tempture), 5, 20, 1)
    display.show()
  time.sleep_ms(4250)
