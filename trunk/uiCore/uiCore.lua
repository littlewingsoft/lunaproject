

function cofunc (i)
	print("start")
		while true do
		 print("coroutine.yield", i)
				 ret = coroutine.yield(i)
				 print("coroutine.resume", ret )
					if ret ~= nil and ret == -1 then
					break
		 end

	end
     print("end")
end


co = coroutine.create( cofunc )

bUpdate = true

function Terminate()
bUpdate = false
end

function printFunc(str)
print( "luaPrint",str )
end

--printOut_native("18")

function main()
	bUpdate = true
	last = os.clock()
	while bUpdate do
		if os.clock() - last > 0.001 then
		print( "clock", os.clock() )
		print( "resum_1", coroutine.resume(co,18) )
		last = os.clock()
		end
	end
end

main()




--print( "resum_2", coroutine.resume(co,-1) )
print( "threadStatus", coroutine.status(co) )

--[[

print( "time", os.time() )
--]]

--print( "resum", coroutine.resume(co,2) )
--print( "resum", coroutine.resume(co,3) )
--print( "resum", coroutine.resume(co,18) )

