/*
 *  Helper.hpp
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

#ifndef HELPER_HPP_
#define HELPER_HPP_

#ifndef _WIN32
#include <pwd.h>
#define PATH_SEPARATOR "/"
#else
#define PATH_SEPARATOR "\\"
#endif

// Allow numbers, letters, "_" and "-"
#define REGEX_STR "^[a-zA-Z0-9_]*$"

#include <unistd.h>
#include <boost/regex.hpp>

/*
 * Function that returns the home folder independent of the OS.
 */
inline static const char * getHomeFolder()
{
#ifdef _WIN32
    char* homep = getenv("USERPROFILE");
    // Home found
    if (homep != NULL)
    {
        return homep;
    }
    else // Home not found
    {
        std::stringstream home;
        char* hd = getenv("HOMEDRIVE");
        char* hp = getenv("HOMEPATH");

        if(hd == NULL || hp == NULL)
        {
            std::cerr << "Home folder could not be found, make sure %HOMEDRIVE% and %HOMEPATH% are set." << std::endl;
            exit(1);
        }

        home << hd << hp;
        return home.str().c_str();
    }
#else
    char* home = getenv("HOME");

    // Home found
    if (home != NULL)
    {
        return home;
    }
    else
    {
        // Home not found
        char* p = getpwuid(getuid())->pw_dir;
        if (p == NULL)
        {
            std::cerr
                    << "Home folder could not be found, make sure $HOME is set."
                    << std::endl;
            exit(1);
        }

        return p;
    }

#endif
}

/*
 * Verify if input is correct.
 */
inline static bool verifyInput(std::string str)
{
    static const boost::regex e(REGEX_STR);
    return regex_match(str, e);
}

#endif /* HELPER_HPP_ */
