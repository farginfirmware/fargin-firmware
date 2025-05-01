
-- services tightly coupled to requestServers[] in main.c
service = {
    time = 0,
    led0 = 1,
}

-- led0 args
ledState = { off = 0, on = 1 }

function delayMilliseconds (milliseconds)
    service_request (service.time, milliseconds)    -- service_request() is defined in Lua.c
end

function led0_set (state)
    service_request (service.led0, state)
end


local function main()

    local blink = { msecPeriod = 1000, duty = 0.05 }

    while true do

        local period = blink.msecPeriod

        local milliseconds_ON  = period * blink.duty
        local milliseconds_OFF = period - milliseconds_ON

        led0_set (ledState.on)      delayMilliseconds (milliseconds_ON)
        led0_set (ledState.off)     delayMilliseconds (milliseconds_OFF)

    end

end

main()  -- execute the function defined above

