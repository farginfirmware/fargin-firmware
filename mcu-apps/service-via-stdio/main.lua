
-- services tightly coupled to requestServers[] in main.c
local service = {
    time = 0,
    led0 = 1,
}

-- led0 args
local ledState = { off = 0, on = 1 }

local function led0_set (state)
    service_request (service.led0, state)   -- service_request() is defined in Lua.c
end

local function delayMilliseconds (milliseconds)
    service_request (service.time, milliseconds)
end


local function main()

    local periodMilliseconds = 10e3     -- 0.1 Hz
    local dutyCycle = 0.5               -- 50%
    local milliseconds_ON  = periodMilliseconds * dutyCycle
    local milliseconds_OFF = periodMilliseconds - milliseconds_ON

    while true do
        led0_set (ledState.on)      delayMilliseconds (milliseconds_ON)
        led0_set (ledState.off)     delayMilliseconds (milliseconds_OFF)
    end

end

main()  -- execute the function defined above

