Weather = 'Cloudy'
Temperature ="20"
WeatherDate = {'00-01','00-02','00-03'}
TemperaturerRange = {'00/01','00/02','00/03'}
WeatherForcast ={'Snow','Cloudy','Rain'}
Humidity = {'27','38','41'}

MenuFlag = 0
WeatherFlag = 0
BackFlag= 0



function Init_OLED()
    local OLED_SDA = 1
    local OLED_SCL = 2
    sla = 0x3c
    i2c.setup(0, OLED_SDA,OLED_SCL, i2c.SLOW)
    disp=u8g2.ssd1306_i2c_128x64_noname(0,sla)
    disp:setFontPosTop()
     
end

function SwitchWeatherIcon(WeatherString,x0,y0)
    local temp
     if string.find(WeatherString,"Sunny") ~= nil then
        temp=sunny_bits
     elseif string.find(WeatherString,"Clear") ~= nil then
        temp=sunny_bits
     elseif string.find(WeatherString,"Fair") ~= nil then
        temp=sunny_bits
     elseif string.find(WeatherString,'Cloudy') ~= nil then
        temp=cloudy_bits
     elseif string.find(WeatherString,'Overcast') ~= nil then
        temp=cloudy_bits
     elseif string.find(WeatherString,"Shower") ~= nil then
        temp=rain_bits
     elseif string.find(WeatherString,"Storm") ~= nil then
        temp=rain_bits
     elseif string.find(WeatherString,"Snow") ~= nil then
        temp=snow_bits
     elseif string.find(WeatherString,"Sleet") ~= nil then
        temp=rain_bits 
     elseif string.find(WeatherString,"Rain") ~= nil then
        temp=rain_bits
     else
        disp:setFont(u8g2.font_t0_22_tr) 
        disp:drawStr(4, 36,"N/A" )
        disp:drawCircle(20,43,20)
     end
      disp:drawXBM(x0,y0, 40, 40, temp) 
end

function WeatherTranform(WeatherString)
     local result 
     if string.find(WeatherString,"Sunny") ~= nil then
        result = 'Sunny'
     elseif string.find(WeatherString,"Clear") ~= nil then
         result = 'Clear'
     elseif string.find(WeatherString,"Fair") ~= nil then
        result = 'Fair'
     elseif string.find(WeatherString,'Cloudy') ~= nil then
        result = 'Cloudy'
     elseif string.find(WeatherString,'Overcast') ~= nil then
        result = 'Cast'
     elseif string.find(WeatherString,"Shower") ~= nil then
        result = 'Shower' 
     elseif string.find(WeatherString,"Storm") ~= nil then
        result = 'Storm'
     elseif string.find(WeatherString,"Snow") ~= nil then
        result = 'Snow'
     elseif string.find(WeatherString,"Sleet") ~= nil then
        result = 'Sleet'
     elseif string.find(WeatherString,"Rain") ~= nil then
        result = 'Rain'
     else
        result = 'Unkonw' 
     end
     return result

end

