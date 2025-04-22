-- This is an attempt at scheduling a main loop using coroutines but its not working yet

local function noop()
	print("no loop")
end
local function main_loop()
	print("======= STARTING MAIN LOOP =======")
	local user_main_loop = Main_loop or noop
	print("Start waiting...")
	tools.wait(3) -- Wait for 3 seconds
	print("Finished waiting!")
	local tick = 1
	while true do
		user_main_loop(tick) -- Call the user-defined main loop function
		tick = tick + 1
		tools.wait(1) -- Wait for 1 second before the next iteration
	end
end

local co = coroutine.create(main_loop)

-- Scheduler to resume the coroutine
while coroutine.status(co) ~= "dead" do
	coroutine.resume(co)
end
