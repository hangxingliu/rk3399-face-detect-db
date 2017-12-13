
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
// Generate time: 2017/12/13 18:11:00

#include "../types/base.hpp"
#include "../types/database.hpp"
#include "../types/database_operation.hpp"

#ifndef STORAGE_HPP
#define STORAGE_HPP 1

//  ====================
//  @file ./src/storage/encoder.cc
//  ====================

//  source: ./src/storage/encoder.cc:47
bool DB_decodeBaseUserInfo(char* bytes, DB_BaseUserItem* result);

//  source: ./src/storage/encoder.cc:60
void DB_decodeLastMatchedInfo();

//  source: ./src/storage/encoder.cc:9
char* DB_encodeUserInfo(DB_BaseUserItem* userItem, DB_BaseUserItem_LastMatched* lastMatchedInfo, char* buffer);

//  ====================
//  @file ./src/storage/hash.cc
//  ====================

//  source: ./src/storage/hash.cc:6
void DB_calcUserItemHash(DB_BaseUserItem* item, unsigned int* hashResult);

//  source: ./src/storage/hash.cc:29
bool DB_validateUserItem(DB_BaseUserItem* item);

//  ====================
//  @file ./src/storage/main.cc
//  ====================

//  source: ./src/storage/main.cc:268
int DB__validateUserId(const char* userId);

//  source: ./src/storage/main.cc:254
int DB_close();

//  source: ./src/storage/main.cc:75
int DB_createDatabaseFile(const char* _path, unsigned long initializedSize);

//  source: ./src/storage/main.cc:297
int DB_deleteUser(const char* userId);

//  source: ./src/storage/main.cc:288
int DB_findUser(const char* userId, DB_BaseUserItem* item);

//  source: ./src/storage/main.cc:248
int DB_flushToDisk();

//  source: ./src/storage/main.cc:39
const unsigned char* DB_getDefaultHead();

//  source: ./src/storage/main.cc:67
FILE* DB_getFileStream();

//  source: ./src/storage/main.cc:49
int DB_getHeadSize();

//  source: ./src/storage/main.cc:53
long DB_getItemOffset(uint itemIndex);

//  source: ./src/storage/main.cc:50
int DB_getLivingCountOffset();

//  source: ./src/storage/main.cc:51
int DB_getLivingCountRepeatOffset();

//  source: ./src/storage/main.cc:209
int DB_init();

//  source: ./src/storage/main.cc:171
int DB_loadDatabase(const char* path);

//  source: ./src/storage/main.cc:266
int DB_pingPong(int ping);

//  source: ./src/storage/main.cc:311
int DB_updateFeatures(const char* userId, FF_FaceFeatures* features);

//  source: ./src/storage/main.cc:343
int DB_updatePriority(const char* userId, int priority);

//  source: ./src/storage/main.cc:132
int DB_validateHead(const unsigned char* head, int* itemCountResult);

//  ====================
//  @file ./src/storage/blank_space_manager.cc
//  ====================

//  source: ./src/storage/blank_space_manager.cc:17
bool BlankSpaceManager_addFragment(uint itemIndex, bool initializing);

//  source: ./src/storage/blank_space_manager.cc:38
uint BlankSpaceManager_getSpace();

//  source: ./src/storage/blank_space_manager.cc:12
void BlankSpaceManager_init();

//  source: ./src/storage/blank_space_manager.cc:34
void BlankSpaceManager_setContinuousSpaceStart(uint itemIndex);

//  ====================
//  @file ./src/storage/item_reader.cc
//  ====================

//  source: ./src/storage/item_reader.cc:80
bool ItemReader__getItemFromDisk(uint itemIndex, DB_BaseUserItem* result);

//  source: ./src/storage/item_reader.cc:163
bool ItemReader__updateItemCacheInMemory(uint itemIndex, DB_BaseUserItem* newItem);

//  source: ./src/storage/item_reader.cc:108
bool ItemReader_findItemByUserId(const char* userId, DB_BaseUserItem* result);

//  source: ./src/storage/item_reader.cc:94
bool ItemReader_getItem(uint itemIndex, DB_BaseUserItem* result);

//  source: ./src/storage/item_reader.cc:19
uint ItemReader_getLivingCount();

//  source: ./src/storage/item_reader.cc:26
int ItemReader_init(FILE* _fs, uint expectlivingCount);

//  source: ./src/storage/item_reader.cc:132
bool ItemReader_iterateItem(DB_Iterator iterator);

//  source: ./src/storage/item_reader.cc:21
void ItemReader_resetAllLivingStatus();

//  ====================
//  @file ./src/storage/item_writer.cc
//  ====================

//  source: ./src/storage/item_writer.cc:50
bool ItemWriter__update(int lCount, uint itemIndex, DB_BaseUserItem* item);

//  source: ./src/storage/item_writer.cc:140
bool ItemWriter_deleteItem(DB_BaseUserItem* oldItem);

//  source: ./src/storage/item_writer.cc:132
bool ItemWriter_modifyItem(DB_BaseUserItem* modifyTo);

//  source: ./src/storage/item_writer.cc:118
bool ItemWriter_newItem(DB_BaseUserItem* newItem);

#endif

// md5sum:
// 29e28b9b18cd49de627d05dd9ae0786a  ./src/storage/encoder.cc
// 153a96213ac50fc87d9d5b968da8a1d0  ./src/storage/hash.cc
// be82f0265d172e5fabca08ffbac1afb6  ./src/storage/main.cc
// 281f8e18e85ee8bb7a48c4f25022fb9f  ./src/storage/blank_space_manager.cc
// f5f4a19173e29e19394842dec005cf35  ./src/storage/item_reader.cc
// f6a59ab041795e20c72f594b541f7f39  ./src/storage/item_writer.cc
