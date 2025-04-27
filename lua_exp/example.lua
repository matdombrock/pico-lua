-- We only need to init GPIO once so do it outside of Main_loop
pico.gpio_init(0)

function Main_loop(tick, delta)
	print("Tick " .. tick)
	pico.wait(1000)
	pico.test()
	pico.wait(1000)
	pico.gpio_set(0, true)
	local delay = ((tick % 100) * 100)
	pico.wait(delay)
	pico.gpio_set(0, false)
	pico.wait(delay)
end
