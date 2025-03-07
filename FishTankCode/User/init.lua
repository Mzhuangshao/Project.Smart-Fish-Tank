print("8266 Success", "\n")
print("initializing...", "\n")
local getTemperatureData = require("ds18b20") -- 引用前置库ds18b20
local getTemperatureDataPin = 3               -- 定义获取温度数据的GPIO口 -- D3 引脚连接DS18B20
local WaterLevelData                          -- 获取水位
local WaterQualityData                        -- 获取水质
local TemperatureData                    -- 获取温度
-- local Device_DS18B20                          -- 定义DS18B20传感器
-- local Device_Water_Sensor                     -- 定义水位传感器
-- local Device_Water_Quality                    -- 定义水质传感器
local debugMode = false -- 调试模式开关
print("initializing success", "\n")

-- ########################### --

print("Checking Device...", "\n")
do
    
end
print("Checking Device success", "\n")

-- ########################### --

function printDataLocal()     -- 本地打印数据
    if debugMode == true then -- 调试模式
        print(" ===== Debug mode is on ===== ")
    else                      -- 正常输出模式
        print("--Start Print Data--")
        print("Water Level: " .. WaterLevelData, "\n")     -- 打印水位数据
        print("Temperature: ",TemperatureData,"°C\n") -- 打印DS18B20传感器数据
        print("Water Quality: " .. WaterQualityData, "\n") -- 打印水质数据
        -- print("--End Print Data--")
    end
end

-- ########################### --

local Timer1 = tmr.create()
Timer1:register(2000, 1,
    function()
        getTemperatureData:read_temp(TemperatureDataReadout, getTemperatureDataPin, getTemperatureData.C)
        printDataLocal()
    end)
Timer1:start()

-- ########################### --

-- 获取水位
do
    local getWaterLevelPin = 0                  -- 定义获取水位数据的ADC channel -- A0
    WaterLevelData = adc.read(getWaterLevelPin) -- 返回ADC读取的水位数据
end

-- 获取温度
function TemperatureDataReadout(temp)
    for _, temp in pairs(temp) do
        if temp then
            TemperatureData = string.format("%s", temp)
        end
    end
end

-- 获取水质
do
    local getWaterQualityPin = 0                           -- 定义获取水质数据的GPIO引脚 -- D0
    gpio.mode(getWaterQualityPin, gpio.INPUT, gpio.PULLUP) -- 设置GPIO为输入模式，并使能上拉电阻
    WaterQualityData = gpio.read(getWaterQualityPin)       -- 返回GPIO读取的水质数据
end