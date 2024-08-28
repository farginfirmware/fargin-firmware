
-- services tightly coupled to requestServers[] in main.c
local time = 0
local led0 = 1
local btn0 = 2

-- led0 args
local off = 0
local  on = 1


local function delayMilliseconds (milliseconds)
    service_request (time, milliseconds)    -- service_request() is defined in Lua.c
end

local function led0_set (state)
    service_request (led0, state)
end

local function buttonPressed()
    local serviceRequestResult  -- ignored
    local buttonState
    serviceRequestResult, buttonState = service_request (btn0)
    return buttonState == 0
end


local function main()

    local milliseconds_ON
    local milliseconds_OFF

    while true do

        if buttonPressed() then
            -- 5 Hz blink
            milliseconds_ON  =  25
            milliseconds_OFF = 175
        else
            -- 1 Hz blink
            milliseconds_ON  = 100
            milliseconds_OFF = 900
        end

        led0_set (on)   delayMilliseconds (milliseconds_ON)
        led0_set (off)  delayMilliseconds (milliseconds_OFF)

    end

end


main()  -- execute the function defined above

