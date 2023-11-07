
local test = 0
local time = 1
local uart = 2
local  i2c = 3

local function delayMilliseconds (milliseconds)
    service_request (time, milliseconds)
end


local function main()

    local testString_1
    local testString_2 = "this really is Lua "
    local testString_3 = "speaking"

    local count = 0

    while true do
        delayMilliseconds ( 500)
        count = count + 1
        testString_1 = "\r\n" .. tostring (count) .. " "
        _, testString_1, testString_2, testString_3 =
            service_request (uart, 0, testString_1, testString_2, testString_3)
    end
end 


main()

