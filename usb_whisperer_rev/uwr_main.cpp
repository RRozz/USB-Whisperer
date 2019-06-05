/****     -- Using irrlicht and hidapi  --
This is a little project that lists the connected HIDs and prompts the user to select one
by inputting its VID and PID in the following format: "vid pid" (separated by a space).
(The user could also type "q" and end the program)
If the user has entered a valid VID and PID that both match a currently connected HID,
then the selected HID will be monitored and a set of values will be displayed. If the controller
changes any value (such as by button press), then the change will be reflected in the text on-screen.

Controls:
[PAGE_UP] / [mouse wheel scrolled up] --> change current "page" of displayed values to lesser indices.
[PAGE_DOWN] / [mouse wheel scrolled down] --> change page to greater indices.
[ESC] --> terminate program
[SPACE] --> print all 65 buffers to console; good for comparing momentary snapshots of values


How to use:
Enter a matching VID and PID from the list of devices and look on the right side for a change in
value after pressing/releasing a button or moving an analog control. It will display which bits have
changed.



What it's good for:
It's great for identifying which values correlate with the state of which button on a given connected
device. Then, you know which values to check for when using that device via hidapi.



Using this program, I have successfully been able to map out the values corresponding to the buttons
of several controllers. The resulting mappings are in the ressystem/ subdirectory.

The controllers I mapped are:

PS2 wired controller (connected via USB-to-PS2_controller connector)
(the rest of the HIDs were connected directly via USB)
PS3 wired controller
PS4 wiresless controller connected via USB charging cable
PC/PS3 steering wheel controller (+ pedals)
Logitech 3D Extreme flight stick controller
Logitech G Saitek Pro Flight Switch Panel (i managed to find all switch/knob/lever values but not LEDs)
Xbox 360 wired controller


*conversion from integer to string representing binary value of that integer may be inefficient

**Some HIDs (keyboard, mouse, tablet) will be listed but, if selected, will not show any change
in any values. I think this is because the operating system uses drivers that first intercept and
interpret these devices... but the real, technical reason is out of the scope of my current knowledge.
****/

#include "irrlicht.h"
#include "windows.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "Main2.cpp"
#include "hidapi.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

