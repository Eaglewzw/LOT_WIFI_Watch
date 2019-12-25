Set_key  = 7
Up_key   = 6
Down_key = 5
Back_key = 3
System_LED = 0

function KeyLED_Init()
    --set key interrept
    gpio.mode(Set_key, gpio.INT)
    gpio.mode(Up_key, gpio.INT)
    gpio.mode(Down_key, gpio.INT)
    gpio.mode(Back_key, gpio.INT)
    gpio.mode(System_LED, gpio.OUTPUT)

    --set click event
    gpio.trig(Set_key,'up',onBtnEventOfSet)
    gpio.trig(Up_key,'down',onBtnEventOfUp)
    gpio.trig(Down_key,'down',onBtnEventOfDown)
    gpio.trig(Back_key,'down',onBtnEventOfBack)
    
   
    gpio.write(System_LED, gpio.LOW)
    
end


function onBtnEventOfSet()
    gpio.trig(Set_key)--clear
    
    tmr.delay(200)
    gpio.trig(Set_key,'up',onBtnEventOfSet)
    print('set~')
end
function onBtnEventOfUp()

    gpio.trig(Up_key)--clear
    tmr.delay(200)
    gpio.trig(Up_key,'down',onBtnEventOfUp)
    
    print('up~')
end

function onBtnEventOfDown()

    gpio.trig(Down_key)--clear
    tmr.delay(200)
    gpio.trig(Down_key,'down',onBtnEventOfDown)
    
    print('down~')
end

function onBtnEventOfBack()

    gpio.trig(Back_key)--clear
    tmr.delay(200)
    gpio.trig(Back_key,'down',onBtnEventOfBack)
    
    print('back~')
end





KeyLED_Init()

 
