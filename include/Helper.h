/*
 *  Helper.h
 *
 *  Helper header for different purposes.
 *
 *  Created on: 7 maj 2013
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

#ifndef HELPER_H_
#define HELPER_H_

#include <pwd.h>
#include <unistd.h>

/*
 * Function that returns the home folder independent of OS.
 */
inline static const char * getHomeFolder()
{
#ifdef _WIN32
    std::string home = getenv("USERPROFILE");
    // Home found
    if(!home.empty())
    {
        return home.c_str();
    }
    else // Home not found
    {
        std::string home = getenv("HOMEDRIVE") + getenv("HOMEPATH");
        return home.c_str();
    }
#else
    std::string home = getenv("HOME");

    // Home found
    if (!home.empty())
    {
        return home.c_str();
    }
    else
        // Home not found
        return getpwuid(getuid())->pw_dir;
#endif

    // Should not be reached
    return "./";
}

#endif /* HELPER_H_ */
