const char *pl_script_system = "\
--\n\
-- Handles running the user's lua script and abstrcting some asyc stuff\n\
-- This is part of the core picolua system and likely will remain unchanged between apps\n\
--\n\
\n\
local function noop()\n\
	print(\"noop\")\n\
end\n\
\n\
local function main_loop()\n\
	print(\"Waiting 1 second before starting main loop...\")\n\
	pico.wait(1000) -- Wait for 1 second\n\
	print(\"======= STARTING MAIN LOOP =======\")\n\
	local user_main_loop = Main_loop or noop\n\
	local tick = 1\n\
	local last_start = pico.clock()\n\
	while true do\n\
		-- Trigger an app specific event before each loop if it exists\n\
		if app.before_main_loop then\n\
			app.before_main_loop(tick)\n\
		end\n\
		local tstart = pico.clock()\n\
		local delta = tstart - last_start\n\
		last_start = tstart\n\
		user_main_loop(tick, delta) -- Call the user-defined main loop function\n\
		local runtime = pico.clock() - tstart\n\
		tick = tick + 1\n\
		-- pico.wait(1) -- Wait for 1ms before the next iteration\n\
		-- Trigger an app specific event after each loop if it exists\n\
		if app.after_main_loop then\n\
			app.after_main_loop(tick, runtime)\n\
		end\n\
	end\n\
end\n\
\n\
local co = coroutine.create(main_loop)\n\
\n\
-- Scheduler to resume the coroutine\n\
while coroutine.status(co) ~= \"dead\" do\n\
	coroutine.resume(co)\n\
end\n\
";
