print("8266 Success", "\n")
print("initializing...", "\n")
local getTemperatureData = require("ds18b20") -- 引用前置库ds18b20
local WaterLevelData                          -- 获取水位
local WaterQualityData                        -- 获取水质
local TemperatureData                         -- 获取温度
local Device_DS18B20                          -- 定义DS18B20传感器
local Device_Water_Sensor                     -- 定义水位传感器
local Device_Water_Quality                    -- 定义水质传感器
local debugMode = false                       -- 调试模式开关
print("initializing success", "\n")

-- ########################### --

print("Checking Device...", "\n")
do
    if getTemperatureData.sens then
        print("Total number of DS18B20 sensors: " .. #getTemperatureData.sens)
    end
end
print("Checking Device success", "\n")

-- ########################### --

function printDataLocal()                       -- 本地打印数据
    if debugMode == true then
        print(" ===== Debug mode is on ===== ") -- 调试模式
        function TemperatureDataReadoutInDebugMode(temp)
            if getTemperatureData.sens then
                print("Total number of DS18B20 sensors: " .. #getTemperatureData.sens)
                for i, s in ipairs(getTemperatureData.sens) do
                    print(
                        string.format(
                            "  sensor #%d address: %s%s",
                            i,
                            ("%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X"):format(s:byte(1, 8)),
                            s:byte(9) == 1 and " (parasite)" or ""
                        )
                    )
                end
            end
            for addr, temp in pairs(temp) do
                print(
                    string.format(
                        "Sensor[%s] Temp: %s °C",
                        ("%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X"):format(addr:byte(1, 8)),
                        temp
                    )
                )
            end
        end
    else -- 正常输出模式
        -- print("--Start Print Data--")
        print("Water Level: " .. WaterLevelData, "\n")
        print("Temperature: %s °C" .. TemperatureData, "\n")
        print("Water Quality: " .. WaterQualityData, "\n")
        -- print("--End Print Data--")
    end
end

-- ########################### --

local T = tmr.create()
T:register(2000, 1,
    function()
        printDataLocal()
    end
)
T:start()

-- ########################### --

-- 获取水位
do
    local getWaterLevelPin = 0                  -- 定义获取水位数据的ADC channel -- A0
    WaterLevelData = adc.read(getWaterLevelPin) -- 返回ADC读取的水位数据
end

-- 获取温度
do
    local getTemperatureDataPin = 3 -- 定义获取温度数据的GPIO口 -- D3 引脚连接DS18B20
    getTemperatureData:read_temp(TemperatureDataReadout, getTemperatureDataPin, getTemperatureData.C)
    function TemperatureDataReadout(temp)
        if getTemperatureData.sens then
            for temp in pairs(temp) do
                TemperatureData = (string.format("%.2f", temp))
            end
        end
    end
end

-- 获取水质的函数
do
    local getWaterQualityPin = 0                     -- 定义获取水质数据的ADC channel
    gpio.mode(getWaterQualityPin, gpio.INPUT, gpio.PULLUP)  -- 设置GPIO为输入模式，并使能上拉电阻
    WaterQualityData = gpio.read(getWaterQualityPin) -- 返回GPIO读取的水质数据
end