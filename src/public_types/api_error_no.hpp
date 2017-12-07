#include "../log/log.hpp"

#ifndef PUBLIC_TYPES_API_ERROR_NO_HPP
#define PUBLIC_TYPES_API_ERROR_NO_HPP
#define __API_ERR(id, msg) LOG_FATAL(msg), id


#define API_CANNOT_INIT_CAPTURE		__API_ERR(101, "Can not initialize capture");
#define API_WORKSPACE_TOO_LONG		__API_ERR(201, "Workspace path is too long");
#define API_RESOURCES_TOO_LONG		__API_ERR(202, "Resources path is too long");

#endif // PUBLIC_TYPES_API_ERROR_NO_HPP
