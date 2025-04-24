--
-- This is a collection of helper functions for the Pico Lua environment.
-- These extend the functions exposed in the C code
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
