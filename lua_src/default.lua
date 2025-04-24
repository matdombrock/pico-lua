--
-- This is the defualt lua script that will run when nothing else is provided by the user
-- This will be modifed on a per-app basis
--

function Main_loop(tick)
	print("=== Main Loop ===")
	print("Tick number: " .. tick)
	if tick > 99 then
		print("Exiting main loop after 100 ticks")
		os.exit()
		return
	end
end
