#include "./base.hpp"

#ifndef PUBLIC_TYPES_DATABASE_OPERATION_HPP
#define PUBLIC_TYPES_DATABASE_OPERATION_HPP


#define DB_MODIFICATION_DESPACITO 0
#define DB_MODIFICATION_UPDATE 1
#define DB_MODIFICATION_DELETE 2

typedef struct DB_Modification {
	/// @see DB_MODIFICATION_DESPACITO
	/// @see DB_MODIFICATION_UPDATE
	/// @see DB_MODIFICATION_DELETE
	int type = DB_MODIFICATION_DESPACITO;

	/**
	 * @see USERID_LENGTH
	 */
	char *userId;

	int featureLength;
	float *features;

} DB_Modification;

#endif // PUBLIC_TYPES_DATABASE_OPERATION_HPP
