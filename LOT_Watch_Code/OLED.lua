OLED_SDA = 1
OLED_SCL = 2



icon_width = 40             
icon_height = 40 
 
City = "Lanzhou"
Weather = 'Cloudy'
Temperature="21"
Code = "4"


MenuFlag = 0
UpFlag = 0
DownFlag= 0
BackFlag= 0

function Init_OLED(sda,scl)
     sla = 0x3c
     i2c.setup(0, sda, scl, i2c.SLOW)
     disp=u8g2.ssd1306_i2c_128x64_noname(0,sla)
     disp:setFontPosTop()
     
end


function refreshTime()
    disp:clearBuffer()    --clearbuffer
    sec,usec,rate=rtctime.get()
    time = rtctime.epoch2cal(sec+28800,usec,rate)
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
     disp:drawStr(40, 24, string.format("%02d/%02d", time["mon"], time["day"]))
     
     disp:drawStr(40, 24, Temperature)
     
     disp:drawStr(40, 42, Weather)
     --disp:setFont(u8g2.font_fub11_tr)  
     --disp:drawStr(39, 24, string.format("%02d/%02d/%02d", time["year"],time["mon"], time["day"]))
     
     if string.find(Weather,"Sunny") ~= nil then
     disp:drawXBM(0,24, icon_width, icon_height, sunny_bits)
     elseif string.find(Weather,"Clear") ~= nil then
     disp:drawXBM(0,24, icon_width, icon_height, clear_bits)
     elseif string.find(Weather,"Fair") ~= nil then
     disp:drawXBM(0,24, icon_width, icon_height, sunny_bits)
     elseif string.find(Weather,'Cloudy',1) ~= nil then
     disp:drawXBM(0,24, icon_width, icon_height, cloudy_bits)
     elseif string.find(Weather,'Partly Cloudy',1) ~= nil then
     disp:drawXBM(0,24, icon_width, icon_height, cloudy_bits)
     elseif string.find(Weather,'Mostly Cloudy',1) ~= nil then
     disp:drawXBM(0,24, icon_width, icon_height, cloudy_bits)
     elseif string.find(Weather,"Snow") ~= nil then
     disp:drawXBM(0,24, icon_width, icon_height, snow_bits)  
     elseif string.find(Weather,"Rain") ~= nil then
     disp:drawXBM(0,24, icon_width, icon_height, rain_bits) 
     else
         disp:drawXBM(0,24, icon_width, icon_height, over_bits)
     end
     disp:sendBuffer() 
     
    elseif MenuFlag == 1 then
    disp:drawXBM(37,20, 40, 42, Clock_bits) 

    --disp:drawFrame(0, 0,128,16)          --Draw a frame (empty box).
    disp:sendBuffer() 



      
    elseif MenuFlag == 2 then
    
    disp:drawXBM(37,20, 40, 42, Weather_bits) 
  
    --disp:drawFrame(0, 0,128,16)          --Draw a frame (empty box).
    disp:sendBuffer() 

      
    elseif MenuFlag == 3 then
    disp:drawXBM(37,20, 40, 42, Home_bits) 
   
    --disp:drawFrame(0, 0,128,16)          --Draw a frame (empty box).
    disp:sendBuffer() 

      
    elseif MenuFlag == 4 then
    disp:drawXBM(37,20, 40, 42, Set_bits) 
    --disp:drawFrame(0, 0,128,16)          --Draw a frame (empty box).
    disp:sendBuffer() 

    else
        MenuFlag=0
    end 
end


function GetWeather()
    
    if wifi.sta.getip() ~= nil then
     
        srv=net.createConnection(net.TCP,0) 
        srv:on("receive", function(sck, c)
--      print(c)
        --value = sjson.decode(pl)
        i,j=string.find(c, "{")
        sjson_str=string.sub(c, i)
        --print(sjson_str)
        local sjson = require("sjson");
        local json = sjson.decode(sjson_str); 

        print("City: " ..json.results[1]["location"]["name"])
        print("Weather: " ..json.results[1]["now"]["text"])
        print("Code: " ..json.results[1]["now"]["code"])
        print("Temperature: " ..json.results[1]["now"]["temperature"] .." C \n")


        City=string.format("%s",json["results"][1]["location"]["name"])
        Weather=string.format("%s",json["results"][1]["now"]["text"])
        Temperature=string.format("%s",json["results"][1]["now"]["temperature"])
        Code=string.format("%s",json["results"][1]["now"]["code"])


        --print("City",City)
        --print("Weather",Weather)
        --print("Code",Code)
    
      
        end)
        srv:on("connection", function(sck, c)
        sck:send("GET /v3/weather/now.json?key=cinm0okk7gzgtujn&location=lanzhou&language=en&unit=c HTTP/1.1\r\nHost: api.seniverse.com\r\nConnection: keep-alive\r\nAccept: */*\r\n\r\n")
        end)
        srv:connect(80,"api.seniverse.com") 
        
   end
end

Init_OLED(OLED_SDA,OLED_SCL)
GetWeather()   




