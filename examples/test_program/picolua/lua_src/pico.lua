--
-- This is a collection of helper functions for the Pico Lua environment.
-- These functions extend the `pico` library defined in the C code
-- This is part of the core picolua system and likely will remain unchanged between apps
--

if not pico then
	pico = {}
end

pico.wait = function(ms)
	ms = ms * 1000 -- clock is in microseconds
	local start = pico.clock()
	while pico.clock() - start < ms do
		coroutine.yield() -- Pauses the coroutine, allowing other coroutines or the main program to run.
	end
end

pico.micro_wait = function(ms)
	local start = pico.clock()
	while pico.clock() - start < ms do
		coroutine.yield() -- Pauses the coroutine, allowing other coroutines or the main program to run.
	end
end

pico.test = function()
	print("pico")
end
