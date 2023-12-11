import machine, onewire, ds18x20, time
import sh1106
import network
import urequests as requests
from machine import Pin, I2C

# 設定 OLED 顯示器
i2c = I2C(scl=Pin(22), sda=Pin(21), freq=400000)
display = sh1106.SH1106_I2C(128, 64, i2c, Pin(16), 0x3c)
display.sleep(False)
display.fill(0)
display.text('Connected', 0, 0, 1)
display.show()

# 設定 ThingSpeak 網址、金鑰
url = "https://api.thingspeak.com/update"
api_key = "JF5LE1V27O8K5U11"

# 設定 wifi SSID 名稱、wifi 密碼與伺服器網址
WIFI_SSID = '你的手機熱點'
WIFI_PASSWORD = '你的熱點密碼'

# 建立 wifi 連線
wifi = network.WLAN(network.STA_IF)
wifi.active(True)
wifi.connect(WIFI_SSID, WIFI_PASSWORD)
while True:
    if not wifi.isconnected():
        # print('connecting to wifi...')
        display.fill(0)   
        display.text('Connecting', 10, 10)   
        display.text('Wifi...', 10, 20)      
        display.show()
        time.sleep(1)
    else:
        # print('wifi connected!')
        display.fill(0)   
        display.text('Wifi connected', 10, 10)      
        display.show()
        break

ds_pin = machine.Pin(17)
ds_sensor = ds18x20.DS18X20(onewire.OneWire(ds_pin))

roms = ds_sensor.scan()
print('Found DS devices: ', roms)

start_time = time.time()
last_write_time = 0
while True:
  ds_sensor.convert_temp()
  time.sleep_ms(750)
  for rom in roms:
    tempture = ds_sensor.read_temp(rom)
    tempture = 12.34
    current_time = time.time() - start_time
    print(f"{current_time} sec\n  {tempture}")
    
    display.fill(0)
    display.text(str(current_time) + " sec", 5, 5, 1)
    display.text(str(tempture) + "C", 5, 20, 1)
    display.show()
    
    if last_write_time == 0 or current_time - last_write_time >= 15:
        query = f"{url}?api_key={api_key}&field1={tempture}"
        display.text("sending data...", 5, 35, 1)
        display.show()
        resp = requests.get(query)
        print("debug: query", query, resp.text)
        if resp.status_code == 200:
            print(resp.text)
            display.text("OK", 5, 50, 1)
            last_write_time = time.time() - start_time
        else:
            print('status_code', resp.status_code)
            display.text(f"ERROR: {resp.status_code}", 5, 50, 1)
        display.show()
  time.sleep_ms(4250)

