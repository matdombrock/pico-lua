--
-- Handles running the user's lua script and abstrcting some asyc stuff
-- This is part of the core picolua system and likely will remain unchanged between apps
--

local function noop()
	print("noop")
end

local function main_loop()
	print("Waiting 1 second before starting main loop...")
	pico.wait(1000) -- Wait for 1 second
	print("======= STARTING MAIN LOOP =======")
	local user_main_loop = Main_loop or noop
	local tick = 1
	local last_start = pico.clock()
	while true do
		-- Trigger an app specific event before each loop if it exists
		if app.before_main_loop then
			app.before_main_loop(tick)
		end
		local tstart = pico.clock()
		local delta = tstart - last_start
		last_start = tstart
		user_main_loop(tick, delta) -- Call the user-defined main loop function
		local runtime = pico.clock() - tstart
		tick = tick + 1
		-- Trigger an app specific event after each loop if it exists
		if app.after_main_loop then
			app.after_main_loop(tick, runtime)
		end
	end
end

local co = coroutine.create(main_loop)

-- Scheduler to resume the coroutine
while coroutine.status(co) ~= "dead" do
	coroutine.resume(co)
end
