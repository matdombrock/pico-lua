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
	while true do
		-- Trigger an app specific event before each loop if it exists
		if app.before_main_loop then
			app.before_main_loop(tick)
		end
		local tstart = pico.clock()
		user_main_loop(tick) -- Call the user-defined main loop function
		local runtime = pico.clock() - tstart
		tick = tick + 1
		pico.wait(1) -- Wait for 1ms before the next iteration
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
