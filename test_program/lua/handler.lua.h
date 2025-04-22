const char *lua_handler = "\
--\n\
-- Handles running the user's lua script and abstrcting some asyc styff\n\
--\n\
\n\
local function noop()\n\
	print(\"no loop\")\n\
end\n\
local function main_loop()\n\
	print(\"======= STARTING MAIN LOOP =======\")\n\
	local user_main_loop = Main_loop or noop\n\
	print(\"Start waiting...\")\n\
	tools.wait(3) -- Wait for 3 seconds\n\
	print(\"Finished waiting!\")\n\
	local tick = 1\n\
	while true do\n\
		user_main_loop(tick) -- Call the user-defined main loop function\n\
		tick = tick + 1\n\
		tools.wait(1) -- Wait for 1 second before the next iteration\n\
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
