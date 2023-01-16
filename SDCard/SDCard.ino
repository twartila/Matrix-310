/*
https://github.com/espressif/arduino-esp32/blob/master/libraries/SD/examples/SD_Test/SD_Test.

listDir
createDir
removeDir

readFile
writeFile
appendFile
renameFile
deleteFile
testFileIO

test_sd
*/

//The SD library allows for reading from and writing to SD cards, the library also include "FS.h" and "SPI.h".
#include <SD.h>
#include "src/Artila-Matrix310.h"
//HSPI, SPI bus mapped to pins 12 - 15
SPIClass spi = SPIClass();

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){//has next file
        //Is a directory 
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        }
        //Is a file
        else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void createDir(fs::FS &fs, const char * path){
    Serial.printf("\nCreating Dir\n: %s", path);
    if(fs.mkdir(path)){
        Serial.println("Dir created\n");
    } else {
        Serial.println("mkdir failed");
    }
}

void removeDir(fs::FS &fs, const char * path){
    Serial.printf("Removing Dir: %s\n", path);
    if(fs.rmdir(path)){
        Serial.println("Dir removed\n");
    } else {
        Serial.println("rmdir failed");
    }
}

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);//file write
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written\n");
    } else {
        Serial.println("Write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);//file append
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended\n");
    } else {
        Serial.println("Append failed");
    }
    file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
    Serial.printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2)) {
        Serial.println("File renamed\n");
    } else {
        Serial.println("Rename failed");
    }
}

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\n", path);
    if(fs.remove(path)){
        Serial.println("File deleted\n");
    } else {
        Serial.println("Delete failed");
    }
}

void testFileIO(fs::FS &fs, const char * path){
    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if(file){
        len = file.size();
        size_t flen = len;
        start = millis();
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        Serial.printf("%u bytes read for %u ms\n", flen, end);
        file.close();
    } else {
        Serial.println("Failed to open file for reading");
    }


    file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }

    size_t i;
    start = millis();
    for(i=0; i<2048; i++){
        file.write(buf, 512);
    }
    end = millis() - start;
    Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
    file.close();
}

void test_sd(){
    if(!SD.begin(SD_CS, spi)){
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }
    //Show SD card type
    Serial.print("SD Card Type: ");
    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD){
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }
    //Show SD card size
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
    Serial.println("\n------------------------------");
    //Create directory
    listDir(SD, "/", 0);
    createDir(SD, "/mydir");
    listDir(SD, "/", 0);
    Serial.println("\n------------------------------");
    //Remove directory
    removeDir(SD, "/mydir");
    listDir(SD, "/", 2);
    Serial.println("\n------------------------------");
    //Write and read file
    writeFile(SD, "/hello.txt", "Hello ");//Overwrite a file
    appendFile(SD, "/hello.txt", "World!\n");//Append to a file instead of overwriting
    readFile(SD, "/hello.txt");
    Serial.println("\n------------------------------");
    //Delete and rename a file
    deleteFile(SD, "/foo.txt");
    renameFile(SD, "/hello.txt", "/foo.txt");
    readFile(SD, "/foo.txt");
    Serial.println("\n------------------------------");
    //test file IO
    testFileIO(SD, "/test.txt");
    Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
    Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}

void setRreadyLed(int state){
    if(state) { // LED ON
        setLED(LED_READY, HIGH);
    }
    else { // LED OFF
        setLED(LED_READY, LOW);
    }
}

void setLED(int pin, int status){
    digitalWrite(pin, status);
}

void initGPIO()
{
    pinMode(DO1, OUTPUT);
    pinMode(DI1, INPUT);
    pinMode(DI2, INPUT);
    pinMode(SD_CS, OUTPUT);
    pinMode(COM1_RTS, OUTPUT);
    pinMode(LED_WIFI, OUTPUT);
    pinMode(LED_READY, OUTPUT);
    
    pinMode(SW_1, INPUT);
    pinMode(SW_2, INPUT);
    pinMode(SW_4, INPUT);
    
    // make sure LED OFF, if possible
    // setRreadyLed(1);
    // digitalWrite(LED_WIFI, LOW);
}

void setup()
{
    initGPIO();
    Serial.begin(115200);
    while (!Serial);
    Serial.println("\n------------------------------");
    Serial.print("Initializing SD card...\n");
    spi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);//HSPI
    
    setRreadyLed(1);
    test_sd();//sd card test
}


void loop(void) {}
