#pragma once
#include <string>
#include <fstream>
#include <vector>
#include "sdmmc_cmd.h"

using namespace std;

std::string FormatFileName(std::string input);
std::size_t file_size(const std::string& file_name);
void FindReplace(std::string& line, std::string& oldString, std::string& newString);

bool CopyFile(string input, string output);
bool DeleteFile(string fn);
bool RenameFile(string from, string to);
bool MakeDir(std::string _what);


string RundeOutput(double _in, int _anzNachkomma);


FILE* OpenFileAndWait(const char* nm, const char* _mode, int _waitsec = 1, bool silent = true);

size_t findDelimiterPos(string input, string delimiter);
//string trim(string istring);
string trim(string istring, string adddelimiter = "");
bool ctype_space(const char c, string adddelimiter);

string getFileType(string filename);
string getFileFullFileName(string filename);
string getDirectory(string filename);

int mkdir_r(const char *dir, const mode_t mode);
int removeFolder(const char* folderPath, const char* logTag);

string toLower(string in);
string toUpper(string in);

float temperatureRead();

time_t addDays(time_t startTime, int days);

void memCopyGen(uint8_t* _source, uint8_t* _target, int _size);

std::vector<string> HelperZerlegeZeile(std::string input, std::string _delimiter);
std::vector<std::string> ZerlegeZeile(std::string input, std::string delimiter = " =, \t");

///////////////////////////
size_t getInternalESPHeapSize();
size_t getESPHeapSize();
string getESPHeapInfo();

/////////////////////////////
string getSDCardPartitionSize();
string getSDCardFreePartitionSpace();
string getSDCardPartitionAllocationSize();

void SaveSDCardInfo(sdmmc_card_t* card);
string SDCardParseManufacturerIDs(int);
string getSDCardManufacturer();
string getSDCardName();
string getSDCardCapacity();
string getSDCardSectorSize();

string getMac(void);


/* Error bit fields
   One bit per error
   Make sure it matches https://github.com/jomjol/AI-on-the-edge-device/wiki/Error-Codes */
enum SystemStatusFlag_t {          // One bit per error
    // First Byte
    SYSTEM_STATUS_PSRAM_BAD         = 1 << 0, //  4, Critical Error
    SYSTEM_STATUS_HEAP_TOO_SMALL    = 1 << 1, //  8, Critical Error
    SYSTEM_STATUS_CAM_BAD           = 1 << 2, //  1, Critical Error

    // Second Byte
    SYSTEM_STATUS_CAM_FB_BAD        = 1 << (0+8), //  2, Flow still might work
    SYSTEM_STATUS_NTP_BAD           = 1 << (1+8), // 16, Flow will work but time will be wrong
};

void setSystemStatusFlag(SystemStatusFlag_t flag);
void clearSystemStatusFlag(SystemStatusFlag_t flag);
int getSystemStatus(void);
bool isSetSystemStatusFlag(SystemStatusFlag_t flag);

string getResetReason(void);
std::string getFormatedUptime(bool compact);

const char* get404(void);
