print("8266 Success", "\n")
print("initializing...", "\n")
local getTemperatureData = require("ds18b20") -- 引用前置库ds18b20
local getTemperatureDataPin = 3               -- 定义获取温度数据的GPIO口 -- D3 引脚连接DS18B20
local WaterLevelData                          -- 获取水位
local WaterQualityData                        -- 获取水质
local TemperatureData = 0                     -- 获取温度
-- local Device_DS18B20                          -- 定义DS18B20传感器
-- local Device_Water_Sensor                     -- 定义水位传感器
-- local Device_Water_Quality                    -- 定义水质传感器
local WaterLevelStateTooLow = 480
local WaterLevelStateLow = 520
local WaterLevelStateHigh = 560
local WaterLevelStateTooHigh = 600
local WaterQualityStateNormal = 1
local TemperatureStateTooHigh = 35
local TemperatureStateHigh = 32
local TemperatureStateLow = 28
local TemperatureStateTooLow = 25
local debugMode = false -- 调试模式开关
print("initializing success", "\n")

-- ########################### --

print("Checking Device...", "\n")
do

end
print("Checking Device success", "\n")

-- ########################### --

function printDataLocal()                                 -- 本地打印数据
    if debugMode == true then                             -- 调试模式
        print(" ===== Debug mode is on ===== ")
    else                                                  -- 正常输出模式
        print("--Start Print Data--")
        print("Water Level: " .. WaterLevelData)          -- 打印水位数据
        print("Temperature: " .. TemperatureData .. "°C") -- 打印DS18B20传感器数据
        print("Water Quality: " .. WaterQualityData)      -- 打印水质数据
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
    local getWaterLevelPin = 0 -- 定义获取水位数据的ADC channel -- A0
    local function getWaterLevelData()
        WaterLevelData = adc.read(getWaterLevelPin) -- 返回ADC读取的水位数据
    end

    Ticker1 = tmr.create()
    Ticker1:alarm(1000, tmr.ALARM_AUTO, getWaterLevelData)
end

-- 获取温度
function TemperatureDataReadout(temp)
    for _, temp in pairs(temp) do
        if temp then
            TemperatureData = tonumber(string.format("%.2f", temp))
            --tonumber(TemperatureData)
        end
    end
end

-- 获取水质
do
    local getWaterQualityPin = 2                           -- 定义获取水质数据的GPIO引脚 -- D0
    gpio.mode(getWaterQualityPin, gpio.INPUT, gpio.PULLUP) -- 设置GPIO为输入模式，并使能上拉电阻
    function getWaterQualityData()
        WaterQualityData = gpio.read(getWaterQualityPin)   -- 返回GPIO读取的水质数据
    end

    Ticker2 = tmr.create()
    Ticker2:alarm(1000, tmr.ALARM_AUTO, getWaterQualityData)
end

function WaterLevelCheck()
    if WaterLevelData == nil then
        print("Water Level Unknown")
    elseif WaterLevelData > WaterLevelStateTooHigh then
        print("Water Level is too high")
    elseif WaterLevelData > WaterLevelStateHigh then
        print("Water Level is high")
    elseif WaterLevelData > WaterLevelStateLow then
        print("Water Level is normal")
    elseif WaterLevelData > WaterLevelStateTooLow then
        print("Water Level is low")
    else
        print("Water Level is too low")
    end
end

function TemperatureCheck()
    if TemperatureData == nil then
        print("Temperature Unknown")
    elseif TemperatureData > TemperatureStateTooHigh then
        print("Temperature is too high")
    elseif TemperatureData > TemperatureStateHigh then
        print("Temperature is high")
    elseif TemperatureData > TemperatureStateLow then
        print("Temperature is normal")
    elseif TemperatureData > TemperatureStateTooLow then
        print("Temperature is low")
    else
        print("Temperature is too low")
    end
end

function WaterQualityCheck()
    if WaterQualityData == nil then
        print("Water Quality Unknown")
    elseif WaterQualityData == WaterQualityStateNormal then
        print("Water Quality is normal")
    else
        print("Water Quality is abnormal")
    end
end