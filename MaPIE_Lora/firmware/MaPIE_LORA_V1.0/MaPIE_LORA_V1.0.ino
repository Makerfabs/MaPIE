/*

使用同一个硬件SPI带LORA和SD卡

// SPI
#define PIN_SPI_MISO  (16u)
#define PIN_SPI_MOSI  (19u)
#define PIN_SPI_SCK   (18u)
#define PIN_SPI_SS    (17u)

Arduino官方只声明了一个SPI库
公用同一个SPI时必须先调用RP2040_SD的库，会初始化SPI
然后再用Lora库，并且不初始化SPI，不然会报错

然后必须手动控制CS开关，尚不清楚是哪个库不能这样用。

库版本
Using library SPI in folder: C:\Users\86156\AppData\Local\Arduino15\packages\arduino\hardware\mbed_rp2040\3.1.1\libraries\SPI (legacy)
Using library RP2040_SD at version 1.0.1 in folder: C:\Users\86156\Documents\Arduino\libraries\RP2040_SD
Using library RadioLib at version 4.6.0 in folder: C:\Users\86156\Documents\Arduino\libraries\RadioLib


*/

#include <SPI.h>
#include <RP2040_SD.h>
#include <RadioLib.h>

#define LORA_CS 17
#define SD_CS 9

// SPI
#define PIN_SPI_MISO (16u)
#define PIN_SPI_MOSI (19u)
#define PIN_SPI_SCK (18u)
#define PIN_SPI_SS (17u)

#define LORA_DIO0 21
#define LORA_DIO1 22
#define LORA_DIO2 23
#define LORA_DIO5 24
#define LORA_RST 20

#define FREQUENCY 434.0
#define BANDWIDTH 125.0
#define SPREADING_FACTOR 9
#define CODING_RATE 7
#define OUTPUT_POWER 10
#define PREAMBLE_LEN 8
#define GAIN 0

// SX1276 radio = new Module(LORA_CS, LORA_DIO0, LORA_RST, LORA_DIO1);
SX1276 radio = new Module(LORA_CS, LORA_DIO0, LORA_RST, LORA_DIO1, SPI, SPISettings());
File root;
long unsigned int runtime = 0;

#define LORA_CS_OFF digitalWrite(LORA_CS, HIGH)
#define LORA_CS_ON digitalWrite(LORA_CS, LOW)
#define SD_CS_OFF digitalWrite(SD_CS, HIGH)
#define SD_CS_ON digitalWrite(SD_CS, LOW)

void setup(void)
{
    Serial.begin(115200);

    pinMode(LORA_CS, OUTPUT);
    pinMode(SD_CS, OUTPUT);

    pinMode(PIN_SPI_MISO, INPUT);
    pinMode(PIN_SPI_MOSI, OUTPUT);
    pinMode(PIN_SPI_SCK, OUTPUT);

    LORA_CS_OFF;
    SD_CS_OFF;
    // SPI.end();

    delay(3000);
    Serial.println(F("Hello! Lora Test"));

    SD_CS_ON;

    while (1)
    {
        if (!SD.begin(SD_CS))
        {
            SD.end();
            Serial.println("SD init failed!");
            delay(1000);
            continue;
        }
        else
        {
            Serial.println("SD init done.");
            break;
        }
    }

    sd_test();
    SD_CS_OFF;

    LORA_CS_ON;
    int state = radio.begin(FREQUENCY, BANDWIDTH, SPREADING_FACTOR, CODING_RATE, SX127X_SYNC_WORD, OUTPUT_POWER, PREAMBLE_LEN, GAIN);

    if (state == ERR_NONE)
    {
        Serial.println(F("success!"));
    }
    else
    {
        Serial.print(F("failed, code "));
        Serial.println(state);
        while (true)
            ;
    }

    runtime = millis();
}

