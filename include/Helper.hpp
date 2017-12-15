/*
 *  Helper.hpp
 *
 *  Helper header for different purposes.
 *
 *  Created on: 7 maj 2013
 *  Author: Hirad Asadi
 *  E-mail: hirad.asadi@gmail.com
 */

#ifndef HELPER_HPP_
#define HELPER_HPP_

#ifndef _WIN32
#include <pwd.h>
#define PATH_SEPARATOR "/"
#else
#define PATH_SEPARATOR "\\"
#endif

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
inline static bool verifyInput(const std::string& str, const std::string& regex)
{
    boost::regex e(regex);
    return regex_match(str, e);
}

#endif /* HELPER_HPP_ */
