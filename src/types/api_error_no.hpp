#include "../log/log.hpp"

#ifndef PUBLIC_TYPES_API_ERROR_NO_HPP
#define PUBLIC_TYPES_API_ERROR_NO_HPP
#define __API_ERR(id, msg) LOG_FATAL(msg), id
#define __API_DEBUG(id, msg) LOG_DEBUG(msg), id

#define API_OK 0

#define API_EMPTY_POINTER			__API_ERR( 10, "Invoke API with empty pointer")
#define API_TODO					__API_ERR( 66, "TODO doesn't means all")
#define API_ERROR_TERRIBLE			__API_ERR( 99, "A terrible error occured and I don't know the reason")

#define API_CANNOT_INIT_CAPTURE		__API_ERR(101, "Can not initialize capture")

#define API_WORKSPACE_TOO_LONG		__API_ERR(201, "Workspace path is too long")
#define API_INIT_CONFIG_AGAIN		__API_ERR(202, "Initialize configuration repeatedly")
#define API_HAS_NOT_INIT_CONFIG		__API_ERR(203, "Configuration has not initialized")

#define API_CANNOT_INIT_BUFFER		__API_ERR(301, "Can not initialize frame buffer")

#define API_READ_FRAME_FAILED		__API_ERR(401, "Read frame image failed")
#define API_FRAME_IS_EMPTY			__API_ERR(402, "Frame image is empty")
#define API_FRAME_IS_NOT_U8C3		__API_ERR(403, "Frame image is not 3 channel unsigned char")
#define API_FRAME_IS_NOT_CONTINUOUS	__API_ERR(404, "Frame image is not continuous")

#define API_DB_MKDIRS_FAILED		__API_ERR(501, "Could not create directory database file located in")
#define API_DB_CREATE_FILE_FAILED	__API_ERR(502, "Could not create database file")
#define API_DB_IO_FAILED			__API_ERR(503, "File IO exception in database file")
#define API_DB_FILE_IS_NOT_A_FILE	__API_ERR(504, "Database file is not a file")
#define API_DB_HEAD_IS_SO_SHORT		__API_ERR(505, "Head of database file is so short")
#define API_DB_INVALID_HEAD			__API_ERR(506, "Invalid head bytes of database file")
#define API_DB_INVALID_VERSION		__API_ERR(507, "Invalid database version")
#define API_DB_INVALID_ITEM_COUNT	__API_ERR(508, "Invalid database item count")

#define API_DB_OPEN_FD_FAILED		__API_ERR(511, "Could not open database file descriptor")
#define API_DB_OPEN_FS_FAILED		__API_ERR(512, "Could not open database file stream from fd")
#define API_DB_FLUSH_FAILED			__API_ERR(513, "Could not flush buffer to disk")
#define API_DB_FCLOSE_FAILED		__API_ERR(514, "Could not fclose database file stream")

#define API_DB_INNER_EXCEPTION		__API_ERR(521, "Database inner exception")


#define API_NO_FACE				__API_DEBUG(10001, "Could not detect face from frame image");

#endif // PUBLIC_TYPES_API_ERROR_NO_HPP
