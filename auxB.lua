--File that holds aditional tools


--Button metaclass, has vertices and an action function
--Every button "has" to be a rectangle.
Button = {fVertex = {xD = 0, yD = 0}, Dimen = {xD = 0, yD = 0}, name = "", bkgcol = colors.WHITE, 
			textcol = colors.BLACK, fn = nil}
Button.__index = Button

function Button:new(fVertexs, Dimen, name, bkgcol,textcol, fn)
	return setmetatable({
		fVertex = {xD = (fVertexs and fVertexs.xD) or 0,
				   yD = (fVertexs and fVertexs.yD) or 0},
		Dimen = {xD = (Dimen and Dimen.xD) or 0,
			     yD = (Dimen and Dimen.yD) or 0},
		name = name or "",
		bkgcol = bkgcol or colors.WHITE,
		textcol = textcol or colors.BLACK,
		fn = fn
	}, self)
end

--checks whether mouse is hovering the button
function Button:is_hovering(mouse)
	local flag = (mouse.xD >= (self.fVertex.xD - self.Dimen.xD) 
			and mouse.xD <= (self.fVertex.xD + self.Dimen.xD))
	flag = flag and (mouse.yD >= (self.fVertex.yD - self.Dimen.xD) 
					 and mouse.yD <= (self.fVertex.yD + self.Dimen.yD))
	return flag
end

function Button:DrawCentralized()
	local x = self.fVertex.xD - self.Dimen.xD
	local y = self.fVertex.yD - self.Dimen.yD
	FillRect(x, y, self.Dimen.xD*2, self.Dimen.yD*2, self.bkgcol)
	DrawRect(x, y, self.Dimen.xD*2, self.Dimen.yD*2, colors.BLACK)
	DrawCentralS(self.fVertex.xD, self.fVertex.yD, self.name, self.textcol, 1) 
end

--if mouse is hovering and left button was clicked it calls fn and returns true
function Button:activate(mouse)
	local mouse = mouse or {xD = GetMouseX(), yD = GetMouseY()}
	local act = ((self:is_hovering(mouse) and MousePress(0)))
	local t = act and self:fn()
	return t or (t == nil)
end

-- some utility functions

--Draws centralized Text
function DrawCentralS(x, y, str, col, scale) 
	local pos = x - (str:len()*4*scale)
	DrawString(pos,y-(4*scale), str, col, scale)
end

DButton = Button:new()
DButton.__index = DButton

--Slightly more complex button
--When mouse is hovering it, it "shrinks"
--fVertex represents the location in the middle of the button
--Dimen represents the dimensions of each side of the button
function DButton:new(fVertexs, Dimen, name, bkgcol, textcol, fn)
	return setmetatable({
		fVertex = {xD = (fVertexs and fVertexs.xD) or 0,
				   yD = (fVertexs and fVertexs.yD) or 0},
		Dimen = {xD = (Dimen and Dimen.xD) or 0,
			     yD = (Dimen and Dimen.yD) or 0},
		name = name or "",
		bkgcol = bkgcol or colors.WHITE,
		textcol = textcol or colors.BLACK,
		fn = fn,
		times = 0
	}, self)
end

function DButton:DrawCentralized(m)
	local m = m or {xD = GetMouseX(), yD = GetMouseY()}
	
	if(self:is_hovering(m)) then
		if(self.times < 20) then
			self.times = self.times + 1
		end
	else
		self.times = 0
	end 

	local tam = (100-self.times)/100
	local Dx = math.floor(self.Dimen.xD*tam)
	local Dy = math.floor(self.Dimen.yD*tam)
	local x = self.fVertex.xD - Dx
	local y = self.fVertex.yD - Dy

	FillRect(x, y, Dx*2, Dy*2, self.bkgcol)
	DrawRect(x, y, Dx*2, Dy*2, colors.BLACK)
	DrawCentralS(self.fVertex.xD, self.fVertex.yD, self.name, self.textcol, 1)
end


