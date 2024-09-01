
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
    service_request (service.led0, state)   -- service_request() is defined in Lua.c
end

local function delayMilliseconds (milliseconds)
    service_request (service.time, milliseconds)
end

local function buttonPressed()
    local serviceRequestResult  -- ignored
    local buttonState
    serviceRequestResult, buttonState = service_request (service.btn0)
    return buttonState == 0
end


-- "class" definition for a gpio output
local GpioOutput = {
    new = function (self, port, bit)
        local pin = {}    -- create the instance
        setmetatable (pin, self)
        self.__index = self
        _, pin.handle = service_request (service.gpio, gpio.getHandle, port, bit)
                        service_request (service.gpio, gpio.configure, pin.handle, gpio.configureArgs.output.pushPull)
        return pin
    end,

    write = function (self, state)
        service_request (service.gpio, gpio.write, self.handle, state)
    end
}


-- "class" definition for a gpio input
local GpioInput = {
    new = function (self, port, bit)
        local pin = {}    -- create the instance
        setmetatable (pin, self)
        self.__index = self
        _, pin.handle = service_request (service.gpio, gpio.getHandle, port, bit)
                        service_request (service.gpio, gpio.configure, pin.handle, gpio.configureArgs.input.pullUp)
        return pin
    end,

    read = function (self)
        local pin
        _, pin = service_request (service.gpio, gpio.read, self.handle)
        return pin
    end
}


local function blink (gpioPin, millisecondPeriod, dutyCycle)
    local milliseconds_ON  = millisecondPeriod * dutyCycle
    local milliseconds_OFF = millisecondPeriod - milliseconds_ON
    gpioPin:write (0)     delayMilliseconds (milliseconds_ON)
    gpioPin:write (1)     delayMilliseconds (milliseconds_OFF)
end


local function main()

    local port, bit

    port = 0   bit = 25     -- makerdiary-nrf52840-mdk-iot-kit
    local testInput = GpioInput:new (port, bit)

    port = 0   bit = 24     -- makerdiary-nrf52840-mdk-iot-kit blue LED
    local blueLED = GpioOutput:new (port, bit)
    blueLED:write (1)       -- initialize off

    while true do

        if testInput:read() == 1 then
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

