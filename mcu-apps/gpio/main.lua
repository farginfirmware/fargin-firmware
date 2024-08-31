
-- services tightly coupled to requestServers[] in main.c
local service = {
    time = 0,
    led0 = 1,
    btn0 = 2,
    gpio = 3
}

local gpio = {
    getHandle = 0,
    configure = 1,
         read = 2,
        write = 3,
    configureArgs = {
            input = { noPull=0, pullUp=1, pullDown=2 },
           output = { pushPull=3, openDrain=4, openDrainPullupEnabled=5 }
    }
}

-- led0 args
local ledState = { off = 0, on = 1 }


local function led0_set (state)
    service_request (service.led0, state)
end

local function delayMilliseconds (milliseconds)
    service_request (service.time, milliseconds)    -- service_request() is defined in Lua.c
end

local function buttonPressed()
    local serviceRequestResult  -- ignored
    local buttonState
    serviceRequestResult, buttonState = service_request (service.btn0)
    return buttonState == 0
end


local function gpio_configureInput (port, bit)
    -- configure the given port/bit and return the gpio handle
    local serviceRequestResult  -- ignored
    local handle
    serviceRequestResult, handle = service_request (service.gpio, gpio.getHandle, port, bit)
    service_request (service.gpio, gpio.configure, handle, gpio.configureArgs.input.pullUp)
    return handle
end

local function gpio_configureOutput (port, bit)
    -- configure the given port/bit and return the gpio handle
    local serviceRequestResult  -- ignored
    local handle
    serviceRequestResult, handle = service_request (service.gpio, gpio.getHandle, port, bit)
    service_request (service.gpio, gpio.configure, handle, gpio.configureArgs.output.pushPull)
    return handle
end

local function gpio_read (gpioHandle)
    local serviceRequestResult  -- ignored
    local pin
    serviceRequestResult, pin = service_request (service.gpio, gpio.read, gpioHandle)
    return pin
end

local function gpio_write (gpioHandle, state)
    service_request (service.gpio, gpio.write, gpioHandle, state)
end

local function blink (gpioHandle, millisecondPeriod, dutyCycle)
    local milliseconds_ON  = millisecondPeriod * dutyCycle
    local milliseconds_OFF = millisecondPeriod - milliseconds_ON
    gpio_write (gpioHandle, 0)      delayMilliseconds (milliseconds_ON)
    gpio_write (gpioHandle, 1)      delayMilliseconds (milliseconds_OFF)
end



local function main()

    local port, bit

    port = 0   bit = 25     -- makerdiary-nrf52840-m2-kit
    local testInput = gpio_configureInput  (port, bit)

    port = 0   bit = 24     -- makerdiary-nrf52840-m2-kit blue LED
    local blueLED = gpio_configureOutput (port, bit)
    gpio_write (gpioHandle, 1)  -- init high (off)

    while true do

        if gpio_read (testInput) == 1 then
            -- test input is not pulled low

            -- default 1 Hz blink rate
            local milliseconds_ON  = 100
            local milliseconds_OFF = 900

            if buttonPressed() then
                -- 2 Hz blink
                milliseconds_ON  =  50
                milliseconds_OFF = 450
            end

            led0_set (ledState.on)      delayMilliseconds (milliseconds_ON)
            led0_set (ledState.off)     delayMilliseconds (milliseconds_OFF)

        else
            -- test input is pulled low

            local millisecondPeriod = 500
            local dutyCycle         =   0.1

            -- test gpio output
            blink (blueLED, millisecondPeriod, dutyCycle)

        end

    end

end


main()  -- execute the function defined above