--Button made to be used in a config enviro.
--has a name which goes in the middle of the button
--And a Key.
CButton = {fxD = 0, Dimen = {xD = 0, yD = 0}, name = "", bkgcol = colors.WHITE, 
			textcol = colors.BLACK, fn = nil, key = ""}
CButton.__index = CButton

--Dimen.xD is the total width, considering both
--Button and key.

function CButton:new(fVertexs, Dimen, name, bkgcol, textcol, key)
	return setmetatable({
		fVertex = {xD = (fVertexs and fVertexs.xD) or 0,
				   yD = (fVertexs and fVertexs.yD) or 0},
		Dimen = {xD = (Dimen and Dimen.xD) or 0,
			     yD = (Dimen and Dimen.yD) or 0},
		name = name or "",
		bkgcol = bkgcol or colors.WHITE,
		textcol = textcol or colors.BLACK,
		key = key or "",
		cBuff = false
	}, self)
end

function CButton:activate(mouse)
	local mouse = mouse or {xD = GetMouseX(), yD = GetMouseY()}
	local act = (self:is_hovering(mouse) and MousePress(0))
	self.cBuff = act
	return act
end

function CButton:print(yD)
	local x = self.fVertex.xD - self.Dimen.xD
	local y = yD - self.Dimen.yD
	local lW = math.floor(self.Dimen.xD*0.8)

	FillRect(x,y,lW,self.Dimen.yD*2, self.bkgcol)
	DrawRect(x,y,lW,self.Dimen.yD*2, colors.BLACK)
	
	DrawCentralS(x + math.floor(lW*0.5), yD, self.name, self.textcol, 1)
	DrawCentralS(self.fVertex.xD, yD, "-->", self.textcol, 1)

	x = self.fVertex.xD + math.floor(self.Dimen.xD*0.2)
	y = yD + self.Dimen.yD
	DrawLine(x, y, x + math.floor(self.Dimen.xD*0.8), y, self.bkgcol)

	x = x + math.floor(self.Dimen.xD*0.4)
	DrawCentralS(x, y - 5, self.key, self.textcol, 1)
end


--"Snek Config Button"

SCButton = CButton:new()
SCButton.__index = SCButton

function SCButton:new(fVertexs, Dimen, name, bkgcol, textcol, key, typef)
	return setmetatable({
		fVertex = {xD = (fVertexs and fVertexs.xD) or 0,
				   yD = (fVertexs and fVertexs.yD) or 0},
		Dimen = {xD = (Dimen and Dimen.xD) or 0,
			     yD = (Dimen and Dimen.yD) or 0},
		name = name or "",
		bkgcol = bkgcol or colors.WHITE,
		textcol = textcol or colors.BLACK,
		key = key or "",
		cBuff = false,
		type = typef or 1,
		tBuff = ""
	}, self)
end

function SCButton:activate(mouse)
	local mouse = mouse or {xD = GetMouseX(), yD = GetMouseY()}
	local act = (self:is_hovering(mouse) and MousePress(0))
	return act
end

function SCButton:is_hovering(mouse)
	local flag = (mouse.xD >= (self.fVertex.xD - self.Dimen.xD) 
			and mouse.xD <= (self.fVertex.xD - math.floor(self.Dimen.xD*0.2)))
	flag = flag and (mouse.yD >= (self.fVertex.yD - self.Dimen.xD) 
					 and mouse.yD <= (self.fVertex.yD + self.Dimen.yD))
	return flag
end

function SCButton:Update(mouse)
	if(self.cBuff) then 
		self:ConcatBuff()
	else
		dequeuePBuffer()
		self.cBuff = self:activate(mouse)
	end
end

function SCButton:ConcatBuff()
	local buffer = {dequeuePBuffer()}
	for _,v in pairs(buffer) do
		if(v == '\n') then self:confirm(); return end
		self.tBuff = self.tBuff .. v
	end
end

function SCButton:confirm()
	if(self.type == 1) then
		self.key = keys[self.tBuff] or self.key	
	else 
		self.key = colors[self.tBuff] or tonumber(self.tBuff) or self.key	
	end

	self.cBuff = false
	self.tBuff = ""
end

function SCButton:printT1()

end	