void loop()
{
    lora_node_general();
    if ((millis() - runtime) > 10000)
    {
        LORA_CS_OFF;
        SD_CS_ON;

        sd_test();
        runtime = millis();

        SD_CS_OFF;
        LORA_CS_ON;
    }
}

void lora_node_general()
{
    Serial.print(F("[SX1278] Waiting for incoming transmission ... "));

    String str;
    int state = radio.receive(str);

    if (state == ERR_NONE)
    {
        // packet was successfully received
        Serial.println(F("success!"));

        // print the data of the packet
        Serial.print(F("[SX1278] Data:\t\t\t"));
        Serial.println(str);

        // print the RSSI (Received Signal Strength Indicator)
        // of the last received packet
        Serial.print(F("[SX1278] RSSI:\t\t\t"));
        Serial.print(radio.getRSSI());
        Serial.println(F(" dBm"));

        // print the SNR (Signal-to-Noise Ratio)
        // of the last received packet
        Serial.print(F("[SX1278] SNR:\t\t\t"));
        Serial.print(radio.getSNR());
        Serial.println(F(" dB"));

        // print frequency error
        // of the last received packet
        Serial.print(F("[SX1278] Frequency error:\t"));
        Serial.print(radio.getFrequencyError());
        Serial.println(F(" Hz"));
    }
    else if (state == ERR_RX_TIMEOUT)
    {
        // timeout occurred while waiting for a packet
        Serial.println(F("timeout!"));
    }
    else if (state == ERR_CRC_MISMATCH)
    {
        // packet was received, but is malformed
        Serial.println(F("CRC error!"));
    }
    else
    {
        // some other error occurred
        Serial.print(F("failed, code "));
        Serial.println(state);
    }
}

void sd_test()
{
    String fileName = "datalog.txt";
    String fileName1 = "newdata.txt";

    root = SD.open("/");

    printDirectory(root, 0);

    Serial.println("Print Directory done!");

    File myWFile = SD.open(fileName, FILE_WRITE);

    if (myWFile)
    {
        char writeData[] = "Testing RP2040 SD";

        myWFile.write((uint8_t *)&writeData, sizeof(writeData));

        Serial.print("writeData = ");
        Serial.println(writeData);

        myWFile.close();
    }
    else
    {
        Serial.print("Error open for writing ");
        Serial.println(fileName);
    }

    myWFile = SD.open(fileName1, FILE_WRITE);

    if (myWFile)
    {
        char writeData[] = "Testing RP2040 SD";

        // myWFile.seek(0, (SeekMode) SEEK_END);

        myWFile.write((uint8_t *)&writeData, sizeof(writeData));

        myWFile.close();
    }
    else
    {
        Serial.print("Error open for writing ");
        Serial.println(fileName1);
    }

    // OK here to read file
    File myRFile = SD.open(fileName, FILE_READ);

    if (myRFile)
    {
        char readData[64] = "\0";

        // myWFile.seek(0, (SeekMode) SEEK_END);

        myRFile.read((uint8_t *)&readData, sizeof(readData) - 1);

        myRFile.close();

        Serial.print("readData = ");
        Serial.println(readData);
    }
    else
    {
        Serial.print("Error open for reading ");
        Serial.println(fileName);
    }

    root = SD.open("/");

    printDirectory(root, 0);

    Serial.println("Print Directory done!");
}

void printDirectory(File dir, int numTabs)
{
    while (true)
    {
        File entry = dir.openNextFile();

        if (!entry)
        {
            // no more files
            break;
        }

        for (uint8_t i = 0; i < numTabs; i++)
        {
            Serial.print('\t');
        }

        Serial.print(entry.name());

        if (entry.isDirectory())
        {
            Serial.println("/");
            printDirectory(entry, numTabs + 1);
        }
        else
        {
            // files have sizes, directories do not
            Serial.print("\t\t");
            Serial.println(entry.size(), DEC);
        }

        entry.close();
    }
}
