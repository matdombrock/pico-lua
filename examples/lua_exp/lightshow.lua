-- THIS DOESNT WORK

--
-- This is the defualt lua script that will run when nothing else is provided by the user
-- This will be modifed on a per-app basis
--

local function blink_loop()
	for i = 1, 100 do
		print(i)
		pico.led(true)
		pico.wait(i)
		pico.led(false)
		pico.wait(i)
	end
end

local function blink_loop_fib()
	for i = 1, 12 do
		local fib = tools.fibonacci(i)
		print(fib)
		pico.led(true)
		pico.wait(fib * 10)
		pico.led(false)
		pico.wait(fib * 10)
	end
end

-- A crude PWM test
local function blink_pulse()
	for i = 1, 1000 do
		local pos = (i / 500) * 6.283185307179586
		local sin = tools.sin_norm(pos)
		local delay = math.floor(sin * 10000)
		pico.led(true)
		tools.micro_wait(1 + delay)
		pico.led(false)
		tools.micro_wait(1 + delay)
	end
end

local is_on = false -- This is effectively a global variable
local function print_sin()
	for i = 0, 628 do
		local pos = (i / 100) * 3.14 * 2
		local val = 1 + math.floor(tools.sin_norm(pos) * 32)
		local str = ""
		for j = 1, val do
			local c = " "
			if j == val then
				c = "*"
			end
			str = str .. c
		end
		print(str)
		pico.led(is_on)
		is_on = not is_on
		pico.wait(50)
	end
end

local function startup()
	print("Hello from Lua!")
	local test = app.set_state(42069) -- Call the custom app function
	print("test " .. test)
	local flash_used = pico.flash_used()
	print("Flash used: " .. flash_used .. " bytes")
	print("Waiting 2 seconds...")
	pico.wait(2000)
end

function Main_loop(tick)
	print("=== Main Loop ===")
	print("Tick number: " .. tick)

	-- Only run the startup function once
	if tick == 1 then
		startup()
	end
	-- Print the system uptime
	-- This is calling to a patched lua function so its behavior different from the original
	local time = os.clock()
	print("System time: " .. time)

	print("Waiting 2 seconds...")
	pico.wait(2000)

	print("== Blink Loop ==")
	blink_loop()
	print("== Blink Loop  Fib ==")
	blink_loop_fib()
	print("== Blink Pulse ==")
	blink_pulse()
	print("== Sine Wave ==")
	print_sin()

	time = os.clock()
	print("System time: " .. time)

	print("Waiting 1 second...")
	pico.wait(1000)

	if tick > 99 then
		print("Exiting main loop after 100 ticks")
		os.exit()
		return
	end
end
