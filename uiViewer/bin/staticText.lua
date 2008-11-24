

function makeARGB( a,r,g,b)

	clr = b
	clr = clr + (g* math.pow(2,8) )
	clr = clr + (r* math.pow(2,16)  )
	clr = clr + (a* math.pow(2,24) )
	return clr
end



function loop( tblHandle )

	tblHandle.InitController.Process(tblHandle,tblHandle.InitController)

	while true do

		for i, v in ipairs(tblHandle.controllerTable) do
			Sleep( tblHandle ) -- ������ �ð���ŭ �����
			v.Process( tblHandle,v )
		end
	end
end

--[[
		animation controller 3���� �����ؾ���.
		Position,
		Alpha,
		Scale,
--]]

ctrl_TextOut={}
ctrl_TextOut.Text = "none"
ctrl_TextOut.Process = function ( tblHandle, ctrl_TextOut )
						--dbgprint( "ctrl_TextOut.Process" )
						luaprintOut_native( tblHandle.name,tblHandle.x,tblHandle.y,
											0xffffffff,ctrl_TextOut.Text )
						end


ctrl_FollowMouse={}
ctrl_FollowMouse.Process = function ( tblHandle, ctrl_FollowMouse )
							--dbgprint( "ctrl_FollowMouse.Process" )
							x = GetMousePositionX()
							y = GetMousePositionY()
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


table_loop1 = makeThread( loop )
table_loop1.name = "lablel1"
table_loop1.x= 10
table_loop1.y= 10
table_loop1.delay = 0.001
ctrl_TextOut.Text = "controllerTest"
table_loop1.InitController = ctrl_TextOut
table_loop1.controllerTable = { ctrl_Alpha,ctrl_FollowMouse } -- ��� ��ü�� ������ ������ �⺻���� �� �־�� �մϴ�.
coroutine.resume( table_loop1.coHandle,table_loop1 )
