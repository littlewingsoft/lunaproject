

--[[
		writed by jungmoona.
		desc: coroutine Test.

--]]


coTable={}

function Sleep( tbl)
	tbl.timeStamp = tbl.delay + os.clock()
	coroutine.yield()
end

function makeARGB( a,r,g,b)

	clr = b
	clr = clr + (g* math.pow(2,8) )
	clr = clr + (r* math.pow(2,16)  )
	clr = clr + (a* math.pow(2,24) )
	return clr
end

function loop( tblHandle )

	alpha = 0x80

	--print( string.format("%08x",clr) )
	luaprintOut_native( tblHandle.desc,tblHandle.x,tblHandle.y,0xffffffff,tblHandle.desc )

	while true do

			Sleep( tblHandle )
			--dbgprint( tblHandle.delay )
			alpha = alpha + 16
			if alpha >= 0xff then
				alpha =0
			end

			control_clr( tblHandle.desc, alpha,0xff,0xff,0xff )
	end
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

table_loop = makeThread( loop )
table_loop.desc = "table_loop"
table_loop.x= 10
table_loop.y= 10
table_loop.delay = 0
coroutine.resume( table_loop.coHandle,table_loop )

--[[
table_loop2 = makeThread( loop )
table_loop2.desc = "table_loop2"
table_loop2.x= 30
table_loop2.y= 30
table_loop2.delay = 0.00000001
coroutine.resume( table_loop2.coHandle,table_loop2 )
--]]

function wakeCheck()

	curr = os.clock()
	for i,v in ipairs(coTable) do
		if curr >= v.timeStamp + v.delay  then
			v.timeStamp = curr
			coroutine.resume( v.coHandle )
		end
	end
end


