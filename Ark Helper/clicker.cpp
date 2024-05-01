#include <iostream>

#include "string_processing.h"
#include "clicker.h"

Clicker::Clicker(HWND h_windows_handle) : h_windows_handle_(h_windows_handle) {}

LeftMouseClicker::LeftMouseClicker(HWND h_windows_handle) : Clicker(h_windows_handle) {}

RightMouseClicker::RightMouseClicker(HWND h_windows_handle) : Clicker(h_windows_handle) {}

SpaceButtonClicker::SpaceButtonClicker(HWND h_windows_handle) : Clicker(h_windows_handle) {}

HWND Clicker::GetWindowHandle() const{
	return h_windows_handle_;
}

void Clicker::SetDelay(size_t delay){
	delay_ = delay;
}

size_t Clicker::GetDelay() const{
	return delay_;
}

void RightMouseClicker::Click() const{
	INPUT input{};
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	SendInput(1, &input, sizeof(INPUT));
	input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	SendInput(1, &input, sizeof(input));
}

void LeftMouseClicker::Click() const{
	INPUT input{};
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &input, sizeof(INPUT));
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(input));
}

void SpaceButtonClicker::Click() const {
	INPUT input{};
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = VK_SPACE; 
	input.ki.dwFlags = 0; 
	SendInput(1, &input, sizeof(INPUT));
	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));
}

void LeftMouseClicker::ClickInsideApplication() const{

	RECT rect;
	GetWindowRect(Clicker::GetWindowHandle(), &rect);
	int x = (rect.left + rect.right) / 2;
	int y = (rect.top + rect.bottom) / 2;

	PostMessage(Clicker::GetWindowHandle(), WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x, y));
	PostMessage(Clicker::GetWindowHandle(), WM_LBUTTONUP, 0, MAKELPARAM(x, y));
}

void RightMouseClicker::ClickInsideApplication() const {
	RECT rect;
	GetWindowRect(Clicker::GetWindowHandle(), &rect);
	int x = (rect.left + rect.right) / 2;
	int y = (rect.top + rect.bottom) / 2;

	PostMessage(Clicker::GetWindowHandle(), WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(x, y));
	PostMessage(Clicker::GetWindowHandle(), WM_RBUTTONUP, 0, MAKELPARAM(x, y));
}

void SpaceButtonClicker::ClickInsideApplication() const {
	PostMessage(Clicker::GetWindowHandle(), WM_KEYDOWN, VK_SPACE, 0);
	PostMessage(Clicker::GetWindowHandle(), WM_KEYUP, VK_SPACE, 0);
}

void LeftMouseClicker::PressingKeyUntilHotKeyInsideApp(bool& is_enable) const{

	Beep(1000, 800);
	is_enable = true;
	while (is_enable) {
		ClickInsideApplication();
		Sleep(Clicker::GetDelay());
	}
}

void RightMouseClicker::PressingKeyUntilHotKeyInsideApp(bool& is_enable) const{

	Beep(1500, 800);
	is_enable = true;
	while (is_enable) {
		ClickInsideApplication();
		Sleep(Clicker::GetDelay());
	}
}

void SpaceButtonClicker::PressingKeyUntilHotKeyInsideApp(bool& is_enable) const{

	Beep(2000, 800);
	is_enable = true;
	while (is_enable) {
		ClickInsideApplication();
		Sleep(Clicker::GetDelay());
	}
}
