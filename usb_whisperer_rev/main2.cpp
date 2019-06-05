// Connecting_with_SQLConnect.cpp
// compile with: user32.lib odbc32.lib
#include <windows.h>
#include "Shlwapi.h"
#include <string>
#include <sstream>
#include <stdio.h>
#include <fstream>
#include <limits>

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

int checkForFile(const char *fname){
	int exists = 0;
	exists = PathFileExistsA(fname);
	return exists;
}int zgetExponent(int xint, int zExponent){
	//
	int zint = 0;
	int zxint = 0;
	zint = xint;
	for(zxint = 1;zxint < zExponent;zxint++){
		zint *= xint;
	}
	return zint;
}
float zgetExponent2(float xint, float zExponent){
	//
	float zint = 0;
	int zxint = 0;
	zint = xint;
	for(zxint = 1;zxint < zExponent;zxint++){
		zint *= xint;
	}
	return zint;
}
bool zControlKeyEnterLetter(char xchar){
	//
	switch(xchar){
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		case 'g':
		case 'h':
		case 'i':
		case 'j':
		case 'k':
		case 'l':
		case 'm':
		case 'n':
		case 'o':
		case 'p':
		case 'q':
		case 'r':
		case 's':
		case 't':
		case 'u':
		case 'v':
		case 'w':
		case 'x':
		case 'y':
		case 'z':
		case '/':
		case '.':
		case '_':
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'G':
		case 'H':
		case 'I':
		case 'J':
		case 'K':
		case 'L':
		case 'M':
		case 'N':
		case 'O':
		case 'P':
		case 'Q':
		case 'R':
		case 'S':
		case 'T':
		case 'U':
		case 'V':
		case 'W':
		case 'X':
		case 'Y':
		case 'Z':{
			return true;
			break;
			   }
		default:{
			return false;
				}
	}
}
bool zControlKeyEnterSyntax(char xchar){
	//
	switch(xchar){
		case '(':
		case ')':{
			return true;
			break;
				 }
		case ',':{
			return true;
			break;
				 }
		default:{
			return false;
				}
	}
}
bool zControlKeyEnterNumber(char xchar){
	//
	switch(xchar){
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '-':{
			return true;
			break;
				 }
		default:{
			return false;
				}
	}
}
int intconvchar(std::string xchar){
	//
	char zchar = xchar[0];
	//zchar = xchar.data();
	switch(zchar){
		case '0':{
			return 0;
			break;
				 }
		case '1':{
			return 1;
			break;
				 }
		case '2':{
			return 2;
			break;
				 }
		case '3':{
			return 3;
				 }
		case '4':{
			return 4;
			break;
				 }
		case '5':{
			return 5;
			break;
				 }
		case '6':{
			return 6;
			break;
				 }
		case '7':{
			return 7;
			break;
				 }
		case '8':{
			return 8;
			break;
				 }
		case '9':{
			return 9;
			break;
				 }
		default:{
			return 0;
				}
	}
}
int intconvcharw(std::wstring xchar){
	//
	wchar_t zchar = xchar[0];
	//zchar = xchar.data();
	switch(zchar){
		case L'0':{
			return 0;
			break;
				 }
		case L'1':{
			return 1;
			break;
				 }
		case L'2':{
			return 2;
			break;
				 }
		case L'3':{
			return 3;
				 }
		case L'4':{
			return 4;
			break;
				 }
		case L'5':{
			return 5;
			break;
				 }
		case L'6':{
			return 6;
			break;
				 }
		case L'7':{
			return 7;
			break;
				 }
		case L'8':{
			return 8;
			break;
				 }
		case L'9':{
			return 9;
			break;
				 }
		default:{
			return 0;
				}
	}
}
int intconvstr(std::string xchar){
	//
	if(xchar.length() < 1){
		return 0;
	}
	int xint = 0;
	std::string xstring = "";
	int xlength = xchar.length();
	for(int i=0;i<xlength;i++){
		xstring = xchar[((xlength - 1) - i)];
		xint += (intconvchar(xstring) * (i > 0 ? (zgetExponent(10, i)) : (1)));
	}
	if(xchar[0] == '-'){
		//xint -= 2;
		/**
		float xint2 = 0;;
		xint2 = xint;
		xint2 *= -1;
		return xint2;**/
		int xint2 = 0;
		xint2 -= xint;
		return xint2;
	}
	return xint;
}
int intconvstr2(std::string xchar, int xint2){
	//
	if(xchar.length() < 1){
		return 0;
	}
	int xint = 0;
	int xint3 = 0;
	xint3 = xchar.length();
	if(xint2 < xint3){
		xint3 = xint2;
	}
	std::string xstring = "";
	for(int i=0;i<xint3;i++){
		xstring = xchar[((xchar.length() - 1) - i)];
		if(xstring == "."){
			break;
		}
		xint += (intconvchar(xstring) * (i > 0 ? (zgetExponent(10, i)) : (1)));
	}
	if(xchar[0] == '-'){
		//xint -= 2;
		/**
		float xint2 = 0;;
		xint2 = xint;
		xint2 *= -1;
		return xint2;**/
	}
	return xint;
}
float floatconvstr(std::string xchar){
	//
	if(xchar.length() < 1){
		return 0;
	}
	float xint = 0;
	float xint2 = 0;
	int xint3 = 0;
	bool xbool = false;
	int xint4 = xchar.length();
	int xint5 = 0;
	for(int i=0;i<xint4;i++){
		if(xchar[i] == '.'){
			xbool = true;
			xint5 = i;
			break;
		}
	}
	std::string xstring = "";
	if(xbool){
		for(int i=0;i<xint4;i++){
			xstring = xchar[((xchar.length() - 1) - i)];
			if(xstring == "."){
				xint3 = i;
				//xint3++;
				break;
			}
			xint2 = float((xint4 + 1) - i);
			xint2 *= -1;
			xint += (intconvchar(xstring) * (i > 0 ? (zgetExponent2(10, xint2)) : (1)));
		}
	}
	for(int i=xint3;i<xint4;i++){
		xstring = xchar[((xchar.length() - 1) - i)];
		if(xstring == "."){
			break;
		}
		xint += (intconvchar(xstring) * (i > 0 ? (zgetExponent(10, i)) : (1)));
	}
	if(xchar[0] == '-'){
		//xint -= 2;
		/**
		float xint2 = 0;;
		xint2 = xint;**/
		xint2 = xint;
		xint2 *= -1;
		return xint2;
	}
	return xint;
}
std::string zgetintstr(int xint){
	ostringstream osstream;
	osstream << xint;
	return osstream.str();
}
std::wstring zgetintstr2(int xint){
	wostringstream osstream;
	osstream << xint;
	return osstream.str();
}
std::string zgetfstr(f32 xint){
	ostringstream osstream;
	osstream << xint;
	return osstream.str();
}
std::wstring zgetfstr2(f32 xint){
	wostringstream osstream;
	osstream << xint;
	return osstream.str();
}
int intconvstrw(std::wstring xchar){
	//
	if(xchar.length() < 1){
		return 0;
	}
	int xint = 0;
	std::wstring xstring = L"";
	int xlength = xchar.length();
	for(int i=0;i<xlength;i++){
		xstring = xchar[((xlength - 1) - i)];
		xint += (intconvcharw(xstring) * (i > 0 ? (zgetExponent(10, i)) : (1)));
	}
	if(xchar[0] == '-'){
		//xint -= 2;
		/**
		float xint2 = 0;;
		xint2 = xint;
		xint2 *= -1;
		return xint2;**/
		int xint2 = 0;
		xint2 -= xint;
		return xint2;
	}
	return xint;
}
class MyEventReceiver : public IEventReceiver
{
public:
    struct SMouseState
    {
        core::position2di Position;
        bool LeftButtonDown;
        bool RightButtonDown;
		float wheel;
        SMouseState() : LeftButtonDown(false), wheel(7) { }
    } MouseState;
    // This is the one method that we have to implement
    virtual bool OnEvent(const SEvent& event)
    {
        // Remember whether each key is down or up
        if (event.EventType == irr::EET_KEY_INPUT_EVENT)
            KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
        if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
        {
            switch(event.MouseInput.Event)
            {
            case EMIE_LMOUSE_PRESSED_DOWN:
                MouseState.LeftButtonDown = true;
                break;

            case EMIE_LMOUSE_LEFT_UP:
                MouseState.LeftButtonDown = false;
                break;

            case EMIE_RMOUSE_PRESSED_DOWN:
                MouseState.RightButtonDown = true;
                break;

            case EMIE_RMOUSE_LEFT_UP:
                MouseState.RightButtonDown = false;
                break;

            case EMIE_MOUSE_MOVED:
                MouseState.Position.X = event.MouseInput.X;
                MouseState.Position.Y = event.MouseInput.Y;
                break;

			case EMIE_MOUSE_WHEEL:
				MouseState.wheel += event.MouseInput.Wheel;
				break;

            default:
                // We won't use the wheel
                break;
            }
        }
		//if(event.EventType == irr::EET_MOUSE_INPUT_EVENT)
			//KeyIsDown[event.MouseInput.ButtonStates] = event.MouseInput.isLeftPressed;
		//if(event.EventType == irr::EET_MOUSE_INPUT_EVENT)
			//KeyIsDown[event.MouseInput.ButtonStates] = event.MouseInput.Event;

        return false;
    }

