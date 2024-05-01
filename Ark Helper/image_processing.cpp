#include <cstdio> // for FILE
#include <iostream>
#include <tesseract/baseapi.h> //read image
#include <leptonica/allheaders.h> // read image

#include "image_processing.h"

// читаем текст и изображения
// read text and images
std::string ReadImage(const std::string& image_path) noexcept {
    tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();
    api->Init(NULL, "eng");

    Pix* image = pixRead(image_path.c_str());

    api->SetImage(image);

    char* text = api->GetUTF8Text();

    std::string result = text;

    api->End();
    delete[] text;
    pixDestroy(&image);
    return result;
}

HBITMAP CaptureScreenshotUPD(int x, int y, int width, int height) noexcept {
    HDC hdcScreen = GetDC(NULL);
    HDC hdcCapture = CreateCompatibleDC(hdcScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
    SelectObject(hdcCapture, hBitmap);
    BitBlt(hdcCapture, 0, 0, width, height, hdcScreen, x, y, SRCCOPY);
    ReleaseDC(NULL, hdcScreen);
    DeleteDC(hdcCapture);
    return hBitmap;
}

void CaptureScreen(const char* save_path, int x, int y, int width, int height) noexcept {
    //RU Получаем хэндл главного окна экрана
    //EN We get the handle of the main screen window
    HWND hDesktopWnd = GetDesktopWindow();

    // Получаем контекст устройства для экрана
    // Getting the device context for the screen
    HDC hDesktopDC = GetDC(hDesktopWnd);

    // Создаем битмап для сохранения скриншота
    // Creating a bitmap to save a screenshot
    HDC hCaptureDC = CreateCompatibleDC(hDesktopDC);
    HBITMAP hCaptureBitmap = CreateCompatibleBitmap(hDesktopDC, width, height);
    SelectObject(hCaptureDC, hCaptureBitmap);

    // Копируем изображение экрана в битмап
    // Copying the screen image to a bitmap
    BitBlt(hCaptureDC, 0, 0, width, height, hDesktopDC, x, y, SRCCOPY);

    // Сохраняем битмап в файл формата BMP (24 бита на пиксель)
    // Save the bitmap to a BMP file(24 bits per pixel)
    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24;  // изменено с 32 на 24 бита  || changed from 32 to 24 bits
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = 0;
    bmi.bmiHeader.biXPelsPerMeter = 0;
    bmi.bmiHeader.biYPelsPerMeter = 0;
    bmi.bmiHeader.biClrUsed = 0;
    bmi.bmiHeader.biClrImportant = 0;

    // Открываем файл для сохранения скриншота
    // Open the file to save the screenshot
    FILE* file;
    if (fopen_s(&file, save_path, "wb") != 0) {
        std::cerr << "Ошибка: Невозможно открыть файл для сохранения скриншота.\nError: The file to save the screenshot cannot be opened." << std::endl;
        DeleteDC(hCaptureDC);
        DeleteObject(hCaptureBitmap);
        ReleaseDC(hDesktopWnd, hDesktopDC);
        return;
    }

    // Записываем заголовок файла BMP
    // Write the header of the BMP file
    BITMAPFILEHEADER bmfh = { 0 };
    bmfh.bfType = 0x4D42;
    bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bmfh.bfSize = bmfh.bfOffBits + width * height * 3;  // изменено с 4 на 3 байта на пиксель  || changed from 4 to 3 bytes per pixel
    fwrite(&bmfh, sizeof(BITMAPFILEHEADER), 1, file);

    // Записываем информацию о изображении
    // Write information about the image
    fwrite(&bmi.bmiHeader, sizeof(BITMAPINFOHEADER), 1, file);

    // Записываем данные пикселей изображения
    // Write image pixel data
    BYTE* pData = new BYTE[width * height * 3];  // изменено с 4 на 3 байта на пиксель  || changed from 4 to 3 bytes per pixel
    GetDIBits(hCaptureDC, hCaptureBitmap, 0, height, pData, &bmi, DIB_RGB_COLORS);
    fwrite(pData, sizeof(BYTE), width * height * 3, file);  // изменено с 4 на 3 байта на пиксель  || changed from 4 to 3 bytes per pixel

    // Закрываем файл и освобождаем ресурсы
    // Close the file and release resources
    fclose(file);
    delete[] pData;
    DeleteDC(hCaptureDC);
    DeleteObject(hCaptureBitmap);
    ReleaseDC(hDesktopWnd, hDesktopDC);
}