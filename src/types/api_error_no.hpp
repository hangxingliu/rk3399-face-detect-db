#include "../log/log.hpp"

#ifndef PUBLIC_TYPES_API_ERROR_NO_HPP
#define PUBLIC_TYPES_API_ERROR_NO_HPP
#define __API_ERR(id, msg) LOG_FATAL(msg), id

#define API_OK 0

#define API_EMPTY_POINTER			__API_ERR( 10, "Invoke API with empty pointer")
#define API_ERROR_TERRIBLE			__API_ERR( 99, "A terrible error occured and I don't know the reason")

#define API_CANNOT_INIT_CAPTURE		__API_ERR(101, "Can not initialize capture")

#define API_WORKSPACE_TOO_LONG		__API_ERR(201, "Workspace path is too long")
#define API_RESOURCES_TOO_LONG		__API_ERR(202, "Resources path is too long")

#define API_CANNOT_INIT_BUFFER		__API_ERR(301, "Can not initialize frame buffer")

#define API_READ_FRAME_FAILED		__API_ERR(401, "Read frame image failed")
#define API_FRAME_IS_EMPTY			__API_ERR(402, "Frame image is empty")
#define API_FRAME_IS_NOT_U8C3		__API_ERR(403, "Frame image is not 3 channel unsigned char")
#define API_FRAME_IS_NOT_CONTINUOUS	__API_ERR(404, "Frame image is not continuous")

#endif // PUBLIC_TYPES_API_ERROR_NO_HPP
