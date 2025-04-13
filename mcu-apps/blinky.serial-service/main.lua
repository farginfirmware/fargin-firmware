
-- this variable is intended to be modified via serial service,
-- so it must not declared local
blink = { auto = true, msecPeriod = 1000, duty = 0.1 }


-- services tightly coupled to requestServers[] in main.c
service = {
    time = 0,
    led0 = 1,
    test = 2
}

-- led0 args
ledState = { off = 0, on = 1 }

-- test args (tightly coupled to defines in test.c)
testType = { readSerialRxString = 2 }


function delayMilliseconds (milliseconds)
    service_request (service.time, milliseconds)    -- service_request() is defined in Lua.c
end

function led0_set (state)
    service_request (service.led0, state)
end

function test_getRxString ()
    _, rxString = service_request (service.test, testType.readSerialRxString)
    return rxString
    -- tbd
end


local function main()

    while true do

        if blink.auto then

            local period = blink.msecPeriod

            local milliseconds_ON  = period * blink.duty
            local milliseconds_OFF = period - milliseconds_ON

            led0_set (ledState.on)      delayMilliseconds (milliseconds_ON)
            led0_set (ledState.off)     delayMilliseconds (milliseconds_OFF)

        else

            delayMilliseconds (1000)

            -- read and execute script
            local rx = test_getRxString ()
            if rx then
                local fn = load (rx)    -- load() turns rx into a function
                fn()                    -- execute the function
            end


        end

    end

end

main()  -- execute the function defined above

