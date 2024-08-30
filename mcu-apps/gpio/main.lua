
-- services tightly coupled to requestServers[] in main.c
local service = {
    time = 0,
    led0 = 1,
    btn0 = 2,
    gpio = 3
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
    gpio_write (gpioHandle, 1)      delayMilliseconds (milliseconds_ON)
    gpio_write (gpioHandle, 0)      delayMilliseconds (milliseconds_OFF)
end



local function main()

    local milliseconds_ON
    local milliseconds_OFF

    local millisecondPeriod
    local dutyCycle

    local D0 = gpio_configureInput  (0, 16)   -- itsybitsy-m4 D0      tbd
    local D1 = gpio_configureOutput (0, 17)   -- itsybitsy-m4 D1      tbd

    while true do

        -- default 1 Hz blink rate
        milliseconds_ON  = 100
        milliseconds_OFF = 900

        if buttonPressed() then
            -- 2 Hz blink
            milliseconds_ON  =  50
            milliseconds_OFF = 450
        end

        -- test gpio input
        if gpio_read (D0) == 0 then
            -- input was pulled low
            -- 5 Hz blink rate
            milliseconds_ON  =  20
            milliseconds_OFF = 180
        end

        led0_set (ledState.on)      delayMilliseconds (milliseconds_ON)
        led0_set (ledState.off)     delayMilliseconds (milliseconds_OFF)

        -- test gpio output
        millisecondPeriod = 250
        dutyCycle         =   0.5
        blink (D1, millisecondPeriod, dutyCycle)

    end

end


main()  -- execute the function defined above

