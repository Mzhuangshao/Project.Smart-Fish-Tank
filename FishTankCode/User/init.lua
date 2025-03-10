print("8266 Success", "\n")
print("initializing...", "\n")
local getTemperatureData = require("ds18b20") -- 引用前置库ds18b20
local powerOLEDDisplay = require("oled")      -- 引用前置库oled
local OLEDDisplay = powerOLEDDisplay.init()   -- 初始化OLED显示屏
-- 定义全局变量
local WaterLevelData                          -- 获取水位
local WaterQualityData                        -- 获取水质
local TemperatureData                         -- 获取水温
local WaterLevelStateTooLow = 480             -- 定义水位：过低
local WaterLevelStateLow = 520                -- 定义水位：较低，处于Low与High之间是正常水位
local WaterLevelStateHigh = 560               -- 定义水位：较高，处于Low与High之间是正常水位
local WaterLevelStateTooHigh = 600            -- 定义水位：过高
local WaterQualityStateNormal = 1             -- 定义水质状态：正常
local TemperatureStateTooHigh = 22            -- 定义水温：过高
local TemperatureStateHigh = 18               -- 定义水温：较高，处于Low与High之间是正常水温
local TemperatureStateLow = 13                -- 定义水温：较低，处于Low与High之间是正常水温
local TemperatureStateTooLow = 10             -- 定义水温：过低
local debugMode = false                       -- 调试模式开关
print("initializing success", "\n")

-- ## 本地打印数据 #################### --

local function printDataLocal()                      -- 本地打印数据
    if debugMode == true then                        -- 调试模式
        print(" ========= Debug mode ========= ")
        print("Water Level: " .. WaterLevelData)     -- 打印水位数据
        --print("Temperature: " .. TemperatureData .. "°C") -- 打印DS18B20传感器数据
        print("Water Quality: " .. WaterQualityData) -- 打印水质数据
    else                                             -- 正常输出模式
        print("--- Start Print Data ---")
        WaterLevelCheck()                            -- 调用水位检查函数
        TemperatureCheck()                           -- 调用水温检查函数
        WaterQualityCheck()                          -- 调用水质检查函数
    end
end

local TimerCallPrintDataLocal = tmr.create()
TimerCallPrintDataLocal:register(2000, 1,
    function()
        printDataLocal()
    end)
TimerCallPrintDataLocal:start()

-- ## 数据获取 ##################### --

-- 获取水位
do
    local getWaterLevelPin = 0                      -- 定义获取水位数据的ADC channel -- A0
    local function getWaterLevel()
        WaterLevelData = adc.read(getWaterLevelPin) -- 返回ADC读取的水位数据
    end

    TimerCallbackGetWaterLevel = tmr.create()
    TimerCallbackGetWaterLevel:alarm(1000, tmr.ALARM_AUTO, getWaterLevel) -- 定时1秒读取水位数据
end

-- 获取水温
do
    local getTemperatureDataPin = 3 -- 定义获取水温数据的GPIO口 -- D3 引脚连接DS18B20
    local function getTemperature()
        getTemperatureData:read_temp(TemperatureDataReadout, getTemperatureDataPin, getTemperatureData.C)
    end

    TimerCallbackGetTemperature = tmr.create()
    TimerCallbackGetTemperature:alarm(1000, tmr.ALARM_AUTO, getTemperature) -- 定时1秒读取水位数据
end

function TemperatureDataReadout(temp)                                              -- 定义DS18B20传感器数据读取回调函数
    if debugMode == true then
        if getTemperatureData.sens then                                            -- 如果DS18B20传感器存在，则打印传感器信息
            print("Total number of DS18B20 sensors: " .. #getTemperatureData.sens) -- 打印DS18B20传感器数量
            for i, s in ipairs(getTemperatureData.sens) do                         -- 打印DS18B20传感器信息
                print(string.format("  sensor #%d address: %s%s", i,
                    ('%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X'):format(s:byte(1, 8)),
                    s:byte(9) == 1 and " (parasite)" or ""))
            end
        end
        for addr, temp in pairs(temp) do
            print(string.format("Sensor[%s] Temp: %s °C",
                ('%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X'):format(addr:byte(1, 8)), temp))
        end
    else
        for _, temp in pairs(temp) do                                   -- 遍历temp数组，寻找水温值
            if temp then                                                -- 如果检测到水温值
                TemperatureData = tonumber(string.format("%.2f", temp)) -- 保留两位小数并写入TemperatureData变量
            end
        end
    end
end

-- 获取水质
do
    local getWaterQualityPin = 2                           -- 定义获取水质数据的GPIO引脚 -- D2
    gpio.mode(getWaterQualityPin, gpio.INPUT, gpio.PULLUP) -- 设置GPIO为输入模式，并使能上拉电阻
    local function getWaterQuality()
        WaterQualityData = gpio.read(getWaterQualityPin)   -- 返回GPIO读取的水质数据
    end

    TimerCallbackGetWaterQuality = tmr.create()
    TimerCallbackGetWaterQuality:alarm(1000, tmr.ALARM_AUTO, getWaterQuality) -- 定时1秒读取水质数据
end

-- ## 数据检查 ### --

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
    if TemperatureData == nil then                        -- 如果没有读取到DS18B20传感器数据，则打印“Temperature Unknown”
        print("Temperature Unknown")
    elseif TemperatureData > TemperatureStateTooHigh then -- 水温过高
        print("Temperature is too high")
    elseif TemperatureData > TemperatureStateHigh then    -- 水温较高
        print("Temperature is high")
    elseif TemperatureData > TemperatureStateLow then     -- 水温正常
        print("Temperature is normal")
    elseif TemperatureData > TemperatureStateTooLow then  -- 水温较低
        print("Temperature is low")
    else                                                  -- 水温过低
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
