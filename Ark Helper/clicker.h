#pragma once
#include <Windows.h>

class Clicker {
public:

	Clicker() = default;
	Clicker(HWND h_windows_handle);

	virtual void Click() const {};
	
	virtual void PressingKeyUntilHotKeyInsideApp(bool& is_enable) const {};

	void SimulateKeyPress(WORD keyCode);

	void SimulateCtrlV();

	HWND GetWindowHandle() const;

	void SetDelay(size_t delay);

	size_t GetDelay() const;

private:

	size_t delay_ = 200;
	HWND h_windows_handle_ = nullptr;

	virtual void ClickInsideApplication() const {}

};//class Clicker

class LeftMouseClicker : public Clicker {
public:

	LeftMouseClicker() = default;

	LeftMouseClicker(HWND h_windows_handle);

	void Click() const override;

	void PressingKeyUntilHotKeyInsideApp(bool& is_enable) const override;

private:

	void ClickInsideApplication() const override;
}; //class LeftMouseClicker

class RightMouseClicker : public Clicker {
public:

	RightMouseClicker() = default;

	RightMouseClicker(HWND h_windows_handle);

	void Click() const override;

	void PressingKeyUntilHotKeyInsideApp(bool& is_enable) const override;

private:

	void ClickInsideApplication() const override;

}; //class RightMouseClicker

class SpaceButtonClicker : public Clicker {
public:
	SpaceButtonClicker() = default;

	SpaceButtonClicker(HWND h_windows_handle);

	void Click() const override;

	void PressingKeyUntilHotKeyInsideApp(bool& is_enable) const override;

private:

	void ClickInsideApplication() const override;

}; //class SpaceButtonClicker