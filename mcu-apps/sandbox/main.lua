
-- NOTE!! these are tightly coupled to requestServers[] in main.c
local test = 0
local time = 1
local gpio = 2
local  spi = 3
local  i2c = 4
local uart = 5
local file = 6

local   red_LED = 0
local green_LED = 1
local  blue_LED = 2

local off = 0
local  on = 1


--[[

--]]


local function led_set (color, state)
    service_request (gpio, color, state)
end


local function delayMilliseconds (milliseconds)
    service_request (time, milliseconds)
end


local function blink (color, millisecondPeriod, dutyCycle)

    local milliseconds_ON  = millisecondPeriod * dutyCycle
    local milliseconds_OFF = millisecondPeriod - milliseconds_ON

    while true do

        coroutine.yield ()

        led_set (color, on)
        delayMilliseconds (milliseconds_ON)

        led_set (color, off)
        delayMilliseconds (milliseconds_OFF)

    end
end


local function main()

    local blink_GREEN = coroutine.create (blink)
    local blink_RED   = coroutine.create (blink)

    local greenPeriod, greenDuty
    local   redPeriod,   redDuty

    _, greenPeriod = service_request (test, 0)
    _, greenDuty   = service_request (test, 1)

    _,   redPeriod = service_request (test, 2)
    _,   redDuty   = service_request (test, 3)

    coroutine.resume (blink_GREEN, green_LED, greenPeriod, greenDuty)
    coroutine.resume (blink_RED,     red_LED,   redPeriod,   redDuty)

    local testString_1 = "this really is Lua "
    local testString_2 = "speaking"
    local testString_3 = "\r\n"

    while true do
        coroutine.resume (blink_GREEN)
        coroutine.resume (blink_RED)

        _, testString_1, testString_2, testString_3 =
            service_request (uart, 0, testString_1, testString_2, testString_3)
    end
end 


-- print ("Hello world, this is lua!")

main()


