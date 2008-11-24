
coTable={}

function Sleep( tbl)
	tbl.timeStamp = tbl.delay + os.clock()
	coroutine.yield()
end


function makeThread( funk )
	coHandle = coroutine.create( funk )
	tbl={}
	tbl.desc = "none"
	tbl.timeStamp = os.clock()
	tbl.x = 0
	tbl.y = 0
	tbl.delay = 0.3
	tbl.coHandle = coHandle
	table.insert( coTable, tbl )

	return tbl
end



function wakeCheck()

	curr = os.clock()
	for i,v in ipairs(coTable) do
		if curr >= v.timeStamp + v.delay  then
			v.timeStamp = curr
			coroutine.resume( v.coHandle )
		end
	end
end


