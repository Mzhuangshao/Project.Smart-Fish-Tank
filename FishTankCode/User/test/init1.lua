gpio.mode(1, gpio.OUTPUT)
gpio.mode(2, gpio.OUTPUT)
gpio.mode(4, gpio.OUTPUT)
for i = 1, 3, 1 do
    gpio.write(1, gpio.HIGH)
    gpio.write(2, gpio.HIGH)
    gpio.write(4, gpio.HIGH)
    tmr.delay(500000)
    gpio.write(1, gpio.LOW)
    gpio.write(2, gpio.LOW)
    gpio.write(4, gpio.LOW)
    tmr.delay(500000)
end
print("Success", "\n")

local timer1 = tmr.create()
local timer2 = tmr.create()

local getTempertureData = 0
-- local getWaterLevelData = 0
-- local getWaterQualityData = 0

function printDataLocal()
    print("--Start Print Data--")
    print("- DS18B20 Temperture Data: " .. getTempertureData)
    print("--End Print Data--")
end

-- function ESP8266Check()
--     print("ESP8266 reboot success", "\n")
-- end

-- timer1:alarm(500, tmr.ALARM_AUTO, ESP8266Check)
local T = tmr.create()
T:register(2000, 1, function()
    printDataLocal()
end)
T:start();


-- 引用 ds18b20 库
local ds = require("ds18b20")

do
    -- 初始化 DS18B20 传感器
    local pin = 5
    ds.setup(pin)

    -- 读取温度数据
    local temp = ds.getTemperature(pin)

    -- 检查是否成功读取
    if temp == nil then
        print("Failed to read temperature from DS18B20 sensor")
    else
        -- 将温度数据转换为 10 进制（实际上，DS18B20 库已经返回 10 进制的温度值）
        getTemperatureData = temp

        -- 打印温度数据
        print("Temperature is " .. getTemperatureData .. " degrees Celsius")
    end
end




-- -- 采集DS18B20传感器数据
-- do
--     local Filter_cnt = 0
--     local TempertureData_Flag = 1   -- 传感器状态，默认1，为1时传感器工作
--     TempertureDataPinPos = 5 ---- 定义检测针脚为5
--     gpio.mode(TempertureDataPinPos, gpio.INPUT, gpio.PULLUP)
--     function TempertureData_Check()
--         if gpio.read(TempertureDataPinPos) == 1 then --- 用于去除信号抖动
--             TempertureData_Flag = 0
--             Filter_cnt = 0
--         elseif TempertureData_Flag == 0 then
--             Filter_cnt = Filter_cnt + 1
--             if Filter_cnt > 50 then
--                 TempertureData_Flag = 1
--             end
--         end
--         if (TempertureData_Flag == 1) then
--             getTempertureData = getTempertureData + 1
--             TempertureData_Flag = 2
--         end
--     end

--     InTmr1 = tmr.create()
--     InTmr1:alarm(1, tmr.ALARM_AUTO, TempertureData_Check)
-- end


-- -- 采集水位传感器数据
-- do
--     local Filter_cnt = 0
--     local WaterLevelData_trigger = 1
--     WaterLevelDataPin = 6 ---- 定义检测针脚为6
--     gpio.mode(WaterLevelDataPin, gpio.INPUT, gpio.PULLUP)
--     function WaterLevelData_Check()
--         if gpio.read(WaterLevelDataPin) == 1 then --- 用于去除信号抖动
--             WaterLevelData_trigger = 0
--             Filter_cnt = 0
--         elseif WaterLevelData_trigger == 0 then
--             Filter_cnt = Filter_cnt + 1
--             if Filter_cnt > 100 then
--                 WaterLevelData_trigger = 1
--             end
--         end
--         if (WaterLevelData_trigger == 1) then
--             getWaterLevelData = getWaterLevelData + 1
--             WaterLevelData_trigger = 2
--         end
--     end

--     InTmr2 = tmr.create()
--     InTmr2:alarm(1, tmr.ALARM_AUTO, WaterLevelData_Check)
-- end


-- --采集水质监测传感器数据

-- do
--     local Filter_cnt = 0
--     local WaterQualityData_trigger = 1
--     WaterQualityDataPin = 7 ---- 定义检测针脚为7
--     gpio.mode(WaterQualityDataPin, gpio.INPUT, gpio.PULLUP)
--     function WaterQualityData_Check()
--         if gpio.read(WaterQualityDataPin) == 1 then --- 用于去除信号抖动
--             WaterQualityData_trigger = 0
--             Filter_cnt = 0
--         elseif WaterQualityData_trigger == 0 then
--             Filter_cnt = Filter_cnt + 1
--             if Filter_cnt > 100 then
--                 WaterQualityData_trigger = 1
--             end
--         end
--         if (WaterQualityData_trigger == 1) then
--             getWaterQualityData = getWaterQualityData + 1
--             WaterQualityData_trigger = 2
--         end
--     end

--     InTmr3 = tmr.create()
--     InTmr3:alarm(1, tmr.ALARM_AUTO, WaterQualityData_Check)
-- end
