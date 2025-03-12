sda = 5
scl = 6
function init_OLED(sda, scl)
    sla = 0x3c
    i2c.setup(0, sda, scl, i2c.SLOW)
    disp = u8g2.ssd1306_i2c_128x64_noname(0, sla)
    disp:setFont(u8g2.font_6x10_tf)
    disp:setFontPosTop()
end

function print_OLED()
--    disp:drawFrame(0, 0, 128, 16)
    disp:setFont(u8g2.font_6x10_tf)
    disp:drawStr(30, 5, "Hello Mcu!!")
    disp:drawStr(30, 20, "CSDNzwh")
    disp:drawStr(80, 40, str1)
    --disp:drawFrame(0, 16, 128, 45)
    disp:sendBuffer()
end

str1 = "by:zwh"
init_OLED(sda, scl)
T = tmr.create()
T:alarm(1000, tmr.ALARM_AUTO, function()
    print_OLED()
end)
