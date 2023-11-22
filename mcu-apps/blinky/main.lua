
-- services tightly coupled to nextLevelProcessor[] in service.c
local time = 0
local led0 = 1

-- led0 args
local  off = 0
local   on = 1


local function delayMilliseconds (milliseconds)
    service_request (time, milliseconds)    -- service_request() is defined in Lua.c
end

local function led0_set (state)
    service_request (led0, state)
end


local function main()

    local milliseconds_ON  =  100
    local milliseconds_OFF = 1900

    while true do
        led0_set (on)   delayMilliseconds (milliseconds_ON)
        led0_set (off)  delayMilliseconds (milliseconds_OFF)
    end

end


main()  -- execute the function defined above

