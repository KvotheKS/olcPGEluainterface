/*
	"Author" : @KvotheKS
	A little program made to interface the olcPixelGameEngine (made by @javidx9) in lua,
	so that no classes have to be directly used during coding.

	
	Linux compile :	g++ -o cpp_base cpp_base.cpp -llua -ldl -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17
	msvc compile : cl /EHsc cpp_base.cpp
*/

#include "include/lua.hpp"
#define AUTOMATIC_REQUIRE
#define OLC_PGE_APPLICATION
#include "base_olc.h"
#include <memory>

#ifdef _WIN32
#pragma comment(lib, "liblua54.a")
#endif

class auxlib
{
public:
	static int random(lua_State* stk)
	{
		uint64_t seed = lua_tonumber(stk,1)*48271;
		uint32_t x = (seed & 0x7fffffff) + (seed >> 31);
		x = (x & 0x7fffffff) + (x >> 31);
		lua_pushnumber(stk,x);
		return 1;
	}
	static void reg_aux(lua_State* stk)
	{
		lua_register(stk, "random", random);
	}
};

class lua_hub
{
private:
	static lua_State* stk;
#ifdef AUTOMATIC_REQUIRE
	static std::string path;
#endif
private:
	friend class init;
	
	class PGExt : public olc::PixelGameEngine
	{
	public:
		bool OnUserCreate() override
		{
		
			lua_getglobal(stk, "Create");
			lua_pcall(stk, 1,1,0);
			return lua_toboolean(stk, -1);
		}
		bool OnUserUpdate(float fElapsedTime) override
		{
//			std::cout << "UPDATE";
			lua_getglobal(stk, "Update");
			lua_pushnumber(stk, fElapsedTime);
			lua_pcall(stk, 1,1,0);
			bool p = lua_toboolean(stk, -1);
			return p;
		}
	};
	
	static std::unique_ptr<PGExt> demiwp;
	
	class init
	{
	public:
		init()
		{
			demiwp = std::make_unique<PGExt>();
			
			stk = luaL_newstate();
			luaL_openlibs(stk);
			reg_fns();
			auxlib::reg_aux(stk);
		#ifdef AUTOMATIC_REQUIRE
			path = "/home/alexsander/Desktop/olcPGEluainterface-main";
			#ifdef _WIN32	
				luaL_dofile(stk, (path + "\134helper.lua").c_str());
			#else
				luaL_dofile(stk, (path + "/helper.lua").c_str());
			#endif
		#endif
		//	strcpy(updater,"update\0");
		}
		~init()
		{
			lua_close(stk);
		}
	}; 
	
	static init __helper;
public:
	static void set_file(const char* str)
	{luaL_dofile(stk, str);}
	
	static void reg_fns()
	{
		lua_register(stk, "ScreenWidth", ScreenWidth);
		lua_register(stk, "ScreenHeight", ScreenHeight);
		lua_register(stk,"Construct",Construct);
		lua_register(stk,"Start",Start);
		lua_register(stk,"Draw",Draw);
		lua_register(stk,"DrawLine",DrawLine);
		lua_register(stk,"DrawCircle",DrawCircle);
		lua_register(stk,"FillCircle",FillCircle);
		lua_register(stk,"DrawRect",DrawRect);
		lua_register(stk,"FillRect",FillRect);
		lua_register(stk,"DrawTriangle",DrawTriangle);
		lua_register(stk,"FillTriangle",FillTriangle);
		lua_register(stk,"DrawSprite",DrawSprite);
		lua_register(stk,"DrawPartialSprite",DrawPartialSprite);
		lua_register(stk,"DrawString",DrawString);
		lua_register(stk,"DrawStringProp",DrawStringProp);
		lua_register(stk,"DrawDecal",DrawDecal);
		lua_register(stk,"DrawPartialDecal",DrawPartialDecal);
		lua_register(stk,"DrawStringDecal",DrawStringDecal);
		lua_register(stk,"DrawStringPropDecal",DrawStringPropDecal);
		lua_register(stk,"FillRectDecal",FillRectDecal);
		lua_register(stk,"Clear",Clear);
		lua_register(stk,"KeyPressed",KeyPressed);
		lua_register(stk,"KeyHold",KeyHold);
		lua_register(stk,"MousePress",MousePress);
		lua_register(stk,"MouseHold",MouseHold);
		lua_register(stk,"GetMouseX",GetMouseX);
		lua_register(stk,"GetMouseY",GetMouseY);
		lua_register(stk,"GetMouseWheel",GetMouseWheel);
		lua_register(stk,"newDecal" , newDecal);
		lua_register(stk,"newSprite" , newSprite);
		lua_register(stk, "delDecal", delDecal);
		lua_register(stk, "delSprite", delSprite);
		//Test-only functions. Serve no other purpose.
		lua_register(stk, "newInt", newInt);
		lua_register(stk, "delInt", delInt);
	}
public:
	static int ScreenWidth(lua_State* stk)
	{
		lua_pushnumber(stk, demiwp->ScreenWidth());
		return 1;
	}
	static int ScreenHeight(lua_State* stk)
	{
		lua_pushnumber(stk, demiwp->ScreenHeight());
		return 1;
	}

