/*
 *  Error.hpp
 *
 *  This file contains the error codes used by maptag.
 *
 *  Created on: 20 maj 2013
 *  E-mail: hirad.asadi@gmail.com
 */

#ifndef ERROR_HPP_
#define ERROR_HPP_

namespace maptag
{

enum ErrorCode
{
    NO_ERROR,
    VERIFICATION_ERROR,
    PATH_DOES_NOT_EXIST_ERROR,
    DB_INIT_ERROR,
    DB_CONNECTION_OR_CREATION_ERROR,
    STATEMENT_PREPARATION_ERROR
};

typedef struct error_
{
    ErrorCode err;
    std::string msg;
} Error;

} /* namespace maptag */

#endif /* ERROR_HPP_ */
