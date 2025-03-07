print("Success", "\n")

local getTemperatureData = require("ds18b20")
local pin = 3

function TemperatureReadout(temp)
    if getTemperatureData.sens then
        print("Total number of DS18B20 sensors: " .. #getTemperatureData.sens)
        for i, s in ipairs(getTemperatureData.sens) do
            print(string.format("  sensor #%d address: %s%s", i,
                ('%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X'):format(s:byte(1, 8)), s:byte(9) == 1 and " (parasite)" or ""))
        end
    end
    for addr, temp in pairs(temp) do
        print(string.format("Sensor[%s] Temp: %s °C",
            ('%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X'):format(addr:byte(1, 8)), temp))
    end

end

-- 每隔5秒读取一次温度
-- ALARM_AUTO 重复打印  ALARM_SINGLE, 只打印一次
local T = tmr.create()
T:register(5000, 1, function()
    getTemperatureData:read_temp(TemperatureReadout, pin, getTemperatureData.C)
end)
T:start();
