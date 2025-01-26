#ifndef SDCARD_HPP
#define SDCARD_HPP

#include <Arduino.h>
#include <SD.h>

class SDCard
{
private:
    File _file;
    bool _isFileOpen;

public:
    String FILE_NAME;
    void begin();
    void setFileName(String fileName){FILE_NAME = fileName;};
    void createFile(String fileName);
    void writeData(String fileName, String data);
    void printData(String fileName);
    void deleteFile(String fileName);
    void deleteAllFiles();
};

#endif // SD_HPP