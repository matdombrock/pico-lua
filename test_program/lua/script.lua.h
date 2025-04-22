const char *lua_script = "\
--\n\
-- This is an example of an end user script\n\
--\n\
\n\
local function blink_loop()\n\
	for i = 1, 100 do\n\
		print(i)\n\
		pico.led(true)\n\
		tools.wait(i)\n\
		pico.led(false)\n\
		tools.wait(i)\n\
	end\n\
end\n\
\n\
local function blink_loop_fib()\n\
	for i = 1, 12 do\n\
		local fib = tools.fibonacci(i)\n\
		print(fib)\n\
		pico.led(true)\n\
		tools.wait(fib * 10)\n\
		pico.led(false)\n\
		tools.wait(fib * 10)\n\
	end\n\
end\n\
\n\
-- A crude PWM test\n\
local function blink_pulse()\n\
	for i = 1, 1000 do\n\
		local pos = (i / 500) * 6.283185307179586\n\
		local sin = tools.sin_norm(pos)\n\
		local delay = math.floor(sin * 10000)\n\
		pico.led(true)\n\
		tools.micro_wait(1 + delay)\n\
		pico.led(false)\n\
		tools.micro_wait(1 + delay)\n\
	end\n\
end\n\
\n\
local is_on = false -- This is effectively a global variable\n\
local function print_sin()\n\
	for i = 0, 628 do\n\
		local pos = (i / 100) * 3.14 * 2\n\
		local val = 1 + math.floor(tools.sin_norm(pos) * 32)\n\
		local str = \"\"\n\
		for j = 1, val do\n\
			local c = \" \"\n\
			if j == val then\n\
				c = \"*\"\n\
			end\n\
			str = str .. c\n\
		end\n\
		print(str)\n\
		pico.led(is_on)\n\
		is_on = not is_on\n\
		tools.wait(50)\n\
	end\n\
end\n\
\n\
local function startup()\n\
	print(\"Hello from Lua!\")\n\
	local test = app.test() -- Call the custom app function\n\
	print(\"test \" .. test)\n\
	local flash_used, flash_free, flash_total = pico.flash_info()\n\
	print(\"Flash used: \" .. flash_used .. \" bytes\")\n\
	print(\"Flash free: \" .. flash_free .. \" bytes\")\n\
	print(\"Flash size: \" .. flash_total .. \" bytes\")\n\
	print(\"Waiting 2 seconds...\")\n\
	tools.wait(2000)\n\
end\n\
\n\
function Main_loop(tick)\n\
	print(\"=== Main Loop ===\")\n\
	print(\"Tick number: \" .. tick)\n\
\n\
	-- Only run the startup function once\n\
	if tick == 1 then\n\
		startup()\n\
	end\n\
	-- Print the system uptime\n\
	-- This is calling to a patched lua function so its behavior different from the original\n\
	local time = os.clock()\n\
	print(\"System time: \" .. time)\n\
\n\
	print(\"Waiting 2 seconds...\")\n\
	tools.wait(2000)\n\
\n\
	print(\"== Blink Loop ==\")\n\
	blink_loop()\n\
	print(\"== Blink Loop  Fib ==\")\n\
	blink_loop_fib()\n\
	print(\"== Blink pcallulse ==\")\n\
	blink_pulse()\n\
	print(\"== Sine Wave ==\")\n\
	print_sin()\n\
\n\
	time = os.clock()\n\
	print(\"System time: \" .. time)\n\
\n\
	print(\"Waiting 1 second...\")\n\
	tools.wait(1000)\n\
\n\
	if tick > 99 then\n\
		print(\"Exiting main loop after 100 ticks\")\n\
		os.exit()\n\
		return\n\
	end\n\
end\n\
";
