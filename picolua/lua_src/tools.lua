--
-- Define a custom library of helper functions that is automatically made available to the main lua script
-- This file provides an easy way for apps to define a custom set of lua tools that are specific to that app
-- This file should be changed to fit the needs of the project
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
