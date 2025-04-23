function Main_loop(tick)
	print("Tick " .. tick)
	pico.led(true)
	local delay = ((tick % 100) * 100)
	tools.wait(delay)
	pico.led(false)
	tools.wait(delay)
end
