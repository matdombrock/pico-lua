const char *lua_script = "\
print(\"Hello from Lua\")\n\
\n\
local flash_used, flash_free, flash_total = pico.flash_info()\n\
\n\
print(\"Flash used: \" .. flash_used .. \" bytes\")\n\
print(\"Flash free: \" .. flash_free .. \" bytes\")\n\
print(\"Flash size: \" .. flash_total .. \" bytes\")\n\
\n\
-- print the system uptime\n\
-- This is calling to a patched lua function so its behavior different from the original\n\
local time = os.time()\n\
print(\"System time: \" .. time .. \" seconds since boot\")\n\
\n\
print(\"Waiting 2 seconds...\")\n\
pico.sleep_ms(2000)\n\
\n\
local function blink_loop()\n\
	for i = 1, 100 do\n\
		print(i)\n\
		pico.led(true)\n\
		pico.sleep_ms(i)\n\
		pico.led(false)\n\
		pico.sleep_ms(i)\n\
	end\n\
end\n\
\n\
local function blink_loop_fib()\n\
	for i = 1, 10 do\n\
		local fib = tools.fibonacci(i)\n\
		print(fib)\n\
		pico.led(true)\n\
		pico.sleep_ms(fib * 10)\n\
		pico.led(false)\n\
		pico.sleep_ms(fib * 10)\n\
	end\n\
end\n\
\n\
local function print_sin()\n\
	for i = 0, 100 do\n\
		local pos = (i / 100) * 3.14 * 2\n\
		local val = 1 + math.floor(tools.sin_norm(pos) * 8)\n\
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
\n\
for i = 1, 2 do\n\
	blink_loop()\n\
end\n\
for i = 1, 3 do\n\
	blink_loop_fib()\n\
end\n\
for i = 1, 8 do\n\
	print_sin()\n\
end\n\
\n\
print(\"Lua completed!\")\n\
\n\
time = os.clock()\n\
print(\"System time: \" .. time .. \" seconds since boot\")\n\
\n\
print(\"Waiting 1 second...\")\n\
pico.sleep_ms(1000)\n\
\n\
print(\"Process will exit to C code now...\")\n\
";
