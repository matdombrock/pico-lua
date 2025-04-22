const char *lua_tools = "\
-- Define a custom library of helper functions that is automatically made available to the main lua script\n\
tools = {}\n\
-- Return a value at the nth position of the fibonacci sequence\n\
tools.fibonacci = function(n)\n\
	if n <= 1 then\n\
		return n\n\
	else\n\
		return tools.fibonacci(n - 1) + tools.fibonacci(n - 2)\n\
	end\n\
end\n\
\n\
tools.sin_norm = function(x)\n\
	return 1 + math.sin(x) * 0.5\n\
end\n\
\n\
tools.wait = function(ms)\n\
	-- ms = ms * 1000 -- clock is in microseconds\n\
	pico_sleep_ms(1000) -- Sleep for the specified number of milliseconds\n\
	-- local start = pico_clock()\n\
	-- print(\"x    \" .. start)\n\
	-- while pico_clock() - start < ms do\n\
	-- 	-- coroutine.yield() -- Pauses the coroutine, allowing other coroutines or the main program to run.\n\
	-- end\n\
end\n\
\n\
tools.micro_wait = function(ms)\n\
	pico_sleep_ms(1000) -- Sleep for the specified number of microseconds\n\
	-- local start = pico_clock()\n\
	-- print(\"y    \" .. start)\n\
	-- while pico_clock() - start < ms do\n\
	-- coroutine.yield() -- Pauses the coroutine, allowing other coroutines or the main program to run.\n\
	-- end\n\
end\n\
";
