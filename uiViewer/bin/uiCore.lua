

--[[
		writed by jungmoona.
		desc: coroutine Test.

--]]

dofile( "uiCoroutine.lua" )
dofile( "staticText.lua" )


--[[
table_loop2 = makeThread( loop )
table_loop2.desc = "_loop2"
table_loop2.x= 30
table_loop2.y= 30
table_loop2.delay = 0.00000001
coroutine.resume( table_loop2.coHandle,table_loop2 )
--]]
