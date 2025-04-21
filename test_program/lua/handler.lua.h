const char *lua_handler = "\
-- This is an attempt at scheduling a main loop using coroutines but its not working yet\n\
\n\
-- local function noop()\n\
-- 	print(\"no loop\")\n\
-- end\n\
-- local function main_loop()\n\
-- 	local user_main_loop = User_main_loop or noop\n\
-- 	print(\"Start waiting...\")\n\
-- 	tools.wait(3) -- Wait for 3 seconds\n\
-- 	print(\"Finished waiting!\")\n\
-- 	while true do\n\
-- 		user_main_loop() -- Call the user-defined main loop function\n\
-- 		tools.wait(1) -- Wait for 1 second before the next iteration\n\
-- 	end\n\
-- end\n\
\n\
-- local co = coroutine.create(main_loop)\n\
--\n\
-- -- Scheduler to resume the coroutine\n\
-- while coroutine.status(co) ~= \"dead\" do\n\
-- 	coroutine.resume(co)\n\
-- end\n\
";
