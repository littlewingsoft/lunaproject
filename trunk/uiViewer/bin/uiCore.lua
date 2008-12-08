

--[[
		writed by jungmoona.
		desc: 모든 확장되는 lua Script 는 여기서 로딩하기로함. this is Rule

--]]


dofile( "uiCoroutine.lua" )
dofile( "staticText.lua" )
dofile( "button.lua" )


--native_ImgLoad( "name", "a.dds" )
MakeControl( "textFollow",{	ctrl_TextOut ,	ctrl_TextAlpha,	ctrl_FollowMouse} )
MakeControl( "button", {	ctrlr_ImgLoading, ctrlr_CheckMouseOver  }  ) --ctrlr_ImgFollow
