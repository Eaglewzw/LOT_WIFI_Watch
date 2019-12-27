Set_key  = 7
Up_key   = 6
Down_key = 5
Back_key = 3
System_LED = 0




local i=0

function KeyLED_Init()
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
    tmr.delay(1800)
    if gpio.read(Set_key) == 1 then
        while(gpio.read(Set_key) == 1) do
            i=i+1
            if i>50 then
                i=0
                break
            end
        end
        disp:clearBuffer()    --clearbuffer
        MenuFlag=MenuFlag+1
            if MenuFlag == 5 then
                MenuFlag = 0
            end
    end
   print('MenuFlag=',MenuFlag)
    
    
end
function onBtnEventOfUp()
    tmr.delay(1800)
    if gpio.read(Up_key) == 0 then
    
        while(gpio.read(Up_key) == 0) do
            i=i+1
            if i>50 then
                i=0
                break
            end
        end
        
       disp:clearBuffer()    --clearbuffer
    end
    print('up~')
  
end

function onBtnEventOfDown()
    tmr.delay(1800)
    if gpio.read(Down_key) == 0 then
    
        while(gpio.read(Down_key) == 0) do
            i=i+1
            if i>50 then
                i=0
                break
            end
        end
        
       disp:clearBuffer()    --clearbuffer
    end
    print('down~')
end

function onBtnEventOfBack()
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

 
