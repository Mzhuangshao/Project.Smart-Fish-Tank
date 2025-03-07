    -- 引入OneWire和DallasTemperature库
local ow = require'onewire'
local ds = require'dallas'

local getTemperatureData = 0

-- 定义DS18B20传感器的引脚
local pin = 16 -- 对应D16针脚

-- local timer1 = tmr.create()

-- 初始化OneWire总线
ow.setup(pin)

-- 初始化DallasTemperature库
ds.setup(ow)

-- 查找并初始化所有连接的传感器
if ds.scan() then
    print("找到传感器:")
    for addr in ds.address_iter() do
        print(string.format("地址: %02X%02X%02X%02X%02X%02X%02X%02X", addr:byte(1,8)))
    end
else
    print("未找到传感器")
    return
end

-- 循环读取温度
while true do
    -- 请求传感器转换温度
    ds.start()
    -- 等待转换完成
    tmr.delay(ds.WAIT_TIME)
    -- 读取传感器地址
    for addr in ds.address_iter() do
        -- 读取温度
        local temp = ds.read(addr)
        if temp then
            print(string.format("传感器 %02X%02X%02X%02X%02X%02X%02X%02X 的温度是 %.2f°C", addr:byte(1,8), temp))
        else
            print(string.format("传感器 %02X%02X%02X%02X%02X%02X%02X%02X 读取失败", addr:byte(1,8)))
        end
    end
    -- 延迟一秒后再次读取
    tmr.delay(1000000)
end