function refreshTime()
    disp:clearBuffer()    --clearbuffer
    local sec,usec,rate=rtctime.get()
    local time = rtctime.epoch2cal(sec+28800,usec,rate)
    print(string.format("%04d/%02d/%02d %02d:%02d:%02d", 
                        time["year"], 
                        time["mon"], 
                        time["day"], 
                        time["hour"], 
                        time["min"], 
                        time["sec"]))
    --disp:drawFrame(0, 0,128,16)          --Draw a frame (empty box).
    --width:21   Height:24
     disp:setFont(u8g2.font_fub20_tn)
 --[[switch menu which depend on the MenuFlag
 --MenuFlag==0  Main menu
 --MenuFlag==1  Clock menu
 --MenuFlag==2  Weather menu
 --MenuFlag==3  Home menu
 MenuFlag==4  Set menu--]]
     if MenuFlag == 0 then
         disp:drawStr(6, 1, string.format("%02d", time["hour"]))
         disp:drawStr(37, 0,":")
         disp:drawStr(50, 1, string.format("%02d", time["min"]))
         disp:drawStr(81, 0,":")
         disp:drawStr(94, 1, string.format("%02d", time["sec"]))
         --width:17   Height:31 
         --width:10   Height:19 
         ---u8g2_font_t0_22_tn
         disp:setFont(u8g2.font_t0_22_tr) 
         disp:drawStr(40, 22, string.format("%02d/%02d", time["mon"], time["day"]))
         disp:setFont(u8g2.font_fub20_tn)
         disp:drawStr(94, 24, Temperature)
         disp:drawXBM(112,48, 16, 16, Temperature_Icon)    --display 
         disp:setFont(u8g2.font_t0_22_tr)
         Weather=WeatherTranform(Weather)
         disp:drawStr(40, 40, Weather)
         SwitchWeatherIcon(Weather,0,24)
         disp:sendBuffer() 
         --disp:setFont(u8g2.font_fub11_tr)  
         --disp:drawStr(39, 24, string.format("%02d/%02d/%02d", time["year"],time["mon"], time["day"]))
         
    elseif MenuFlag == 1 then
        disp:drawXBM(0,0, 40, 42, clock_bits) 
        disp:sendBuffer() 
      
    elseif MenuFlag == 2 then
        
        --disp:drawXBM(0,0, 40, 42, Weather_bits)     --display WeatherMenu icon
        --height: 21 width:11
         disp:drawXBM(112,48, 16, 16, Temperature_Icon)    --display
        if WeatherFlag == 0 then
            disp:setFont(u8g2.font_t0_22_tr) 
            disp:drawFrame(115, 0,14,22)
            disp:drawStr(118, 0,'1')
            disp:drawStr(40, 0, WeatherDate[1])
            Weather=WeatherTranform(WeatherForcast[1])
            disp:drawStr(8, 42, Humidity[1])
            disp:drawStr(40, 40, WeatherForcast[1])    --display string of weather
            SwitchWeatherIcon(Weather,0,0)             --display Weather icon         
            disp:setFont(u8g2.font_fub20_tn)
            disp:drawStr(40, 18, TemperaturerRange[1])      --display Temperaturer Range
           

        elseif WeatherFlag == 1 then
            disp:setFont(u8g2.font_t0_22_tr) 
            disp:drawFrame(115, 0,14,22)
            disp:drawStr(118, 0,'2')
            disp:drawStr(40, 0, WeatherDate[2])
            Weather=WeatherTranform(WeatherForcast[2])
            disp:drawStr(8, 42, Humidity[2])
            disp:drawStr(40, 40, WeatherForcast[2])
            SwitchWeatherIcon(Weather,0,0)             --display Weather icon         
            disp:setFont(u8g2.font_fub20_tn)
            disp:drawStr(40, 18, TemperaturerRange[2])      --display Temperaturer Range
          
   
            
        elseif WeatherFlag == 2 then
            disp:setFont(u8g2.font_t0_22_tr) 
            disp:drawFrame(115, 0,14,22)
            disp:drawStr(118, 0,'3')
            disp:drawStr(40, 0, WeatherDate[3])
            Weather=WeatherTranform(WeatherForcast[3])
            disp:drawStr(8, 42, Humidity[3])
            disp:drawStr(40, 40, WeatherForcast[3])
            SwitchWeatherIcon(Weather,0,0)             --display Weather icon         
            disp:setFont(u8g2.font_fub20_tn)
            disp:drawStr(40, 18, TemperaturerRange[3])      --display Temperaturer Range
           
        end
        disp:sendBuffer() 
        
    elseif MenuFlag == 3 then
        disp:drawXBM(0,0, 40, 42, home_bits) 
        disp:sendBuffer() 
    elseif MenuFlag == 4 then
        disp:drawXBM(0,0, 40, 42, set_bits) 
        disp:sendBuffer() 
    else
        MenuFlag=0
    end 
end


