
-- services tightly coupled to requestServers[] in main.c
local service = {
    time = 0,
    led0 = 1,
    btn0 = 2
}

-- led0 args
local ledState = { off = 0, on = 1 }


local function delayMilliseconds (milliseconds)
    service_request (service.time, milliseconds)    -- service_request() is defined in Lua.c
end

local function led0_set (state)
    service_request (service.led0, state)
end

local function buttonPressed()
    local serviceRequestResult  -- ignored
    local buttonState
    serviceRequestResult, buttonState = service_request (service.btn0)
    return buttonState == 0
end


local function main()

    local milliseconds_ON
    local milliseconds_OFF

    while true do

        -- default 1 Hz blink rate
        milliseconds_ON  = 100
        milliseconds_OFF = 900

        if buttonPressed() then
            -- 2 Hz blink
            milliseconds_ON  =  50
            milliseconds_OFF = 450
        end

        led0_set (ledState.on)      delayMilliseconds (milliseconds_ON)
        led0_set (ledState.off)     delayMilliseconds (milliseconds_OFF)

    end

end

main()  -- execute the function defined above

