#pragma once

#include <cstddef>
#include <Windows.h>

namespace clicker {
// ------------------------------------ [Clicker] Definition --------------------------
//                                                                                    +
//                                                                                    + ----------------
// ------------------------------------------------------------------------------------ Clicker Itself +

	class Clicker {
	public:
		Clicker() noexcept = default;
		Clicker(HWND h_windows_handle) noexcept;
		std::size_t GetDelay() const noexcept;
		HWND GetWindowHandle() const noexcept;
		void SetDelay(std::size_t delay) noexcept;

		virtual void Click() const = 0;
		virtual void PressKeyUntilHotKeyIsInputed(bool& is_enable) const = 0;

	private:
		std::size_t delay_ = 200;
		HWND h_windows_handle_ = nullptr;

		virtual void ClickInsideApplication() const = 0;
	};

//
// 
//                                                                                    + --------------------
// ------------------------------------------------------------------------------------ Left Mouse Clicker +

	class LeftMouseClicker final : public Clicker {
	public:
		LeftMouseClicker() noexcept = default;
		LeftMouseClicker(HWND h_windows_handle) noexcept;
		void Click() const override;
		void PressKeyUntilHotKeyIsInputed(bool& is_enable) const override;

	private:
		void ClickInsideApplication() const override;
	};

//
// 
//                                                                                    + ---------------------
// ------------------------------------------------------------------------------------ Right Mouse Clicker +

	class RightMouseClicker final : public Clicker {
	public:
		RightMouseClicker() noexcept = default;
		RightMouseClicker(HWND h_windows_handle) noexcept;
		void Click() const override;
		void PressKeyUntilHotKeyIsInputed(bool& is_enable) const override;

	private:
		void ClickInsideApplication() const override;
	};

//
// 
//                                                                                    + ----------------------
// ------------------------------------------------------------------------------------ Space Button Clicker +

	class SpaceButtonClicker final : public Clicker {
	public:
		SpaceButtonClicker() noexcept = default;
		SpaceButtonClicker(HWND h_windows_handle) noexcept;
		void Click() const override;
		void PressKeyUntilHotKeyIsInputed(bool& is_enable) const override;

	private:
		void ClickInsideApplication() const override;
	};
} // namespace clicker