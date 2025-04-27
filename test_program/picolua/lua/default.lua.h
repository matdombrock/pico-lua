const char *pl_script_default = "\
--\n\
-- This is the defualt lua script that will run when nothing else is provided by the user\n\
-- This will be modifed on a per-app basis\n\
--\n\
\n\
function Main_loop(tick, delta)\n\
	print(\"=== Main Loop ===\")\n\
	print(\"Tick number: \" .. tick)\n\
	print(\"Delta time: \" .. delta .. \" microseconds\")\n\
	if tick > 99 then\n\
		print(\"Exiting main loop after 100 ticks\")\n\
		os.exit()\n\
		return\n\
	end\n\
end\n\
";
