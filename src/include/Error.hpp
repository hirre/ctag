/*
 *  Error.hpp
 *
 *  This file contains the error codes used by maptag.
 *
 *  Created on: 20 maj 2013
 *  E-mail: hirad.asadi@gmail.com
 *
 *  Copyright (C) 2013  Hirad Asadi
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
