wifi.setmode(wifi.STATION)--wifi.STATION

local cfg={}
cfg.ssid="Eagle"
cfg.pwd="12345wzw"
wifi.sta.config(cfg)
wifi.sta.connect()

m = mqtt.Client("clientid",60)
wifi.eventmon.register(wifi.eventmon.STA_GOT_IP, function()
     print("Connected, IP is "..wifi.sta.getip())
     GetWeather()  
     refreshTime()
     GetThreeDaysWeather()
     m:connect("39.105.5.215", 1883, 0, function(client)
     print("connected")
     client:subscribe("EagleNodeMcu", 0, function(client) print("subscribe success") end)
     client:publish("/topic", "hello", 0, 0, function(client) print("sent") end)
    end)
end)

wifi.eventmon.register(wifi.eventmon.STA_DISCONNECTED, function()
     print("wifi disconnect")
end)

m:on("message", function(client, topic, data)
  print(topic .. ":" )
  if data ~= nil then
    print(data)
  end
end)
