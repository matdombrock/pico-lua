local max_ticks = 2048 -- When to loop / reset
pico.gpio_init(0)
function Main_loop(tick, delta)
	local tt = tick % max_ticks
	if tt == 1 then
		print("First tick")
		pico.gpio_set(0, true)
	end
	if tt == (max_ticks / 2) then
		pico.gpio_set(0, false)
	end
end
