
coTable={}
controlTable={}

function Sleep( tbl)
	tbl.timeStamp = tbl.delay + os.clock()
	coroutine.yield()
end


function makeThread( name, funk )
	coHandle = coroutine.create( funk )
	tbl={}
	tbl.name = name
	tbl.timeStamp = os.clock()
	tbl.coHandle = coHandle
	table.insert( coTable, tbl )

	return tbl
end


function wakeCheck()

	curr = os.clock()
	for i,v in ipairs(coTable) do
		if curr >= v.timeStamp + v.delay  then
			v.timeStamp = curr
			coroutine.resume( v.coHandle )
		end
	end
end
---------------------------------------------------------
-- nothread Version
-- 괜히 쓰레드를 쓸일이 딱히 없다.
function MakeControl( name, param_controllerTable )
		ctrl={}
		ctrl.name = name
		ctrl.controllerTable= param_controllerTable

		function ctrl.Update (ctrl)
			for i,controller in ipairs( ctrl.controllerTable ) do
				curr = os.clock()

				if controller.timeStamp ~= nil and controller.delay ~= nil then

					if curr >= controller.timeStamp + controller.delay  then
						controller.timeStamp = curr
						controller.Process( ctrl, controller, i )
					end
				else
					controller.Process( ctrl, controller, i )
				end

			end
		end
	dbgprint( name )
	table.insert( controlTable, ctrl )
end

function UpdateControl()
	for i,v in ipairs(controlTable) do
		v.Update(v)
	end

end
