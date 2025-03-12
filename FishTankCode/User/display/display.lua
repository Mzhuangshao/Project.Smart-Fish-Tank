local sda_pin = 2
local scl_pin = 1
i2c.setup(0, sda_pin, scl_pin, i2c.SLOW)
local lib_ssd1306 = require("ssd1306")
local lib_fn = require("pixeloperator") -- or "terminus16"
local lib_fb = require("framebuffer")
collectgarbage()

lib_ssd1306.init(128, 64) -- assuming that a 128x64 OLED is connected
lib_ssd1306.contrast(255) -- maximum contrast
lib_fb.init(128, 64)      -- initialize framebuffer for 128x64 pixels
lib_fb.print(lib_fn, "Hello from NodeMCU!\nHello yes, this is Lua\n")
lib_ssd1306.show(lib_fb.buf)
