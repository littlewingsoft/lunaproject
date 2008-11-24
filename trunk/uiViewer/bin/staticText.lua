

function makeARGB( a,r,g,b)

	clr = b
	clr = clr + (g* math.pow(2,8) )
	clr = clr + (r* math.pow(2,16)  )
	clr = clr + (a* math.pow(2,24) )
	return clr
end

function moveText( tblHandle )
x = GetMousePositionX()
y = GetMousePositionY()
control_textPos( tblHandle.desc,x,y )
end

function loop( tblHandle )



	--print( string.format("%08x",clr) )
	luaprintOut_native( tblHandle.name,tblHandle.x,tblHandle.y,0xffffffff,tblHandle.text )

	while bUpdate and  do

		for i,v in tblHandle.controllerTable do
			Sleep( tblHandle ) -- 정해진 시간만큼 대기함

			v( tblHandle )
		end

	end
end

--[[
		animation controller 3가지 구현해야함.
		Position,
		Alpha,
		Scale,
--]]

twinkle={}
twinkle.alpha = 0x80
twinkle.Process= twinkle_process

function twinkle_process( tblHandle, alpha )
	alpha = alpha + 16
		if 	alpha >= 0xff then
			alpha =0
		end

	control_clr( tblHandle.desc, alpha,0xff,0xff,0xff )
end

--[[
       unitTest코드임 나중엔 뺄것이다.

table_loop = makeThread( loop )
table_loop.desc = "lablel1"
table_loop.x= 10
table_loop.y= 10
table_loop.delay = 0.01
table_loop.text = "coroutine Test!!!!"
coroutine.resume( table_loop.coHandle,table_loop )
--]]


table_loop1 = makeThread( staticloop )
table_loop1.name = "lablel1"
table_loop1.x= 10
table_loop1.y= 10
table_loop1.delay = 0.01
table_loop1.text = "controllerTest"
table_loop1.controllerTable = { twinkle, moveText } -- 모든 객체는 루프와 슬립은 기본으로 들어가 있어야 합니다.
coroutine.resume( table_loop.coHandle,table_loop )
