/*
 * CTagTest.hpp
 *
 *  Unit tests for ctag.
 *
 *
 *  Created on: 13 maj 2013
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

#ifndef CTAGTEST_HPP_
#define CTAGTEST_HPP_

#include <iostream>
#define ARRAY_SIZE (127-32)

/*
 * Generates char array
 * Take care of memory allocation!
 */
char* genChar()
{
    // Watch out for memory leak
    char* c = new char[ARRAY_SIZE];

    for (int i = 32; i < 127; i++)
    {
        c[i-32] = i;
    }

    return c;
}

#endif /* CTAGTEST_HPP_ */
