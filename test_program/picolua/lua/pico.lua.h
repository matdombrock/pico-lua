const char *pl_script_pico = "\
--\n\
-- This is a collection of helper functions for the Pico Lua environment.\n\
-- These functions extend the `pico` library defined in the C code\n\
-- This is part of the core picolua system and likely will remain unchanged between apps\n\
--\n\
\n\
if not pico then\n\
	pico = {}\n\
end\n\
\n\
pico.wait = function(ms)\n\
	ms = ms * 1000 -- clock is in microseconds\n\
	local start = pico.clock()\n\
	while pico.clock() - start < ms do\n\
		coroutine.yield() -- Pauses the coroutine, allowing other coroutines or the main program to run.\n\
	end\n\
end\n\
\n\
pico.micro_wait = function(ms)\n\
	local start = pico.clock()\n\
	while pico.clock() - start < ms do\n\
		coroutine.yield() -- Pauses the coroutine, allowing other coroutines or the main program to run.\n\
	end\n\
end\n\
\n\
pico.test = function()\n\
	print(\"pico\")\n\
end\n\
";