    // This is used to check whether a key is being held down
    virtual bool IsKeyDown(EKEY_CODE keyCode) const
    {
        return KeyIsDown[keyCode];
    }

	virtual bool ControlKeyCheck(int keyint) const{
		return KeyIsDown[keyint];
	}
    
    MyEventReceiver()
    {
        for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
            KeyIsDown[i] = false;
    }

private:
    // We use this array to store the current state of each key
    bool KeyIsDown[KEY_KEY_CODES_COUNT];
};
class xfpslimiter{
	//
	int MaxFPS;
	int MaxSDLFPS;
	int FPS;
public:
	int nowTime;
	xfpslimiter();
	void sdlwait();
	irr::ITimer *timer;
};
xfpslimiter::xfpslimiter(){
	//
	MaxFPS = 30;
	MaxSDLFPS = 30;
	FPS = 0;
	nowTime = 0;
}
void xfpslimiter::sdlwait(){
	if(timer->getTime() - nowTime < 1000/MaxFPS){
		FPS = timer->getTime() - nowTime;
		if(FPS == 0){
			Sleep(1000/MaxSDLFPS);
		}else{
			Sleep(int((int((1000/FPS)))/MaxSDLFPS));
		}
	}
}
std::string zgetreststr(std::string xstring, int xint){
	//
	std::string zxstring = "";
	int zxint = 0;
	zxint = xstring.length();
	for(int xint2 = xint;xint2<zxint;xint2++){
		zxstring += xstring[xint2];
	}
	return zxstring;
}
std::wstring zgetreststr2(std::wstring xstring, int xint){
	//
	std::wstring zxstring = L"";
	int zxint = 0;
	zxint = xstring.length();
	for(int xint2 = xint;xint2<zxint;xint2++){
		zxstring += xstring[xint2];
	}
	return zxstring;
}
std::string zgetstrto(std::string xstring, int xint, char tochar){
	//
	std::string zxstring = "";
	for(int xint2 = xint;xint2<xstring.length();xint2++){
		if(xstring[xint2] == tochar){
			break;
		}else{
			zxstring += xstring[xint2];
		}
	}
	return zxstring;
}
std::wstring zgetstrto2(std::wstring xstring, int xint, wchar_t tochar){
	//
	std::wstring zxstring = L"";
	for(int xint2 = xint;xint2<xstring.length();xint2++){
		if(xstring[xint2] == tochar){
			break;
		}else{
			zxstring += xstring[xint2];
		}
	}
	return zxstring;
}
int intconvchar2(char xchar){
	//
	char zchar = xchar;
	//zchar = xchar.data();
	switch(zchar){
		case '0':{
			return 0;
			break;
				 }
		case '1':{
			return 1;
			break;
				 }
		case '2':{
			return 2;
			break;
				 }
		case '3':{
			return 3;
				 }
		case '4':{
			return 4;
			break;
				 }
		case '5':{
			return 5;
			break;
				 }
		case '6':{
			return 6;
			break;
				 }
		case '7':{
			return 7;
			break;
				 }
		case '8':{
			return 8;
			break;
				 }
		case '9':{
			return 9;
			break;
				 }
		default:{
			return 0;
				}
	}
}
char xconvchar(wchar_t xchar){
	//
	char zchar = int(xchar);
	return zchar;
}
wchar_t zconvchar(char xchar){
	//
	wchar_t zchar = int(xchar);
	return zchar;
}
std::string xconvstr(std::wstring xstring){
	//
	int xlength = xstring.length();
	std::string zstring = "";
	for(int xint = 0;xint < xlength;xint++){
		zstring += xconvchar(xstring[xint]);
	}
	return zstring;
}
std::wstring zconvstr(std::string xstring){
	//
	int xlength = xstring.length();
	std::wstring zstring = L"";
	for(int xint = 0;xint < xlength;xint++){
		zstring += zconvchar(xstring[xint]);
	}
	return zstring;
}
f32 ffromstr(std::string str){
	int xlen = str.length();
	if(xlen < 1){
		return 0.0f;
	}
	f32 newf = 0.0f;
	std::string strpart = zgetstrto(str, 0, '.');
	std::string fint = strpart;
	xlen = strpart.length();
	xlen++;
	strpart = zgetstrto(str, xlen, '.');
	std::string fdec = strpart;
	newf += f32(intconvstr(fint));
	int decimal = 0;
	std::string decis = "1";
	xlen = fdec.length();
	for(int xint = 0;xint < xlen;xint++){
		decis += "0";
	}
	decimal = intconvstr(decis);
	newf += f32(f32(intconvstr(fdec)) / f32(decimal));
	return newf;
}
int timetoint(int hour, int min, int sec){
	int xint = 0;
	xint += sec;
	xint += (min * 60);
	xint += (hour * 3600);
	return xint;
}
std::string timetostr(SYSTEMTIME time){
	std::string str;
	str += zgetintstr(time.wHour);
	str += " : ";
	if(time.wMinute < 10)str += "0";
	str += zgetintstr(time.wMinute);
	return str;
}
std::string mirrorstr(std::string str){
	std::string xstr = "";
	int len = str.length();
	for(int xint = len;xint > 0;xint--){
		xstr += str[xint - 1];
	}
	return xstr;
}
std::string inttobin(unsigned int xint){
	std::string str = "0000 0000 0000 0000 0000 0000 0000 0000";
	if(xint > 0){
		str = "";
		int check = 0;
		unsigned int tmp = xint;
		for(int zint = 0;zint < 32;zint++){
			if((tmp>>zint)&1)str += "1"; else str += "0";
			//tmp>>1;
			check++;
			if(check == 4 && zint < 31){
				str += " ";
				check = 0;
			}
		}
	}
	return mirrorstr(str);
}