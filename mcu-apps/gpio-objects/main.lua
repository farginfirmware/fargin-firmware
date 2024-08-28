
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
local off = 0
local  on = 1

local function led0_set (state)
    service_request (service.led0, state)
end

local function delayMilliseconds (milliseconds)
    service_request (service.time, milliseconds)
end


local GpioOutput = {
    new = function (self, port, bit)
        local pin
        pin = {}    -- create the instance
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


local GpioInput = {
    new = function (self, port, bit)
        local pin
        pin = {}    -- create the instance
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

    local milliseconds_ON
    local milliseconds_OFF

    local millisecondPeriod
    local dutyCycle

    local D0 = GpioInput :new (0, 16)   -- itsybitsy-m4 D0      tbd
    local D1 = GpioOutput:new (0, 17)   -- itsybitsy-m4 D1      tbd

    while true do

        -- default 1 Hz blink rate
        milliseconds_ON  = 100
        milliseconds_OFF = 900

        if D0:read() == 0 then
            -- input was pulled low
            -- 5 Hz blink rate
            milliseconds_ON  =  20
            milliseconds_OFF = 180
        end

        led0_set (on)       delayMilliseconds (milliseconds_ON)
        led0_set (off)      delayMilliseconds (milliseconds_OFF)

        millisecondPeriod = 250
        dutyCycle         =   0.5
        blink (D1, millisecondPeriod, dutyCycle)

    end

end


main()  -- execute the function defined above