	static int newInt(lua_State* stk)
	{
		int* p = new int;
		std::cout << p << std::endl;
		lua_pushnumber(stk, (long long)p);
		return 1;
	}
	static int delInt(lua_State* stk)
	{
		int* p = (int*)(long long)lua_tonumber(stk,1);
		std::cout << p << std::endl;
		delete p;
		return 1;
	}
	static int newDecal(lua_State* stk)
	{ 
		olc::Sprite* sprt = (olc::Sprite*)(long long)lua_tonumber(stk,1);
		lua_pushnumber(stk, (long long)new olc::Decal(sprt));
		return 1;
	}
	static int newSprite(lua_State* stk)
	{
		const char* str = lua_tostring(stk,1);
		lua_pushnumber(stk, (long long)new olc::Sprite(str));
		return 1;
	}
	static int delDecal(lua_State* stk)
	{ 
		long long t = lua_tonumber(stk, 1);
		delete (olc::Decal*)t;
		return 1;
	}
	static int delSprite(lua_State* stk)
	{
		long long t = lua_tonumber(stk,1);
		delete (olc::Sprite*)t;
		return 1;
	}
public:
	static int Construct(lua_State* stk);
	static int Start(lua_State* stk);
	static int Draw(lua_State* stk);
	static int DrawLine(lua_State* stk);
	static int DrawCircle(lua_State* stk);
	static int FillCircle(lua_State* stk);
	static int DrawRect(lua_State* stk);
	static int FillRect(lua_State* stk);
	static int DrawTriangle(lua_State* stk);
	static int FillTriangle(lua_State* stk);
	static int DrawSprite(lua_State* stk);
	static int DrawPartialSprite(lua_State* stk);
	static int DrawString(lua_State* stk);
	static int DrawStringProp(lua_State* stk);
	static int DrawDecal(lua_State* stk);
	static int DrawPartialDecal(lua_State* stk);
	static int DrawStringDecal(lua_State* stk);
	static int DrawStringPropDecal(lua_State* stk);
	static int FillRectDecal(lua_State* stk);
	static int Clear(lua_State* stk);
	static int KeyPressed(lua_State* stk);
	static int KeyHold(lua_State* stk);
	static int MousePress(lua_State* stk);
	static int MouseHold(lua_State* stk);
	static int GetMouseX(lua_State* stk);
	static int GetMouseY(lua_State* stk);
	static int GetMouseWheel(lua_State* stk);
};

lua_State* lua_hub::stk;
std::unique_ptr<lua_hub::PGExt> lua_hub::demiwp;
lua_hub::init lua_hub::__helper;
std::string lua_hub::path;

int lua_hub::Construct(lua_State* stk)
{
	int a, b, c, d;
	a = lua_tonumber(stk,1);
	b = lua_tonumber(stk,2);
	c = lua_tonumber(stk,3);
	d = lua_tonumber(stk,4);
	lua_pushboolean(stk,demiwp->Construct(a,b,c,d));
	return 1;
}

int lua_hub::Start(lua_State* stk)
{
	demiwp->Start();
	return 1;
}

int lua_hub::Draw(lua_State* stk)
{
	int a, b, c;
	a = lua_tonumber(stk,1);
	b = lua_tonumber(stk,2);
	c = lua_tointeger(stk,3);
	demiwp->Draw(a,b,olc::Pixel(c));
	return 1;
}

int lua_hub::DrawLine(lua_State* stk)
{
	int a, b, c, d, e;
	a = lua_tonumber(stk,1);
	b = lua_tonumber(stk,2);
	c = lua_tonumber(stk,3);
	d = lua_tonumber(stk,4);
	e = lua_tointeger(stk,5);
	demiwp->DrawLine(a,b,c,d,olc::Pixel(e));
	return 1;
}

