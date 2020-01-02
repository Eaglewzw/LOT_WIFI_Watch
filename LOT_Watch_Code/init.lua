dofile("icon.lua")
dofile("OLED.lua")
dofile("key.lua")
dofile("wifi.lua")
RefeshTime = tmr.create()
RefeshWeather = tmr.create()



sntp.sync(wifi.sta.getip(),nil,nil)
RefeshWeather:alarm(6000000,tmr.ALARM_AUTO,GetWeather)
--refesh the rtc time every second
RefeshTime:alarm(1000,tmr.ALARM_AUTO,refreshTime)
