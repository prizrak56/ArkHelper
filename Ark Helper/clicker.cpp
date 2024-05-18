#include "clicker.h"

namespace clicker {
// ----------------------------------- [Clicker] Realization --------------------------
//                                                                                    +
//                                                                                    + ----------------
// ------------------------------------------------------------------------------------ Clicker Itself +

	Clicker::Clicker(HWND h_windows_handle) noexcept : h_windows_handle_(h_windows_handle) {}

	std::size_t Clicker::GetDelay() const noexcept {
		return delay_;
	}

	HWND Clicker::GetWindowHandle() const noexcept {
		return h_windows_handle_;
	}

	void Clicker::SetDelay(std::size_t delay) noexcept {
		delay_ = delay;
	}

//                                                                                    + --------------------
// ------------------------------------------------------------------------------------ Left Mouse Clicker +

	LeftMouseClicker::LeftMouseClicker(HWND h_windows_handle) noexcept : Clicker(h_windows_handle) {}

	void LeftMouseClicker::Click() const {
		INPUT input{};
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		SendInput(1, &input, sizeof(INPUT));
		input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		SendInput(1, &input, sizeof(input));
	}

	void LeftMouseClicker::ClickInsideApplication() const {
		RECT rect;
		GetWindowRect(Clicker::GetWindowHandle(), &rect);
		int x = (rect.left + rect.right) / 2;
		int y = (rect.top + rect.bottom) / 2;

		PostMessage(Clicker::GetWindowHandle(), WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x, y));
		PostMessage(Clicker::GetWindowHandle(), WM_LBUTTONUP, 0, MAKELPARAM(x, y));
	}

	void LeftMouseClicker::PressKeyUntilHotKeyIsInputed(bool& is_enable) const {
		Beep(1000, 800);
		is_enable = true;
		while (is_enable) {
			ClickInsideApplication();
			Sleep(Clicker::GetDelay());
		}
	}

//                                                                                    + ---------------------
// ------------------------------------------------------------------------------------ Right Mouse Clicker +

	RightMouseClicker::RightMouseClicker(HWND h_windows_handle) noexcept : Clicker(h_windows_handle) {}

	void RightMouseClicker::Click() const {
		INPUT input{};
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
		SendInput(1, &input, sizeof(INPUT));
		input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
		SendInput(1, &input, sizeof(input));
	}

	void RightMouseClicker::ClickInsideApplication() const {
		RECT rect;
		GetWindowRect(Clicker::GetWindowHandle(), &rect);
		int x = (rect.left + rect.right) / 2;
		int y = (rect.top + rect.bottom) / 2;

		PostMessage(Clicker::GetWindowHandle(), WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(x, y));
		PostMessage(Clicker::GetWindowHandle(), WM_RBUTTONUP, 0, MAKELPARAM(x, y));
	}

	void RightMouseClicker::PressKeyUntilHotKeyIsInputed(bool& is_enable) const {
		Beep(1500, 800);
		is_enable = true;
		while (is_enable) {
			ClickInsideApplication();
			Sleep(Clicker::GetDelay());
		}
	}

//                                                                                    + ----------------------
// ------------------------------------------------------------------------------------ Space Button Clicker +

	SpaceButtonClicker::SpaceButtonClicker(HWND h_windows_handle) noexcept : Clicker(h_windows_handle) {}

	void SpaceButtonClicker::Click() const {
		INPUT input{};
		input.type = INPUT_KEYBOARD;
		input.ki.wVk = VK_SPACE;
		input.ki.dwFlags = 0;
		SendInput(1, &input, sizeof(INPUT));
		input.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &input, sizeof(INPUT));
	}

	void SpaceButtonClicker::ClickInsideApplication() const {
		PostMessage(Clicker::GetWindowHandle(), WM_KEYDOWN, VK_SPACE, 0);
		PostMessage(Clicker::GetWindowHandle(), WM_KEYUP, VK_SPACE, 0);
	}

	void SpaceButtonClicker::PressKeyUntilHotKeyIsInputed(bool& is_enable) const {
		Beep(2000, 800);
		is_enable = true;
		while (is_enable) {
			ClickInsideApplication();
			Sleep(Clicker::GetDelay());
		}
	}
} // namespace clicker