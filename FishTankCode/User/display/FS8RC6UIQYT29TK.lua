-- oledscroll
-- Written by John Longworth July 2016   

function initDisplay()
   local sda = 4 -- GPIO2
   local scl = 3 -- GPIO0
   local sla = 0x3c
   i2c.setup(0, sda, scl, i2c.SLOW)
   disp = u8g.ssd1306_128x64_i2c(sla)
   disp:setFont(u8g.font_6x10)   
   disp:setColorIndex(1)   
   --disp:setRot180() 
end

function moveit()
   disp:firstPage()
   repeat             
      disp:drawStr(count, count, "Hello World") 
      if count > 70 then 
         count = -5 
      end
   until disp:nextPage() == false         
end

initDisplay()
count = -5
tmr.alarm(0,250,1,function()     
   moveit()
   count = count + 1  
end)
    
 