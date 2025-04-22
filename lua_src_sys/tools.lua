--
-- Define a custom library of helper functions that is automatically made available to the main lua script
--
tools = {}
-- Return a value at the nth position of the fibonacci sequence
tools.fibonacci = function(n)
	if n <= 1 then
		return n
	else
		return tools.fibonacci(n - 1) + tools.fibonacci(n - 2)
	end
end

tools.sin_norm = function(x)
	return 1 + math.sin(x) * 0.5
end

tools.wait = function(ms)
	ms = ms * 1000 -- clock is in microseconds
	local start = pico.clock()
	while pico.clock() - start < ms do
		coroutine.yield() -- Pauses the coroutine, allowing other coroutines or the main program to run.
	end
end

tools.micro_wait = function(ms)
	local start = pico.clock()
	while pico.clock() - start < ms do
		coroutine.yield() -- Pauses the coroutine, allowing other coroutines or the main program to run.
	end
end
