#include "webhook.h"

#include <vector>
#include <opencv2/opencv.hpp>
#include <atlimage.h> // for CImage
#include <fstream>

using namespace std::literals;

DiscordWebhook::DiscordWebhook() {

    std::ifstream input;
    input.open(webhook_path_);
    if (!input.is_open()) {
        std::cout << webhook_path_ << " file was not created or opened"s << std::endl;
        return;
    }
    
    std::string line;
    std::size_t counter = 0;
    while (std::getline(input, line)) {
        
        if (counter == 1) {
            webhook_path_ = line;
            input.close();
            return;
        }
        ++counter;
    }
    input.close();
}

DiscordWebhook::DiscordWebhook(std::string webhook_path) : webhook_path_(webhook_path) {}

void DiscordWebhook::SendImage(int x, int y, int width, int height,const std::string& image_path){

    HBITMAP h_bitmap = CaptureScreenshotForWebhook(x, y, width, height);
    std::vector<BYTE> buf;
    IStream* stream = NULL;
    HRESULT hr = CreateStreamOnHGlobal(0, TRUE, &stream);
    CImage image;
    image.Attach(h_bitmap);
    image.Save(stream, Gdiplus::ImageFormatPNG);
    ULARGE_INTEGER liSize;
    IStream_Size(stream, &liSize);
    DWORD len = liSize.LowPart;
    IStream_Reset(stream);
    buf.resize(len);
    IStream_Read(stream, &buf[0], len);
    stream->Release();
    std::fstream fi;
    fi.open(image_path, std::fstream::binary | std::fstream::out);
    fi.write(reinterpret_cast<const char*>(&buf[0]), buf.size() * sizeof(BYTE));
    fi.close();

    // Отправка изображения в Discord через вебхук
    // Send an image to Discord via webhook
    const std::string command = cmd_for_image_message_ + image_path + "\" " + GetWebHookUrl() + " > NUL";
    system(command.c_str());

    DeleteObject(h_bitmap); // Освобождение ресурсов битмапа  || Freeing up bitmap resources
}

int DiscordWebhook::system_no_output(std::string command) noexcept{
    command.insert(0, "/C ");

    SHELLEXECUTEINFOA sh_exec_info = { 0 };
    sh_exec_info.cbSize = sizeof(SHELLEXECUTEINFO);
    sh_exec_info.fMask = SEE_MASK_NOCLOSEPROCESS;
    sh_exec_info.hwnd = NULL;
    sh_exec_info.lpVerb = NULL;
    sh_exec_info.lpFile = "cmd.exe";
    sh_exec_info.lpParameters = command.c_str();
    sh_exec_info.lpDirectory = NULL;
    sh_exec_info.nShow = SW_HIDE;
    sh_exec_info.hInstApp = NULL;

    if (ShellExecuteExA(&sh_exec_info) == FALSE) {
        return -1;
    }
    if (sh_exec_info.hProcess != nullptr) {
        WaitForSingleObject(sh_exec_info.hProcess, INFINITE);
        DWORD rv;
        GetExitCodeProcess(sh_exec_info.hProcess, &rv);
        CloseHandle(sh_exec_info.hProcess);
        return rv;
    }
    else {
        return -1;
    }
}

std::string DiscordWebhook::GetWebHookUrl() const{
    return webhook_path_;
}

void DiscordWebhook::SendText(std::string message) {
    std::string command = cmd_1_for_text_message_ + message + cmd_2_for_text_message_ + GetWebHookUrl();
    system_no_output(command.c_str());
}

