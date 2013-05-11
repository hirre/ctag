ctag
====

Command line program to tag files and folders in the OS. Tested on Windows 7 and Mac OSX 10.8.3.

### USAGE 
ctag [flag] [arguments]

(Multiple flags can be used)

### VERSION HISTORY 
0.1:
- Support for adding, deleting and showing tags.

### DEPENDENCY
- Boost >=1.53.
- SQLite (Sqlite3.o) >=3.7.16.2 (download [Amalgamation](http://www.sqlite.org/download.html) and compile your own .o-file with e.g. gcc).

### REQUIREMENTS
Example: OS X 10.8.3, g++ >=4.7

    g++ -I/usr/local/lib/boost_1_53_0/ -I"/Users/hirre/dev/ctag/include" -O3 -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"test/CTagTest.d" -MT"test/CTagTest.d" -o "test/CTagTest.o" "../test/CTagTest.cpp"
         
    g++ -I/usr/local/lib/boost_1_53_0/ -I"/Users/hirre/dev/ctag/include" -O3 -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"src/core/CTagHandler.d" -MT"src/core/CTagHandler.d" -o "src/core/CTagHandler.o" "../src/core/CTagHandler.cpp"
         
    g++ -I/usr/local/lib/boost_1_53_0/ -I"/Users/hirre/dev/ctag/include" -O3 -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"src/Main.d" -MT"src/Main.d" -o "src/Main.o" "../src/Main.cpp"
        
    g++ -L/usr/local/lib/boost_1_53_0/stage/lib -Bstatic -o "ctag"  ./test/CTagTest.o  ./src/core/CTagHandler.o  ./src/Main.o  /Users/hirre/dev/CTag/lib/sqlite3.o -lboost_program_options -lboost_system -lboost_filesystem

(Linker uses specific static boost libraries and sqlite3.o)

It is required that if you use Windows, environmental variables %USERPROFILE% or %HOMEDRIVE% and %HOMEPATH% must be set (usually they are). In OSX or Linux $HOME must be set (usually is).

If "TEST" is defined you need to dynamically link "boost_unit_test_framework" and set the corresponding environmental variable for dynamic libraries (OS dependent).
 
### LICENSE
[GNU General Public License v3](http://www.gnu.org/licenses/gpl.html)
