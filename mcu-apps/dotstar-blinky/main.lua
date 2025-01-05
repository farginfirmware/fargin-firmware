
-- services tightly coupled to requestServers[] in main.c
local service = {
    time    = 0,
    led0    = 1,
    btn0    = 2,
    dotstar = 3
}

-- led0 args
local ledState = { off = 0, on = 1 }

-- dotstar args
local dotstarSetting = { hue = 0, saturation = 1, brightness = 2 }


local function delayMilliseconds (milliseconds)
    service_request (service.time, milliseconds)
end

local function led0_set (state)
    service_request (service.led0, state)   -- service_request() is defined in Lua.c
end

local function buttonPressed()
    local serviceRequestResult  -- ignored
    local buttonState
    serviceRequestResult, buttonState = service_request (service.btn0)
    return buttonState == 0
end


local function dotstar_setHue (index, hue)
    -- 0 <= hue < 360 (0 is red, 120 is green, 240 is blue)
    -- index is 0-based
    service_request (service.dotstar, dotstarSetting.hue, index, hue)
end

local function dotstar_setSaturation (index, saturation)
    -- saturation ranges from 0.0 to 1.0
    -- index is 0-based
    service_request (service.dotstar, dotstarSetting.saturation, index, saturation)
end

local function dotstar_setBrightness (index, brightness)
    -- brightness ranges from 0.0 to 1.0
    -- index is 0-based
    service_request (service.dotstar, dotstarSetting.brightness, index, brightness)
end


local function main()

    local dotstar = { index = 0, brightness = 0.10, saturation = 1.00 }
    local hue = 0

    dotstar_setBrightness (dotstar.index, dotstar.brightness)
    dotstar_setSaturation (dotstar.index, dotstar.saturation)

    while true do
        hue = (hue + 1) % 360
        dotstar_setHue (dotstar.index, hue)
        delayMilliseconds (100)
    end

    while true do

        local period = 1000         -- default 1 Hz blink rate
        if buttonPressed() then
            period = period / 2     -- double the rate
        end

        local milliseconds_ON  = period * 0.20      -- 20% duty cycle
        local milliseconds_OFF = period - milliseconds_ON

        led0_set (ledState.on)      delayMilliseconds (milliseconds_ON)
        led0_set (ledState.off)     delayMilliseconds (milliseconds_OFF)

    end

end

main()  -- execute the function defined above