function GetWeather()
    
    if wifi.sta.getip() ~= nil then
     
        srv=net.createConnection(net.TCP,0) 
        srv:on("receive", function(sck, c)
        local i,j=string.find(c, "{")
        local sjson_str=string.sub(c, i)

        local sjson = require("sjson");
        local json = sjson.decode(sjson_str); 

        Weather=string.format("%s",json["results"][1]["now"]["text"])
        Temperature=string.format("%s",json["results"][1]["now"]["temperature"])
        --City=string.format("%s",json["results"][1]["location"]["name"])
        --Code=string.format("%s",json["results"][1]["now"]["code"])

        --print("City: " ..json.results[1]["location"]["name"])
        --print("Weather: " ..json.results[1]["now"]["text"])
        --print("Code: " ..json.results[1]["now"]["code"])
        --print("Temperature: " ..json.results[1]["now"]["temperature"] .." C \n")

        end)
        srv:on("connection", function(sck, c)
        sck:send("GET /v3/weather/now.json?key=cinm0okk7gzgtujn&location=lanzhou&language=en&unit=c HTTP/1.1\r\nHost: api.seniverse.com\r\nConnection: keep-alive\r\nAccept: */*\r\n\r\n")
        end)
        srv:connect(80,"api.seniverse.com") 
   end
   
end

function GetThreeDaysWeather()
if wifi.sta.getip() ~= nil then
    srv=net.createConnection(net.TCP,0) 
    srv:on("receive", function(sck, c)
  
    local i,j=string.find(c, "{")
    local sjson_str=string.sub(c, i)
 
    local sjson = require("sjson");
    local json = sjson.decode(sjson_str); 
    local temp = ''

    for i=1,3 do
        WeatherDate[i] = string.format("%s",json["results"][1]["daily"][i]["date"])
        WeatherDate[i] = string.sub(WeatherDate[i],-5)
        WeatherForcast[i] = string.format("%s",json["results"][1]["daily"][i]["text_day"])
        Humidity[i] = string.format("%s",json["results"][1]["daily"][i]["humidity"])
        --get weather of Temperature range and put it into variable TemperaturerRange
        TemperaturerRange[i] = string.format("%s",json["results"][1]["daily"][i]["high"])
        temp = string.format("%s",json["results"][1]["daily"][i]["low"])
        TemperaturerRange[i] = TemperaturerRange[i]..'/'..temp
    end
    
    --this code is used to test to print the weather of later three days forcast
    --print("City: " ..json.results[1]["location"]["name"])
    --print("Date: " ..json.results[1]["daily"][1]["date"])
    --print("Weather: " ..json.results[1]["daily"][1]["text_day"])
    --print("Code: " ..json.results[1]["daily"][1]["code_day"])
    --print("Temperature-High: " ..json.results[1]["daily"][1]["high"])
    --print("Temperature-Low: " ..json.results[1]["daily"][1]["low"] .." C\n")
    
    --print("Date: " ..json.results[1]["daily"][2]["date"])
    --print("Weather: " ..json.results[1]["daily"][2]["text_day"])
    --print("Code: " ..json.results[1]["daily"][2]["code_day"])
    --print("Temperature-High: " ..json.results[1]["daily"][2]["high"])
    --print("Temperature-Low: " ..json.results[1]["daily"][2]["low"] .." C\n")
    
    --print("Date: " ..json.results[1]["daily"][3]["date"])
    --print("Weather: " ..json.results[1]["daily"][3]["text_day"])
    --print("Code: " ..json.results[1]["daily"][3]["code_day"])
    --print("Temperature-High: " ..json.results[1]["daily"][3]["high"])
    --print("Temperature-Low: " ..json.results[1]["daily"][3]["low"] .." C\n")
    

    end)
    srv:on("connection", function(sck, c)
    sck:send("GET /v3/weather/daily.json?key=cinm0okk7gzgtujn&location=lanzhou&language=en&unit=c&start=0&days=4 HTTP/1.1\r\nHost: api.seniverse.com\r\nConnection: keep-alive\r\nAccept: */*\r\n\r\n")
    end)
    srv:connect(80,"api.seniverse.com") 
    end
    
end

Init_OLED() 
