
// 888       888        d8888 8888888b.  888b    888 8888888 888b    888  .d8888b.   888 888 888
// 888   o   888       d88888 888   Y88b 8888b   888   888   8888b   888 d88P  Y88b  888 888 888
// 888  d8b  888      d88P888 888    888 88888b  888   888   88888b  888 888    888  888 888 888
// 888 d888b 888     d88P 888 888   d88P 888Y88b 888   888   888Y88b 888 888         888 888 888
// 888d88888b888    d88P  888 8888888P"  888 Y88b888   888   888 Y88b888 888  88888  888 888 888
// 88888P Y88888   d88P   888 888 T88b   888  Y88888   888   888  Y88888 888    888  Y8P Y8P Y8P
// 8888P   Y8888  d8888888888 888  T88b  888   Y8888   888   888   Y8888 Y88b  d88P   "   "   " 
// 888P     Y888 d88P     888 888   T88b 888    Y888 8888888 888    Y888  "Y8888P88  888 888 888
// 
// 
// This header file is generated by script.
// DON'T modify manually!
// Generate time: 2017/12/23 07:08:40

#include "./base.hpp"

#ifndef DB_CLIENT_INDEX
#define DB_CLIENT_INDEX 1

//  ====================
//  @file ./src/db-client/help.cc
//  ====================

//  source: ./src/db-client/help.cc:4
bool isLaunchForHelp(int argc, char ** argv);

//  source: ./src/db-client/help.cc:14
void printHelpforInsideREPL();

//  source: ./src/db-client/help.cc:11
void printHelpforShell();

//  ====================
//  @file ./src/db-client/seek.cc
//  ====================

//  source: ./src/db-client/seek.cc:6
void seekByCommand(FILE* file, char cmd, char* params);

//  ====================
//  @file ./src/db-client/reader.cc
//  ====================

//  source: ./src/db-client/reader.cc:11
void readBytes(FILE* file, uint bytesInRow, uint rows, bool followCursor);

//  source: ./src/db-client/reader.cc:72
void readBytesByCommand(FILE* file, const char* parameters, bool followCursor);

//  ====================
//  @file ./src/db-client/writer.cc
//  ====================

//  source: ./src/db-client/writer.cc:4
void writeBytes(FILE* file, void* data, int length, const char* name);

//  source: ./src/db-client/writer.cc:19
void writeBytesByCommand(FILE* file, const char* parameters);

//  ====================
//  @file ./src/db-client/db_info.cc
//  ====================

//  source: ./src/db-client/db_info.cc:21
void readDatabaseHeaderInfo(FILE* file);

//  source: ./src/db-client/db_info.cc:69
void readUserItemByCommand(FILE* file, const char* parameters);

//  ====================
//  @file ./src/db-client/command_reader.cc
//  ====================

//  source: ./src/db-client/command_reader.cc:50
bool readCommand(const char* prefix, UserCommand* result);

#endif

// md5sum:
// dc092eb3ddf8c2a7ab40b331ece9fc10  ./src/db-client/help.cc
// cc4d33631864f7b28680f96af5241e74  ./src/db-client/seek.cc
// d46b6aa6965cee8d39b0348588f30f41  ./src/db-client/reader.cc
// b4de8b0e1bd7b70d888204a3f586789a  ./src/db-client/writer.cc
// e592e033a5e49fd186e1bf7317c050cb  ./src/db-client/db_info.cc
// e8b15d9dcdbc9b448d7cfb288cd8d6bc  ./src/db-client/command_reader.cc