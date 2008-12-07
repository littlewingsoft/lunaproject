

function makeARGB( a,r,g,b)

	clr = b
	clr = clr + (g* math.pow(2,8) )
	clr = clr + (r* math.pow(2,16)  )
	clr = clr + (a* math.pow(2,24) )
	return clr
end


function loop( hCtrl )

dbgprint( string.format("tablCnt:%d",table.maxn(hCtrl.controllerTable) ) )
	while true do

		for i, v in ipairs(hCtrl.controllerTable) do
			--Sleep( hCtrl ) -- 정해진 시간만큼 대기함
			v.Process( hCtrl,v , i )
		end



	end
end

--[[
		animation controller 3가지 구현해야함.
		Position,
		Alpha,
		Scale,
--]]

	ctrl_TextOut ={}
	ctrl_TextOut.timeStamp = os.clock()
	ctrl_TextOut.delay = 0.01
	ctrl_TextOut.Text = "controllerTest"
	function ctrl_TextOut.Process ( hCtrl, self , i )
						--dbgprint( "ctrl_TextOut.Process" )
--						tmpTable={}
--						tmpTable.name= hCtrl.name
--						tmpTable.Text = self.Text
--						tmpTable.x = 0
--						tmpTable.y = 0
--						tmpTable.clr = 0xffffffff
						native_textInsert(hCtrl.name, self.Text )
						native_textClr( hCtrl.name, 0xffffffff )
						native_textPosition( hCtrl.name, 0,0,0 )
						table.remove( hCtrl.controllerTable, i  )
					end


	ctrl_FollowMouse={}
	ctrl_FollowMouse.timeStamp = os.clock()
	ctrl_FollowMouse.delay = 0
	function  ctrl_FollowMouse.Process ( hCtrl, self )
							--dbgprint( "ctrl_FollowMouse.Process" )
							native_GetMousePosition() -- native 코드에서 마우스를 감지했다는 사실이 중요함
							x = mousePos.x +15
							y = mousePos.y +5
							native_textPosition( hCtrl.name, x, y )
						end


	ctrl_TextAlpha={}
	ctrl_TextAlpha.timeStamp = os.clock()
	ctrl_TextAlpha.delay = 0.05
	ctrl_TextAlpha.value = 0x80
	ctrl_TextAlpha.velSign = 1
	function ctrl_TextAlpha.Process( hCtrl, self )
					--dbgprint( "ctrl_Alpha.Process" )
							self.value = self.value + (self.velSign * 5 )
						if 	self.value >= 0xff then
							self.velSign =-1
							self.value = 0xff
						else if self.value <= -1 then
							self.velSign =1
							self.value = 0
							end
						end

						native_textClr( hCtrl.name, self.value,0xff,0xff,0xff )
					end

	ctrlr_ImgLoading =
	{
		value = "btn.bmp",
		Process = function ( hCtrl, self, i )
					--dbgprint( "ctrlr_ImgLoading.Process" )
					native_ImgLoad( hCtrl.name, self.value )
					native_ImgPosition( hCtrl.name, 100,100,1 )
					--lua_CheckMouseOver()
					native_ImgSetSrcRect( hCtrl.name, 0,0,32,32 )
					table.remove( hCtrl.controllerTable, i  )
					end
	}

	ctrlr_ImgFollow =
	{
			timeStamp = os.clock(),
			delay = 0,
			Process = 	function  ( hCtrl, self )
							native_GetMousePosition() -- native 코드에서 마우스를 감지했다는 사실이 중요함
							x = mousePos.x
							y = mousePos.y
							native_ImgPosition( hCtrl.name, x, y,1 )
						end

	}

	ctrlr_CheckMouseOver =
	{
		Process=function ( hCtrl, self, i )
					-- 현재 이미지 위치 위에 마우스가 있는가?
					bRet = native_CheckMouseOver(hCtrl.name) -- colorKey 까지도 체크해서 결과 넘겨줌.
					mouseState = native_UpdateMouseState()
					if mouseState.LButton == 0 then
						native_ImgSetSrcRect( hCtrl.name, 96,0,128,32 )

					end

					if mouseState.LButton == 1 then
--						native_ImgSetSrcRect( hCtrl.name, 64,0,95,32 )
						native_ImgSetSrcRect( hCtrl.name, 0,0,31,32 )
					end

					if mouseState.LButton == 2 then
						native_ImgSetSrcRect( hCtrl.name, 0,0,31,32 )
					end
					if mouseState.LButton == 3 then
						native_ImgSetSrcRect( hCtrl.name, 32,0,63,32 )
					end



				end
	}

	-- 특정컨트롤이 왼쪽 버튼으로 눌렸는가 체크.
	-- 루아에서 마우스 상태는 위치,버튼눌림상태를 얻어 온다.

	ctrlr_CheckMouseLButtonDown =
	{
		Process=function (hCtrl, self, i )


				end
	}



--native_ImgLoad( "name", "a.dds" )
MakeControl( "textFollow",{	ctrl_TextOut ,	ctrl_TextAlpha,	ctrl_FollowMouse} )
MakeControl( "button", {	ctrlr_ImgLoading,ctrlr_CheckMouseOver  }  ) --ctrlr_ImgFollow

--[[
ctrlImgAdd = {}
function ctrm_ImgAdd.Process ( hCtrl,self )

end

ImgControl = makeThread( loop )
ImgControl.name = "img1"
ImgControl.delay = 0.1
ImgControl.controllerTable =
{
	ctrl_ImgAdd ,
	ctrl_FollowMouse

}
--]]

--[[

						tmpTable={}
						tmpTable.name="label1"
						tmpTable.x = 18
						tmpTable.y = 18
						tmpTable.clr = 0xffffffff
						luaprintOut_native( tmpTable )
---]]

--coroutine.resume(	Textcontrol.coHandle,	    Textcontrol )


