



OLEDTimer= tmr.create()

RefeshTime = tmr.create()

RefeshWeather = tmr.create()


wifi.setmode(wifi.STATION)--wifi.STATION
cfg={}
--cfg.ssid="eagle"
--cfg.pwd="12345wzw"
cfg.ssid="eagle"
cfg.pwd="12345wzw"
wifi.sta.config(cfg)
wifi.sta.connect()


wifi.eventmon.register(wifi.eventmon.STA_GOT_IP, function()
     print("Connected, IP is "..wifi.sta.getip())
end)

wifi.eventmon.register(wifi.eventmon.STA_DISCONNECTED, function()
     print("wifi disconnect")
end)


tmr.delay(10000)
dofile("icon.lua")
dofile("OLED.lua")

sntp.sync(wifi.sta.getip(),nil,nil)

RefeshWeather:alarm(8000,tmr.ALARM_AUTO,GetWeather)
--refesh the rtc time every second
RefeshTime:alarm(1000,tmr.ALARM_AUTO,refreshTime)
