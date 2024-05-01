#include "webhook.h"

#include <vector>
#include <opencv2/opencv.hpp>
#include <atlimage.h> // for CImage
#include <fstream>
//#include <curl/curl.h>

DiscordWebhook::DiscordWebhook(std::string webhook_path) : webhook_path_(webhook_path) {}

void DiscordWebhook::SendImage(int x, int y, int width, int height,const std::string& image_path){

    HBITMAP hBitmap = CaptureScreenshotForWebhook(x, y, width, height);
    std::vector<BYTE> buf;
    IStream* stream = NULL;
    HRESULT hr = CreateStreamOnHGlobal(0, TRUE, &stream);
    CImage image;
    image.Attach(hBitmap);
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
    //Gluing();
    system(command.c_str());

    DeleteObject(hBitmap); // Освобождение ресурсов битмапа  || Freeing up bitmap resources

}

int DiscordWebhook::system_no_output(std::string command) noexcept{
    command.insert(0, "/C ");

    SHELLEXECUTEINFOA ShExecInfo = { 0 };
    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo.hwnd = NULL;
    ShExecInfo.lpVerb = NULL;
    ShExecInfo.lpFile = "cmd.exe";
    ShExecInfo.lpParameters = command.c_str();
    ShExecInfo.lpDirectory = NULL;
    ShExecInfo.nShow = SW_HIDE;
    ShExecInfo.hInstApp = NULL;

    if (ShellExecuteExA(&ShExecInfo) == FALSE) {
        return -1;
    }

    if (ShExecInfo.hProcess != nullptr) {
        WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
        DWORD rv;
        GetExitCodeProcess(ShExecInfo.hProcess, &rv);
        CloseHandle(ShExecInfo.hProcess);
        return rv;
    }
    else {
        return -1;
    }
}

std::string DiscordWebhook::GetWebHookUrl() const{
    std::ifstream webhook_url;
    webhook_url.open(webhook_path_);
    std::string result;
    std::getline(webhook_url, result);
    return result;
}

void DiscordWebhook::SendText(std::string message) {

    std::string command = cmd_1_for_text_message_ + message + cmd_2_for_text_message_ + GetWebHookUrl();

    system_no_output(command.c_str());
}