int lua_hub::DrawCircle(lua_State* stk)
{
	int a, b, c, d;
	a = lua_tonumber(stk,1);
	b = lua_tonumber(stk,2);
	c = lua_tonumber(stk,3);
	d = lua_tointeger(stk,4);
	demiwp->DrawCircle(a,b,c,olc::Pixel(d));
	return 1;
}

int lua_hub::FillCircle(lua_State* stk)
{
	int a, b, c, d;
	a = lua_tonumber(stk,1);
	b = lua_tonumber(stk,2);
	c = lua_tonumber(stk,3);
	d = lua_tointeger(stk,4);
	demiwp->FillCircle(a,b,c,olc::Pixel(d));
	return 1;
}

int lua_hub::DrawRect(lua_State* stk)
{
	int a, b, c, d, e;
	a = lua_tonumber(stk,1);
	b = lua_tonumber(stk,2);
	c = lua_tonumber(stk,3);
	d = lua_tonumber(stk,4);
	e = lua_tointeger(stk,5);
	demiwp->DrawRect(a,b,c,d,olc::Pixel(e));
	return 1;
}

int lua_hub::FillRect(lua_State* stk)
{
	int a, b, c, d, e;
	a = lua_tonumber(stk,1);
	b = lua_tonumber(stk,2);
	c = lua_tonumber(stk,3);
	d = lua_tonumber(stk,4);
	e = lua_tointeger(stk,5);
	demiwp->FillRect(a,b,c,d,olc::Pixel(e));
	return 1;
}

int lua_hub::DrawTriangle(lua_State* stk)
{
	int a, b, c, d, e, f, g;
	a = lua_tonumber(stk,1);
	b = lua_tonumber(stk,2);
	c = lua_tonumber(stk,3);
	d = lua_tonumber(stk,4);
	e = lua_tonumber(stk,5);
	f = lua_tonumber(stk,6);
	g = lua_tointeger(stk,7);
	demiwp->DrawTriangle(a,b,c,d,e,f,olc::Pixel(g));
	return 1;
}

int lua_hub::FillTriangle(lua_State* stk)
{
	int a, b, c, d, e, f, g;
	a = lua_tonumber(stk,1);
	b = lua_tonumber(stk,2);
	c = lua_tonumber(stk,3);
	d = lua_tonumber(stk,4);
	e = lua_tonumber(stk,5);
	f = lua_tonumber(stk,6);
	g = lua_tointeger(stk,7);
	demiwp->FillTriangle(a,b,c,d,e,f,olc::Pixel(g));
	return 1;
}

int lua_hub::DrawSprite(lua_State* stk)
{
	int a, b, d;
	olc::Sprite* c;
	a = lua_tonumber(stk,1);
	b = lua_tonumber(stk,2);
	c = (olc::Sprite*) (long long) lua_tonumber(stk,3);
	d = lua_tonumber(stk,4);
	demiwp->DrawSprite(a,b, c, d);
	return 1;
}

int lua_hub::DrawPartialSprite(lua_State* stk)
{
	int a, b, d, e, f, g, h;
	olc::Sprite* c;
	a = lua_tonumber(stk,1);
	b = lua_tonumber(stk,2);
	c = (olc::Sprite*) (long long) lua_tonumber(stk,3);
	d = lua_tonumber(stk,4);
	e = lua_tonumber(stk,5);
	f = lua_tonumber(stk,6);
	g = lua_tonumber(stk,7);
	h = lua_tonumber(stk,8);
	demiwp->DrawPartialSprite(a, b, c, d, e, f, g, h);
	return 1;
}

int lua_hub::DrawString(lua_State* stk)
{
	int a, b, d, e;
	a = lua_tonumber(stk,1);
	b = lua_tonumber(stk,2);
	const char* c = lua_tostring(stk,3);
	d = lua_tonumber(stk,4);
	e = lua_tointeger(stk,5);
	demiwp->DrawString(a,b,c,d, e);
	return 1;
}

int lua_hub::DrawStringProp(lua_State* stk)
{
	int a, b, d, e;
	a = lua_tonumber(stk,1);
	b = lua_tonumber(stk,2);
	const char* c = lua_tostring(stk,3);
	d = lua_tonumber(stk,4);
	e = lua_tointeger(stk,5);
	demiwp->DrawStringProp(a,b, c, olc::Pixel(d), e);
	return 1;
}

