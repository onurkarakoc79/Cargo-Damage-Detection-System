#include "SDCard.hpp"
#include "ESP32_Pins.hpp"

void SDCard::begin()
{
    Serial.print("Initializing SD card...");
    
    if (!SD.begin(SD_CS_PIN))
    {
        Serial.println("Initialization failed!");
        while (1)
        {
            if(SD.begin(SD_CS_PIN))
            {
                break;
            }
        };
        Serial.println("Initialization done.");
    }
    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE)
    {
        Serial.println("No SD card attached");
        return;
    }
    Serial.print("SD Card Type: ");
    if (cardType == CARD_MMC)
    {
        Serial.println("MMC");
    }
    else if (cardType == CARD_SD)
    {
        Serial.println("SDSC");
    }
    else if (cardType == CARD_SDHC)
    {
        Serial.println("SDHC");
    }
    else
    {
        Serial.println("UNKNOWN");
    }
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
}

void SDCard::createFile(String fileName)
{
    _file = SD.open(fileName, FILE_WRITE);
    if (_file)
    {
        _file.close();
        Serial.println("File created successfully");
    }
    else
    {
        Serial.println("File creation failed");
    }
}

void SDCard::writeData(String fileName, String data)
{
    _file = SD.open(fileName, FILE_APPEND);
    if (_file)
    {
        _file.println(data);
        _file.close();
        Serial.println("Data written successfully");
    }
    else
    {
        Serial.println("Data writing failed");
        Serial.println("You may need to create a file first");
    }
}

void SDCard::printData(String fileName)
{
    _file = SD.open(fileName);
    if (_file)
    {
        while (_file.available())
        {
            Serial.write(_file.read());
        }
        _file.close();
    }
    else
    {
        Serial.println("File opening failed");
    }
}

void SDCard::deleteFile(String fileName)
{
    if (SD.remove(fileName))
    {
        Serial.println("File deleted successfully");
    }
    else
    {
        Serial.println("File deletion failed");
    }
}

void SDCard::deleteAllFiles()
{
    File root = SD.open("/");
    while (true)
    {
        File entry = root.openNextFile();
        if (!entry)
        {
            break;
        }
        if (entry.isDirectory())
        {
            entry.close();
            continue;
        }
        Serial.print("Deleting file: ");
        Serial.println(entry.name());
        if (SD.remove(entry.name()))
        {
            Serial.println("File deleted successfully");
        }
        else
        {
            Serial.println("File deletion failed");
        }
        entry.close();
    }
    root.close();
}

