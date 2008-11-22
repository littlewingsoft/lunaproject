

--[[
Ư���ð����� �ش� �����带 ���߰� �ϴµ�.
�ڵ��� �ʿ����� ��������
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

--- native �ڵ忡�� �ҷ������.
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
	--��ϵ� ��� �������� �ڵ�� �ð����� �˻��ؼ� sleep ���� 0�� �ƴϸ�
	--üũ�Ŀ� ���� �ð��� ������ ���� �˻��Ͽ� �ٽ� resume ��Ų��.
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


mainEntry() -- ������Ʈ ������ �ϳ��� ������Ű�� �������� ������ 2���� ������.
	   -- uiControl ���� ���⼭ ���̺�� ���� ���� ����.
	   -- ������ uiControl �� �Է¿� ���� ó���� ������ �ҿ�����.
	   -- uiControl���ۼ���: �ǳ�, �̹���, ��ư, �̸����� ������ �������� ���������.
	   -- ��ӵ��� ���� �������ʴ´�.

--[[
���ϴ� ���� ���

�������� A �� ħ���� B �� ���� ����� ��
A �� 2�� ���Ҷ� B�� 1�� ���Ѵ�.

�������� A �� ����Ÿ���� B �� ���� �����̸� �ȴ�. A:100 B:200
--]]

