
-- gpio args (see ff.gpio.c)
gpio = {
    getHandle = 0,
    configure = 1,
         read = 2,
        write = 3,
    configureArgs = {
            input = { noPull=0, pullUp=1, pullDown=2 },
           output = { pushPull=3, openDrain=4, openDrainPullupEnabled=5 }
    }
}


-- "class" definition for a gpio output
GpioOutput = {
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
GpioInput = {
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

