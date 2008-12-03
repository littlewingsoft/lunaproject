

function makeARGB( a,r,g,b)

	clr = b
	clr = clr + (g* math.pow(2,8) )
	clr = clr + (r* math.pow(2,16)  )
	clr = clr + (a* math.pow(2,24) )
	return clr
end



function loop( tblHandle )

--	tblHandle.InitController.Process(tblHandle,tblHandle.InitController)

	while true do

		for i, v in ipairs(tblHandle.controllerTable) do
			Sleep( tblHandle ) -- ������ �ð���ŭ �����
			v.Process( tblHandle,v , i )
		end

		

	end
end

--[[
		animation controller 3���� �����ؾ���.
		Position,
		Alpha,
		Scale,
--]]

	ctrl_TextOut ={}
	ctrl_TextOut .Text = "controllerTest" 
	ctrl_TextOut .Process= function ( tblHandle, ctrl_TextOut , i )
						dbgprint( "ctrl_TextOut.Process" )
						tmpTable={}
						tmpTable.name= tblHandle.name
						tmpTable.Text = ctrl_TextOut.Text
						tmpTable.x = 0
						tmpTable.y = 0
						tmpTable.clr = 0xffffffff
						luaprintOut_native( tmpTable )
						table.remove( tblHandle.controllerTable, i  )
					end


	ctrl_FollowMouse={}
	ctrl_FollowMouse.Process = function ( tblHandle, ctrl_FollowMouse )
							--dbgprint( "ctrl_FollowMouse.Process" )
							x = GetMousePositionX()+15
							y = GetMousePositionY()+5
							control_textPos( tblHandle.name,x,y )
						end

	ctrl_Alpha={}
	ctrl_Alpha.value = 0x80
	ctrl_Alpha.Process= function ( tblHandle, ctrl_Alpha )
					--dbgprint( "ctrl_Alpha.Process" )
							ctrl_Alpha.value = ctrl_Alpha.value + 16
						if 	ctrl_Alpha.value >= 0xff then
							ctrl_Alpha.value =0
						end

						control_clr( tblHandle.name, ctrl_Alpha.value,0xff,0xff,0xff )
					end

--[[
       unitTest�ڵ��� ���߿� �����̴�.
--]]


-- ��� ��ü�� ������ ������ �⺻���� �� �־�� �մϴ�.
table_loop1 = makeThread( loop )
table_loop1.name = "lablel1"
table_loop1.delay = 0.0

table_loop1.controllerTable = 
{ 
	ctrl_TextOut ,
	ctrl_Alpha,
	ctrl_FollowMouse 

} 


--[[

						tmpTable={}
						tmpTable.name="label1"
						tmpTable.x = 18
						tmpTable.y = 18
						tmpTable.clr = 0xffffffff
						luaprintOut_native( tmpTable )
---]]

coroutine.resume(	table_loop1.coHandle,	    table_loop1 )


