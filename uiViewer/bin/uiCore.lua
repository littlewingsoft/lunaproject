

--[[
		writed by jungmoona.
		desc: ��� Ȯ��Ǵ� lua Script �� ���⼭ �ε��ϱ����. this is Rule

--]]


dofile( "uiCoroutine.lua" )
dofile( "staticText.lua" )
dofile( "button.lua" )


--native_ImgLoad( "name", "a.dds" )
MakeControl( "textFollow",{	ctrl_TextOut ,	ctrl_TextAlpha,	ctrl_FollowMouse} )
MakeControl( "button", {	ctrlr_ImgLoading, ctrlr_CheckMouseOver  }  ) --ctrlr_ImgFollow