int main(int argc, char* args[]){
	// 1. initialize irrlicht

		MyEventReceiver receiver;
	IrrlichtDevice *device =
		createDevice(EDT_OPENGL, core::dimension2d<u32>(1030, 745), 16, false, false, true, &receiver);
	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* env = device->getGUIEnvironment();
	device->setWindowCaption(L"USB Whisperer - Revisited");




	// 2. grab the irrlicht window and move it to the upper left corner (0,0) -- this part is not necessary

	HWND hwnd;
	hwnd = FindWindowA(NULL, "USB Whisperer - Revisited");
	device->maximizeWindow();
	MoveWindow(hwnd, 0, 0, driver->getScreenSize().Width, driver->getScreenSize().Height, true);




	// 3. set-up irrlicht environment
	// This includes setting the receiver to get keyboard/mouse input, loading a font and background,
	// then scaling the background.

	device->setEventReceiver(&receiver);
	IGUIImage *bg;
	{
		std::string file = "ressystem/bg.";
		// It will check for several formats of a 'bg' image in "ressystem/" subdirectory.
		// First it will check png, then jpg, jpeg, and bmp.
		// It will use the first that it finds to be present.
		// This was jst for convenience in changing background images to keep things fresh.
		{
			std::string ext = file;
			ext += "png";
			if(checkForFile(ext.data())){
				file = ext;
			}else{
				ext = file;
				ext += "jpg";
				if(checkForFile(ext.data())){
					file = ext;
				}else{
					ext = file;
					ext += "jpeg";
					if(checkForFile(ext.data())){
						file = ext;
					}else{
						ext = file;
						ext += "bmp";
						file = ext;
					}
				}
			}
		}
		bg = env->addImage(driver->getTexture(file.data()), vector2d<s32>(0, 0), true);
	}
	bg->setScaleImage(true);
	bg->setMinSize(dimension2du(1030, 745));
	bg->setMaxSize(dimension2du(1030, 745));
// IGUIImage *fade will be a semi-transparent image on top of the background image so that text can
// be displayed more legibly on top of fade.
	IGUIImage *fade = env->addImage(driver->getTexture("ressystem/fade.png"), vector2d<s32>(0, 0));
	fade->setScaleImage(true);
	fade->setMinSize(dimension2du(1030, 745));
	fade->setMaxSize(dimension2du(1030, 745));
	fade->setColor(SColor(127, 40, 40, 40));
	IGUIFont *font = env->getFont("ressystem/superfont.xml");
// This xfpslimiter will tell the system to sleep after an appropraite amount of time has passed.
// That amount of time will correlate with the FPS variable in its class.
// FPS will be set to 30 (user probably won't be pressing a button and looking for change in value
// in less than 30 miliseconds time)
	xfpslimiter fps;
	fps.timer = device->getTimer();




	int res;
	unsigned char buf[65];
	#define MAX_STR 255
	wchar_t wstr[MAX_STR];
	hid_device *handle;
	int i;
	struct hid_device_info *devs, *cur_dev;
	//in order to stop getting [2013: unresolved..._hid_enumeration in _main], build the hidapi.lib
	devs = hid_enumerate(0x0, 0x0);
	cur_dev = devs;	
	int cur_pid, cur_vid = 0;
//list all currently connected HIDs
	while (cur_dev) {
		printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls",
			cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
		printf("\n");
		printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
		printf("  Product:      %ls\n", cur_dev->product_string);
		cout << "IDS (VID, PID): " << cur_dev->vendor_id << ", " << cur_dev->product_id << endl;
		printf("\n");
		cur_dev = cur_dev->next;
	}
	hid_free_enumeration(devs);

// now get user input and verify whether an hid with the specified vid and pid is currently connected
	SetForegroundWindow(GetConsoleWindow());
	bool quit = false;
	std::string input = "";
	int vid, pid = 0;
	while(!quit){
		cout << "\n\n\nPlease enter HID's VID and PID (separated by a space)\nor enter q to quit" << endl;
		getline(cin, input);
		if(input == "q"){
			quit = true;
		}else{
			std::string x = zgetstrto(input, 0, ' ');
			unsigned int length = x.length();
			length++;
			vid = intconvstr(x);
			if(x.length() == input.length()){
				cout << "did not find 2 values; invalid input." << endl;
				continue;
			}
			pid = intconvstr(zgetreststr(input, length));
			cout << "attempting to connect to device with VID(" << vid << ") and PID(" << pid << ")..." << endl;
			handle = hid_open(vid, pid, NULL);
			if(!handle){
				cout << "failed to connect to device (check the vid and pid)" << endl;
			}else{
				cout << "device successfully connected!\n\n\n" << endl;
				break;
			}
		}
	}
	if(quit){
		device->drop();
		return 0;
	}
	//handle = hid_open(1699, 3431, NULL); // SET THE DEVICE TO BE TRACKED HERE - VIP, PID
	if(!handle){
		cout << "FAILED TO CONNECT" << endl;
	}else{
		cout << "Opened connection with device '";
		res = hid_get_product_string(handle, wstr, MAX_STR);
	printf("Product String: %ls\n", wstr);
		cout << "'" << endl;
	}
	// Set the hid_read() function to be non-blocking.
	hid_set_nonblocking(handle, 1);
	buf[0] = 1;
	//res = hid_write(handle, buf, 65);
	
	res = hid_read(handle, buf, 65);
	cout << "========================================\nOriginals\n----------------------------" << endl;
	for(int xint = 0;xint < 65;xint++){
		cout << inttobin(unsigned int(buf[xint])) << " [" << xint << "] (" << int(buf[xint]) << ")" << endl;
	}
	cout << "=========================================" << endl;
	// Read a Feature Report from the device
	buf[0] = 0x2;
	res = hid_get_feature_report(handle, buf, sizeof(buf));

	// Print out the returned buffer.
	printf("Feature Report\n   ");
	for (i = 0; i < res; i++)
		printf("%02hhx ", buf[i]);
	printf("\n");

	// Send an Output report to toggle the LED (cmd 0x80)
	buf[0] = 0; // First byte is report number
	buf[1] = 0;
	buf[2] = 72;
	res = hid_write(handle, buf, 65);

	// Read requested state
	res = hid_read(handle, buf, 65);
	if (res < 0)
		printf("Unable to read()\n");

	// Print out the returned buffer.
	for (i = 0; i < res; i++)
		printf("buf[%d]: %d\n", i, buf[i]);
	cout << "read data amount: >>" << res << "<< str: '";
	{
		std::string str = "";
	for(int xint = 0;xint < res;xint++){
		str += buf[xint];
	}
	cout << str << "'" << endl;
	}
	array<std::wstring> recread;
	for(int xint = 0;xint < 8;xint++){
		std::wstring str = L"[";
		str += zgetintstr2(xint);
		str += L"]: ''";
		recread.insert(str.data(), recread.size());
	}
	std::string bufbuf = "";
	for(int xint = 0;xint < 65;xint++){
		bufbuf += buf[xint];
	}




// now the main loop
	bool space = false;
	bool esc = false;
	bool pup = false;
	bool pdown = false;
	unsigned int index_step = 0;
	unsigned int index_min = 0;
	unsigned int index_max = 20;
	std::wstring status = L"Device connected!\nCurrent index_step: 0";
	status += L"\nCurrent index_min: ";
	status += zgetintstr2(index_min);
	status += L"\nCurrent index_max: ";
	status += zgetintstr2(index_max);
	status += L"\nmouse wheel: ";
	status += zgetfstr2(receiver.MouseState.wheel);
	float lastwheel = 0;
	lastwheel = receiver.MouseState.wheel;
	while(device->run()){
		fps.nowTime = fps.timer->getTime();
		res = hid_read(handle, buf, 65);
		std::string str = "(";
		str += zgetintstr(res);
		str += "): '";
		for(int xint = 0;xint < res;xint++){
			str += buf[xint];
		}
		std::string tbuf = "";
		for(int xint = 0;xint < 65;xint++){
			tbuf += buf[xint];
		}
		str += "'";
		for(int xint = 1;xint < 8;xint++){
			recread[xint - 1] = recread[xint];
		}
		recread[7] = zconvstr(str);
		/**if(res > 0){
			cout << "Change occured: res = " << res << ", str = '" << buf << "'\n1(report): '" << buf[0] << "'" << endl;
			for(int xint = 0;xint < 65;xint++){
				if(bufbuf[xint] != tbuf[xint]){
					cout << "CHANGE AT: [" << xint << "] :\n";
					cout << inttobin(unsigned int((unsigned char)bufbuf[xint])) << " OLD ('";
					cout << unsigned int((unsigned char)bufbuf[xint]) << "')\n";
					cout << inttobin(unsigned int((unsigned char)tbuf[xint])) << " NEW ('";
					cout << unsigned int((unsigned char)tbuf[xint]) << "')" << endl;
				}
			}
			bufbuf = tbuf;
		}**/
		if(receiver.IsKeyDown(KEY_ESCAPE)){
			esc = true;
		}else if(esc){
			break;
		}
		if(receiver.MouseState.wheel != lastwheel){
			if(receiver.MouseState.wheel > lastwheel){
				pup = true;
			}else{
				pdown = true;
			}
			lastwheel = receiver.MouseState.wheel;
		}
		if(receiver.IsKeyDown(KEY_PRIOR)){
			pup = true;
		}else if(pup){
			index_step--;
			if(index_step > 3){
				index_step = 0;
			}
			index_min = (index_step * 20);
			index_max = index_min + 20;
			if(index_max > 64){
				index_max = 64;
			}
			status = L"Device connected!\nCurrent index_step: ";
			status += zgetintstr2(index_step);
			status += L"\nCurrent index_min: ";
			status += zgetintstr2(index_min);
			status += L"\nCurrent index_max: ";
			status += zgetintstr2(index_max);
			status += L"\nmouse wheel: ";
			status += zgetfstr2(receiver.MouseState.wheel);
			pup = false;
		}
		if(receiver.IsKeyDown(KEY_NEXT)){
			pdown = true;
		}else if(pdown){
			index_step++;
			if(index_step > 3){
				index_step = 3;
			}
			index_min = (index_step * 20);
			index_max = index_min + 20;
			if(index_max > 65){
				index_max = 65;
			}
			status = L"Device connected!\nCurrent index_step: ";
			status += zgetintstr2(index_step);
			status += L"\nCurrent index_min: ";
			status += zgetintstr2(index_min);
			status += L"\nCurrent index_max: ";
			status += zgetintstr2(index_max);
			status += L"\nmouse wheel: ";
			status += zgetfstr2(receiver.MouseState.wheel);
			pdown = false;
		}
		if(receiver.IsKeyDown(KEY_SPACE)){
			space = true;
		}else if(space){ // prints all 65 values to console
			cout << "=====================================\nPreseent\n------------------------------" << endl;
			for(int xint = 0;xint < 65;xint++){
				cout << inttobin(unsigned int(tbuf[xint])) << " [" << xint << "] (" << int(tbuf[xint]) << ")" << endl;
			}
			cout << "======================================" << endl;
			space = false;
		}
		driver->beginScene(false, false, SColor(255, 255, 255, 255));
		env->drawAll();
		font->draw(status.data(), rect<s32>(0, 505, 200, 600), SColor(255, 247, 255, 38));
		for(int xint = 0;xint < 8;xint++){
			font->draw(recread[xint].data(), rect<s32>(0, 0 + (xint * 30),
				driver->getScreenSize().Width, 20 + (xint * 30)), SColor(255, 247, 255, 38), false, false);
			/**std::wstring str = L"[";
			str += zgetintstr2(xint);
			str += L"] ";
			str += zconvstr(inttobin(buf[xint]));
			str += L" (";
			str += zgetintstr2(unsigned int(buf[xint]));
			str += L")";
			env->getSkin()->getFont(EGDF_DEFAULT)->draw(str.data(), rect<s32>(450, 0 + (xint * 30),
				driver->getScreenSize().Width, 20 + (xint * 30)), SColor(255, 247, 255, 38), false, false);**/
		}
		for(unsigned int xint = index_min, pos = 0;xint < index_max;xint++,pos++){
			std::wstring str = L"[";
			str += zgetintstr2(xint);
			str += L"] ";
			str += zconvstr(inttobin(buf[xint]));
			str += L" (";
			str += zgetintstr2(unsigned int(buf[xint]));
			str += L")";
			font->draw(str.data(), rect<s32>(450, 0 + (pos * 30),
				driver->getScreenSize().Width, 20 + (pos * 30)), SColor(255, 247, 255, 38), false, false);
		}
		driver->endScene();
		fps.sdlwait();
	}
	device->drop();
}