print("8266 Success", "\n")
print("initializing...", "\n")
local lib_ds18b20 = require("ds18b20")   -- 引用前置库ds18b20
-- 定义全局变量
local WaterLevelData               -- 获取水位
local WaterQualityData             -- 获取水质
local TemperatureData              -- 获取水温
-- local pHData                       -- 获取PH值
local WaterLevelStateTooLow = 480  -- 定义水位：过低
local WaterLevelStateLow = 520     -- 定义水位：较低，处于Low与High之间是正常水位
local WaterLevelStateHigh = 560    -- 定义水位：较高，处于Low与High之间是正常水位
local WaterLevelStateTooHigh = 600 -- 定义水位：过高
local WaterQualityStateNormal = 1  -- 定义水质状态：正常
local TemperatureStateTooHigh = 22 -- 定义水温：过高
local TemperatureStateHigh = 18    -- 定义水温：较高，处于Low与High之间是正常水温
local TemperatureStateLow = 13     -- 定义水温：较低，处于Low与High之间是正常水温
local TemperatureStateTooLow = 10  -- 定义水温：过低
-- local pHLevel = 7                  -- 定义PH值：正常水质
local OLED_SDA_Pin = 5                   -- 定义OLED的SDA引脚
local OLED_SCL_Pin = 6                   -- 定义OLED的SCL引脚
local getWaterLevelPin = 0
local getWaterQualityPin = 7
local getTemperatureDataPin = 9 -- 定义获取水温数据的GPIO口 -- D8 引脚连接DS18B20
-- local WaterLevelMessage = ""
-- local TemperatureMessage = ""
-- local WaterQualityMessage = ""
local debugMode = false -- 调试模式开关
print("initializing success", "\n")

local function init_OLED(OLED_SDA_Pin, OLED_SCL_Pin)
    local sla = 0x3c
    i2c.setup(0, OLED_SDA_Pin, OLED_SCL_Pin, i2c.SLOW)
    disp = u8g2.ssd1306_i2c_128x64_noname(0, sla)
    disp:setFont(u8g2.font_unifont_t_symbols)
    disp:setFontPosTop()
end

-- ## 数据获取 ##################### --

function getTemperatureData(temp)                                       -- 定义DS18B20传感器数据读取回调函数
    for _, temp in pairs(temp) do                                       -- 遍历temp数组，寻找水温值
        if temp then                                                    -- 如果检测到水温值
            TemperatureData = tonumber(string.format("%.2f", temp))     -- 保留两位小数并写入TemperatureData变量
        end
    end
end

do
    gpio.mode(getWaterQualityPin, gpio.INPUT, gpio.PULLUP) -- 设置GPIO为输入模式，并使能上拉电阻

    local function GetData()
        WaterLevelData = adc.read(getWaterLevelPin)      -- 返回ADC读取的水位数据
        WaterQualityData = gpio.read(getWaterQualityPin) -- 返回GPIO读取的水质数据
        lib_ds18b20:read_temp(getTemperatureData, getTemperatureDataPin, lib_ds18b20.C)
    end

    TimerCallbackGetData = tmr.create()
    TimerCallbackGetData:alarm(1000, tmr.ALARM_AUTO, GetData)
end

-- ## 数据检查 ### --

function WaterLevelCheck()
    if WaterLevelData == nil then
        WaterLevelMessage = "!! Unknown"
    elseif WaterLevelData > WaterLevelStateTooHigh then
        WaterLevelMessage = "too high"
    elseif WaterLevelData > WaterLevelStateHigh then
        WaterLevelMessage = "high"
    elseif WaterLevelData > WaterLevelStateLow then
        WaterLevelMessage = "normal"
    elseif WaterLevelData > WaterLevelStateTooLow then
        WaterLevelMessage = "low"
    else
        WaterLevelMessage = "too low"
    end
end

function TemperatureCheck()
    if TemperatureData == nil then                        -- 如果没有读取到DS18B20传感器数据，则打印“Temperature Unknown”
        TemperatureMessage = "!! Unknown"
    elseif TemperatureData > TemperatureStateTooHigh then -- 水温过高
        TemperatureMessage = "too high"
    elseif TemperatureData > TemperatureStateHigh then    -- 水温较高
        TemperatureMessage = "high"
    elseif TemperatureData > TemperatureStateLow then     -- 水温正常
        TemperatureMessage = "normal"
    elseif TemperatureData > TemperatureStateTooLow then  -- 水温较低
        TemperatureMessage = "low"
    else                                                  -- 水温过低
        TemperatureMessage = "too low"
    end
end

function WaterQualityCheck()
    if WaterQualityData == nil then
        WaterQualityMessage = "!! Unknown"
    elseif WaterQualityData == WaterQualityStateNormal then
        WaterQualityMessage = "normal"
    else
        WaterQualityMessage = "abnormal"
    end
end

-- ###################### --

local function printDataLocal() -- 本地打印数据
    -- if debugMode == true then                        -- 调试模式
    --     print(" ========= Debug mode ========= ")
    --     print("Water Level: " .. WaterLevelData)     -- 打印水位数据
    --     --print("Temperature: " .. TemperatureData .. "°C") -- 打印DS18B20传感器数据
    --     print("Water Quality: " .. WaterQualityData) -- 打印水质数据
    --     -- print("PH: " .. pHData)                      -- 打印PH数据
    -- end
    WaterLevelCheck()
    TemperatureCheck()
    WaterQualityCheck()
    disp:setFont(u8g2.font_6x10_tf)
    --disp:drawBox(0, 0, 128, 50)
    disp:drawStr(5, 5, "WaterLevel: "..WaterLevelMessage)
    disp:drawStr(5, 20, "Temperature: "..TemperatureMessage)
    disp:drawStr(5, 35, "WaterQuality: "..WaterQualityMessage)
    disp:drawFrame(0, 0, 128, 50)
    disp:sendBuffer()
end


TimerCallPrintOnOLED = tmr.create()
TimerCallPrintOnOLED:alarm(10, tmr.ALARM_AUTO, function()
    init_OLED(OLED_SDA_Pin, OLED_SCL_Pin)
    printDataLocal()
end)

-- TimerCallPrintDataLocal = tmr.create()
-- TimerCallPrintDataLocal:register(1000, 1,
--     function()
--         printDataLocal()
--     end)
-- TimerCallPrintDataLocal:start()
