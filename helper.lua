-- Elements from olcPixelGameEngine adapted for usage in lua.
function Pixel(a,b,c,d)
	return a | (b << 8) | (c << 16) | ((d or 0xFF) << 24)
end

keys = {["NONE"] = 0,
		["A"] = 1, ["B"] = 2, ["C"] = 3, ["D"] = 4, ["E"] = 5, ["F"] = 6, ["G"] = 7, ["H"] = 8, ["I"] = 9, ["J"] = 10, ["K"] = 11, ["L"] = 12, ["M"] = 13, ["N"] = 14, ["O"] = 15, ["P"] = 16, ["Q"] = 17, ["R"] = 18, ["S"] = 19, ["T"] = 20, ["U"] = 21, ["V"] = 22, ["W"] = 23, ["X"] = 24, ["Y"] = 25, ["Z"] = 26,
		["K0"] = 27, ["K1"] = 28, ["K2"] = 29, ["K3"] = 30, ["K4"] = 31, ["K5"] = 32, ["K6"] = 33, ["K7"] = 34, ["K8"] = 35, ["K9"] = 36,
		["F1"] = 37, ["F2"] = 38, ["F3"] = 39, ["F4"] = 40, ["F5"] = 41, ["F6"] = 42, ["F7"] = 43, ["F8"] = 44, ["F9"] = 45, ["F10"] = 46, ["F11"] = 47, ["F12"] = 48,
		["UP"] = 49, ["DOWN"] = 50, ["LEFT"] = 51, ["RIGHT"] = 52,
		["SPACE"] = 53, ["TAB"] = 54, ["SHIFT"] = 55, ["CTRL"] = 56, ["INS"] = 57, ["DEL"] = 58, ["HOME"] = 59, ["END"] = 60, ["PGUP"] = 61, ["PGDN"] = 62,
		["BACK"] = 63, ["ESCAPE"] = 64, ["RETURN"] = 65, ["ENTER"] = 66, ["PAUSE"] = 67, ["SCROLL"] = 68,
		["NP0"] = 69, ["NP1"] = 70, ["NP2"] = 71, ["NP3"] = 72, ["NP4"] = 73, ["NP5"] = 74, ["NP6"] = 75, ["NP7"] = 76, ["NP8"] = 77, ["NP9"] = 78,
		["NP_MUL"] = 79, ["NP_DIV"] = 80, ["NP_ADD"] = 81, ["NP_SUB"] = 82, ["NP_DECIMAL"] = 83, ["PERIOD"] = 84,
		["EQUALS"] = 85, ["COMMA"] = 86, ["MINUS"] = 87,
		["OEM_1"] = 88, ["OEM_2"] = 89, ["OEM_3"] = 90, ["OEM_4"] = 91, ["OEM_5"] = 92, ["OEM_6"] = 93, ["OEM_7"] = 94, ["OEM_8"] = 95,
		["CAPS_LOCK"] = 96, ["ENUM_END"] = 97}

colors ={["GREY"] = Pixel(192, 192, 192), ["DARK_GREY"] = Pixel(128, 128, 128), ["VERY_DARK_GREY"] = Pixel(64, 64, 64),
		["RED"] = Pixel(255, 0, 0), ["DARK_RED"] = Pixel(128, 0, 0), ["VERY_DARK_RED"] = Pixel(64, 0, 0),
		["YELLOW"] = Pixel(255, 255, 0), ["DARK_YELLOW"] = Pixel(128, 128, 0), ["VERY_DARK_YELLOW"] = Pixel(64, 64, 0),
		["GREEN"] = Pixel(0, 255, 0), ["DARK_GREEN"] = Pixel(0, 128, 0), ["VERY_DARK_GREEN"] = Pixel(0, 64, 0),
		["CYAN"] = Pixel(0, 255, 255), ["DARK_CYAN"] = Pixel(0, 128, 128), ["VERY_DARK_CYAN"] = Pixel(0, 64, 64),
		["BLUE"] = Pixel(0, 0, 255), ["DARK_BLUE"] = Pixel(0, 0, 128), ["VERY_DARK_BLUE"] = Pixel(0, 0, 64),
		["MAGENTA"] = Pixel(255, 0, 255), ["DARK_MAGENTA"] = Pixel(128, 0, 128), ["VERY_DARK_MAGENTA"] = Pixel(64, 0, 64),
		["WHITE"] = Pixel(255, 255, 255), ["BLACK"] = Pixel(0, 0, 0), ["BLANK"] = Pixel(0, 0, 0, 0)}

--Homebrew Functions

--Button metaclass, has vertices and an action function
--Every button "has" to be a rectangle.
Button = {fVertex = {}, Dimen = {}, name = "", col = colors.WHITE, fn = nil}

function Button:create(fVertex, Dimen, name, col, fn)
	btt = {}
	setmetatable(btt, self)
	self.__index = self
	table.insert(self.fVertex, {xD = fVertex.xD, yD = fVertex.yD})
	table.insert(self.Dimen, {xD = Dimen.xD, yD = Dimen.yD})
	self.name = name
	self.col = col
	self.fn = fn
	
	return btt
end

--checks whether mouse is hovering the button
function Button:is_hovering()
	local k = {xD = GetMouseX(), yD = GetMouseY()}
	flag = (k.xD >= self.fVertex.xD and k.xD <= (self.fVertex.xD + self.Dimen.xD))
	flag = flag and (k.yD >= self.fVertex.yD and k.yD <= (self.fVertex.yD + self.Dimen.yD))
	return flag
end

--if mouse is hovering, it calls fn and returns true
--for this function to return the correct flag, fn has to 
--be a void function.
function Button:activate()
	local act = ((self.is_hovering() and self.fn()) == nil)
	return act
end

-- some utility functions

--Draws centralized Text
function DrawCentralS(x, y, str, col, scale) 
	local pos = x - (str:len()*4*scale)
	DrawString(pos, y, str, col, scale)
end