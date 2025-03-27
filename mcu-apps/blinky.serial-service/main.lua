
-- this variable is intended to be modified via serial service,
-- so it must not declared local
blink = { auto = true, msecPeriod = 1000, duty = 0.1 }


-- services tightly coupled to requestServers[] in main.c
service = {
    time = 0,
    led0 = 1,
    btn0 = 2,
    gpio = 3,
    test = 4
}

-- led0 args
ledState = { off = 0, on = 1 }

function led0_set (state)
    service_request (service.led0, state)   -- service_request() is defined in Lua.c
end

function delayMilliseconds (milliseconds)
    service_request (service.time, milliseconds)
end

function buttonPressed()
    local serviceRequestResult  -- ignored
    local buttonState
    serviceRequestResult, buttonState = service_request (service.btn0)
    return buttonState == 0
end


local function main()

    local string1 = "string 1"
    local string2 = "STRING 2"
    local string3 = "hello  3"

    local count = 10

    while true do

        local period = blink.msecPeriod

        if buttonPressed() then
            period = period / 2     -- double the rate
        end

        local milliseconds_ON  = period * blink.duty
        local milliseconds_OFF = period - milliseconds_ON

        if blink.auto then
            led0_set (ledState.on)      delayMilliseconds (milliseconds_ON)
            led0_set (ledState.off)     delayMilliseconds (milliseconds_OFF)
        end

        count = count - 1
        local f

        if count == 0 then
            f = load ("blink.msecPeriod =  500")    -- returns a function
            f()                                     -- execute the function
        end

        if count <= -10 then
            count = 10
            f = load ("blink.msecPeriod = 1000")    -- returns a function
            f()                                     -- execute the function
        end


        local testSubcommand = 0
        local anythingButAString = 0
        _, string1, string2, string3 =
            service_request (service.test, testSubcommand,
                string1,
                anythingButAString,
                string2,
                string3)

--      delayMilliseconds (1000)

    end

end

main()  -- execute the function defined above

