

--[[
특정시간동안 해당 스레드를 멈추게 하는데.
핸들이 필요하진 않으려나
--]]

function sleepTime( handle, sleepVal )

    if handle~=nul and fiberTable[handle] ~= nil then
		fiberTable[handle].sleepVal = sleepVal
		coroutine.yield()
	end
end


--[[
function cofunc (i)
	print("start")
		while true do
		 print("coroutine.yield", i)

				 print("coroutine.resume", ret )
					if ret ~= nil and ret == -1 then
					break
		 end

	end
     print("end")
end
--]]

function makeFiber( sleepVal )
	t={}
	t.handle = coroutine.create( func )
	t.sleepVal = sleepVal
	t.timeStamp = os.clock() + sleepVal

	table.insert( t )
	return t
end


bUpdate = true

--- native 코드에서 불러줘야함.
function Terminate()
bUpdate = false
end

function printFunc(str)
print( "luaPrint",str )
end


fiberTable={}
--[[
fiberTable.threadHandle = 0
fiberTable.sleepVal = 0
]]--

function update_checkYieldState()

    print( "update_checkYieldState begin" )
	--등록된 모든 스레드의 핸들과 시간값을 검사해서 sleep 값이 0이 아니면
	--체크후에 현재 시간과 슬립의 값을 검사하여 다시 resume 시킨다.
	while bUpdate do
		currClock = os.clock()
		for i,ft in ipairs(fiberTable) do
			if ft.sleepVal ~= 0 and ft.timeStamp <= currClock then
				ft.sleepVal = 0
				coroutine.resume(ft.handle)
			end
		end
	end

	print( "update_checkYieldState end" )
end

function makeUpdateFiber()
	bUpdate = true
	handle = coroutine.create( update_checkYieldState )
	coroutine.resume( handle )
end

--[[
	bUpdate = true
	last = os.clock()
	while bUpdate do
		if os.clock() - last > 0.001 then
		print( "clock", os.clock() )
		print( "resum_1", coroutine.resume(co,18) )
		last = os.clock()
		end
	end
--]]
-----------------------------------------------------------------------------------------------------------

function mainEntry()
	--makeUpdateFiber()
	--makeFiberTable()
	update_checkYieldState()
end


mainEntry() -- 업데이트 스레드 하나를 생성시키고 수다쟁이 스레드 2개를 생성함.
	   -- uiControl 들을 여기서 테이블로 만들어서 돌릴 예정.
	   -- 각각의 uiControl 은 입력에 대한 처리를 스스로 할예정임.
	   -- uiControl제작순서: 판넬, 이미지, 버튼, 이모든것은 극한의 조합으로 만들어질것.
	   -- 상속등은 전혀 쓰이지않는다.

--[[
원하는 최종 결과

수다쟁이 A 와 침묵의 B 가 서로 떠드는 데
A 가 2번 말할때 B가 1번 말한다.

수다쟁이 A 는 슬립타임이 B 에 비해 절반이면 된다. A:100 B:200
--]]

