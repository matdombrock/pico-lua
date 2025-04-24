const char *pl_script_tools = "\
--\n\
-- Define a custom library of helper functions that is automatically made available to the main lua script\n\
-- This file provides an easy way for apps to define a custom set of lua tools that are specific to that app\n\
-- This file should be changed to fit the needs of the project\n\
--\n\
\n\
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
";
