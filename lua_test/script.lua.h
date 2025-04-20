const char *lua_script = "\
print(\"Hello from Lua\")\n\
-- local function blink_loop()\n\
-- 	for i = 1, 100 do\n\
-- 		print(i)\n\
-- 		pico.led(true)\n\
-- 		pico.sleep_ms(i)\n\
-- 		pico.led(false)\n\
-- 		pico.sleep_ms(i)\n\
-- 	end\n\
-- end\n\
-- for i = 1, 2 do\n\
-- 	blink_loop()\n\
-- end\n\
-- -- blink the Fibonacci sequence\n\
-- local function fibonacci(n)\n\
-- 	if n <= 1 then\n\
-- 		return n\n\
-- 	else\n\
-- 		return fibonacci(n - 1) + fibonacci(n - 2)\n\
-- 	end\n\
-- end\n\
--\n\
-- local function blink_loop_fib()\n\
-- 	for i = 1, 10 do\n\
-- 		local fib = fibonacci(i)\n\
-- 		print(fib)\n\
-- 		pico.led(true)\n\
-- 		pico.sleep_ms(fib * 10)\n\
-- 		pico.led(false)\n\
-- 		pico.sleep_ms(fib * 10)\n\
-- 	end\n\
-- end\n\
-- for i = 1, 3 do\n\
-- 	blink_loop_fib()\n\
-- end\n\
\n\
local function print_sin()\n\
	for i = 0, 100 do\n\
		local pos = (i / 100) * 3.14 * 2\n\
		local val = 9 + math.floor(math.sin(pos) * 8)\n\
		local str = \"\"\n\
		for j = 1, val do\n\
			local c = \" \"\n\
			if j == val then\n\
				c = \"*\"\n\
			end\n\
			str = str .. c\n\
		end\n\
		print(str)\n\
		pico.sleep_ms(10)\n\
	end\n\
end\n\
for i = 1, 3 do\n\
	print_sin()\n\
end\n\
\n\
print(\"Lua completed!\")\n\
print(\"Process will exit to C code now...\")\n\
";
