print("Hello from Lua")

local flash_size, flash_used, flash_free = pico.flash_info()

print("Flash size: " .. flash_size .. " bytes")
print("Flash used: " .. flash_used .. " bytes")
print("Flash free: " .. flash_free .. " bytes")

pico.sleep_ms(500)

local function blink_loop()
	for i = 1, 100 do
		print(i)
		pico.led(true)
		pico.sleep_ms(i)
		pico.led(false)
		pico.sleep_ms(i)
	end
end

local function blink_loop_fib()
	for i = 1, 10 do
		local fib = tools.fibonacci(i)
		print(fib)
		pico.led(true)
		pico.sleep_ms(fib * 10)
		pico.led(false)
		pico.sleep_ms(fib * 10)
	end
end

local function print_sin()
	for i = 0, 100 do
		local pos = (i / 100) * 3.14 * 2
		local val = 1 + math.floor(tools.sin_norm(pos) * 8)
		local str = ""
		for j = 1, val do
			local c = " "
			if j == val then
				c = "*"
			end
			str = str .. c
		end
		print(str)
		pico.sleep_ms(10)
	end
end

for i = 1, 2 do
	blink_loop()
end
for i = 1, 3 do
	blink_loop_fib()
end
for i = 1, 8 do
	print_sin()
end

print("Lua completed!")
print("Process will exit to C code now...")