int lua_hub::DrawDecal(lua_State* stk)
{
	const olc::vf2d a = {(float)lua_tonumber(stk,1) , (float)lua_tonumber(stk,2)};
	olc::Decal* b = (olc::Decal*) (long long) lua_tonumber(stk,3);
	const olc::vf2d c = {(float)lua_tonumber(stk,4),(float) lua_tonumber(stk,5)};
	int d = lua_tointeger(stk,6);
	demiwp->DrawDecal(a,b,c,d);
	return 1;
}

int lua_hub::DrawPartialDecal(lua_State* stk)
{
	const olc::vf2d a = {(float)lua_tonumber(stk,1) ,(float)lua_tonumber(stk,2)};
	olc::Decal* b = (olc::Decal*) (long long) lua_tonumber(stk,3);
	const olc::vf2d c = {(float)lua_tonumber(stk,4), (float)lua_tonumber(stk,5)};
	const olc::vf2d d = {(float)lua_tonumber(stk,6), (float)lua_tonumber(stk,7)};
	const olc::vf2d e = {(float)lua_tonumber(stk,8), (float)lua_tonumber(stk,9)};
	int f = lua_tointeger(stk,10);
	demiwp->DrawPartialDecal(a,b,c,d,e,f);
	return 1;
}

int lua_hub::DrawStringDecal(lua_State* stk)
{
	const olc::vf2d a = {(float)lua_tonumber(stk,1), (float)lua_tonumber(stk,2)};
	const char* b = lua_tostring(stk,3);
	int c = lua_tointeger(stk,4);
	const olc::vf2d d = {(float)lua_tonumber(stk,5), (float)lua_tonumber(stk,6)};
	demiwp->DrawStringDecal(a,b,c,d);
	return 1;
}

int lua_hub::DrawStringPropDecal(lua_State* stk)
{
	const olc::vf2d a = {(float)lua_tonumber(stk,1),(float)lua_tonumber(stk,2)};
	const char* b = lua_tostring(stk,3);
	int c = lua_tointeger(stk,4);
	const olc::vf2d d = {(float)lua_tonumber(stk,5),(float) lua_tonumber(stk,6)};
	demiwp->DrawStringPropDecal(a,b,c,d);
	return 1;
}

int lua_hub::FillRectDecal(lua_State* stk)
{
	const olc::vf2d a = {(float)lua_tonumber(stk,1),(float) lua_tonumber(stk,2)};
	const olc::vf2d b = {(float)lua_tonumber(stk,3),(float) lua_tonumber(stk,4)};
	int c = lua_tointeger(stk,5);
	demiwp->FillRectDecal(a,b,c);
	return 1;
}


int lua_hub::Clear(lua_State* stk)
{
	demiwp->Clear(lua_tointeger(stk,1));
	return 1;
}

int lua_hub::KeyPressed(lua_State* stk)
{
	int k = demiwp->GetKey((olc::Key)(long long)lua_tonumber(stk,1)).bPressed;
	lua_pushboolean(stk, k);
	return 1;
}

int lua_hub::KeyHold(lua_State* stk)
{
	int k = demiwp->GetKey((olc::Key)(long long)lua_tonumber(stk,1)).bHeld;
	lua_pushboolean(stk, k);
	return 1;

}

int lua_hub::MousePress(lua_State* stk)
{
	int p = demiwp->GetMouse((long long)lua_tonumber(stk,1)).bPressed;
	lua_pushboolean(stk,p);
	return 1;
}

int lua_hub::MouseHold(lua_State* stk)
{
	int p = demiwp->GetMouse((long long)lua_tonumber(stk,1)).bHeld;
	lua_pushboolean(stk,p);
	return 1;
}

int lua_hub::GetMouseX(lua_State* stk)
{
	lua_pushinteger(stk, demiwp->GetMouseX());
	return 1;
}

int lua_hub::GetMouseY(lua_State* stk)
{
	lua_pushinteger(stk, demiwp->GetMouseY());
	return 1;
}

int lua_hub::GetMouseWheel(lua_State* stk)
{
	lua_pushinteger(stk, demiwp->GetMouseWheel());
	return 1;
}


int main(int argc, char** argv)
{
	if(argc >= 2)
		lua_hub::set_file(argv[1]);
	else
		std::cout << "No Output Files!\n";
}