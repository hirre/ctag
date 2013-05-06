/*
 *  CTagHandler.cpp
 *
 *  Handler class that handles input from the command line.
 *
 *  Created on: 5 maj 2013
 *  Author: Hirad Asadi
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

#include "CTagHandler.h"
#include "Debug.hpp"

namespace ctag
{

CTagHandler::CTagHandler()
{
}

CTagHandler::~CTagHandler()
{
}

/*
 * Method to process input from the command line.
 */
void CTagHandler::processInput(const std::vector<std::string>& argVec,
        const std::string& flag)
{
    if (flag.compare("tag") == 0)
    {
#ifdef DEBUG
        debug::dbgPrint("PROCESSING: tag");
#endif

        // TODO: implement tag
    }
    else if (flag.compare("removetag") == 0)
    {
#ifdef DEBUG
        debug::dbgPrint("PROCESSING: removetag");
#endif

        // TODO: implement removetag
    }
    else if (flag.compare("showtag") == 0)
    {
#ifdef DEBUG
        debug::dbgPrint("PROCESSING: showtag");
#endif

        // TODO: implement showtag
    }
}

} /* namespace ctag */
