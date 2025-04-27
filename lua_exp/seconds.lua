-- Blink an LED every second using a delta accumulator
local delta_acc = 0
local on = false
local second = 1000000 -- 1 second in microseconds
pico.gpio_init(0)
function Main_loop(tick, delta)
	delta_acc = delta_acc + delta
	if delta_acc >= second then
		print("1 second has passed")
		delta_acc = delta_acc - second
		pico.gpio_set(0, on) -- Set GPIO pin 0 to high
		on = not on
	end
end
