--[[
    local Set_key  = 7
    local Up_key   = 6
    local Down_key = 5
    local Back_key = 3
    local System_LED = 0
--]]







function KeyLED_Init()
    local Set_key  = 7
    local Up_key   = 6
    local Down_key = 5
    local Back_key = 3
    local System_LED = 0
    --set key interrept
    gpio.mode(Set_key, gpio.INT)
    gpio.mode(Up_key, gpio.INT)
    gpio.mode(Down_key, gpio.INT)
    gpio.mode(Back_key, gpio.INT)
    gpio.mode(System_LED, gpio.OUTPUT)

    --set click event
    --set high level or low call the function
    gpio.trig(Set_key,'up',onBtnEventOfSet)
    gpio.trig(Up_key,'down',onBtnEventOfUp)
    gpio.trig(Down_key,'down',onBtnEventOfDown)
    gpio.trig(Back_key,'down',onBtnEventOfBack)
   
    gpio.write(System_LED, gpio.LOW)
    
end


function onBtnEventOfSet()
    local Set_key  = 7
    local i=0
    tmr.delay(1800)
    if gpio.read(Set_key) == 1 then
        while(gpio.read(Set_key) == 1) do
            i=i+1
            if i>50 then
                i=0
                break
            end
        end
       
        MenuFlag=MenuFlag+1
            if MenuFlag == 5 then
                MenuFlag = 0
            end
        disp:clearBuffer()    --clearbuffer
    end
   print('MenuFlag=',MenuFlag)
    
    
end
function onBtnEventOfUp()
    local Up_key   = 6
    local i=0;
    tmr.delay(1800)
    if gpio.read(Up_key) == 0 then
    
        while(gpio.read(Up_key) == 0) do
            i=i+1
            if i>50 then
                i=0
                break
            end
        end
        if MenuFlag == 2 then
           WeatherFlag = WeatherFlag+1
           if WeatherFlag == 3 then
               WeatherFlag = 0
           end  
        end
       disp:clearBuffer()    --clearbuffer
    end
    print(' WeatherFlag', WeatherFlag)
  
end

function onBtnEventOfDown()

    local Down_key = 5
    local i=0;
    tmr.delay(1800)
    if gpio.read(Down_key) == 0 then
    
        while(gpio.read(Down_key) == 0) do
            i=i+1
            if i>50 then
                i=0
                break
            end
        end
        if MenuFlag == 2 then
            if WeatherFlag >=1 then
                WeatherFlag = WeatherFlag-1
            elseif WeatherFlag <= 0 then
               WeatherFlag = 0
           end  
        end
        
       disp:clearBuffer()    --clearbuffer
    end
    print(' WeatherFlag', WeatherFlag)
end

function onBtnEventOfBack()
    local Back_key = 3
    local i=0;
    tmr.delay(1800)
    if gpio.read(Back_key) == 0 then
    
        while(gpio.read(Back_key) == 0) do
            i=i+1
            if i>50 then
                i=0
                break
            end
        end
        
        disp:clearBuffer()    --clearbuffer
    end
    print('back~')
end





KeyLED_Init()
