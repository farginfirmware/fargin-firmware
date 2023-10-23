
function test (interval, count, message)
    n = 0
    while n < count do
        n = n + 1
        delayMinutes (interval)
        transmit (message .. n)
    end
end

test (60, 48, "xyz ")

