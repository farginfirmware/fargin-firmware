
-- services available to Lua (see service_request() in Lua.c)
local service = {
    -- these defs are tightly coupled to nextLevelProcessor[] in service.c
    time = 0,
    led0 = 1,
    gpio = 2
}

-- led0 args
local ledState = { off = 0, on = 1 }


-- gpio args (see gpio.c)
local gpio = {
    getHandle = 0,
    configure = 1,
         read = 2,
        write = 3,
    configureArgs = {   -- note: pin may already be configured
            input = { noPull=0, pullUp=1, pullDown=2 },
           output = { pushPull=3, openDrain=4, openDrainPullupEnabled=5 }
        }
}


local blueLED
local button

local function blueLED_set (state)
    service_request (service.gpio, gpio.write, blueLED, state)
end

local function buttonPressed()
    local buttonState
    _, buttonState = service_request (service.gpio, gpio.read, button)
    return buttonState == 0
end

local function led0_set (state)
    service_request (service.led0, state)
end

local function delayMilliseconds (milliseconds)
    service_request (service.time, milliseconds)
end


local function main()

    local port, bit
    local result

    port =  0
    bit  = 31
    result, blueLED = service_request (service.gpio, gpio.getHandle, port, bit)
                      service_request (service.gpio, gpio.configure, blueLED, gpio.configureArgs.output.pushPull)

    port =  0
    bit  = 19
    result, button = service_request (service.gpio, gpio.getHandle, port, bit)
                     service_request (service.gpio, gpio.configure, button, gpio.configureArgs.input.pullUp)


    local milliseconds_ON  =  100
    local milliseconds_OFF = 1900

    while true do

        led0_set (ledState.on)
        delayMilliseconds (milliseconds_ON)

        -- keep led0 on as long as button is pressed
        while buttonPressed() do
            delayMilliseconds (100)
        end

        led0_set (ledState.off)

        blueLED_set (0)             -- on
        delayMilliseconds (milliseconds_OFF)

        blueLED_set (1)             -- off
        delayMilliseconds (1000)

    end

end


main()  -- execute the function defined above

