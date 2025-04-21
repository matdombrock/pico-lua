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
tools.wait = function(seconds)\n\
	local start = os.time()\n\
	while os.time() - start < seconds do\n\
		coroutine.yield() -- Pauses the coroutine, allowing other coroutines or the main program to run.\n\
	end\n\
end\n\
";
