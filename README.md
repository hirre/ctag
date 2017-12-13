maptag
====

Command line program to tag files and folders in the OS using a key-value system. Tested on Windows 7 and Mac OSX 10.8.3.

### USAGE 
maptag [flag] [arguments]

(Multiple flags can be used)

### VERSION HISTORY 
0.3:
- Key-value support added.
- Bug fixes.
- Tests updated.
- Old code removed.

0.22:
- Name change to maptag.

0.21:
- Some bugs fixed.
- Menu system updated.
- Unit tests updated.

0.2:
- Many bugs fixed.
- Unit tests added.
- Small changes here and there.

0.1:
- Support for adding, deleting and showing tags.

### DEPENDENCY
Needed for compilation:
- Boost >=1.53.
- SQLite (Sqlite3.o) >=3.7.16.2 (download [Amalgamation](http://www.sqlite.org/download.html) and compile your own .o-file with e.g. gcc).

### COMPILATION
Makefile in "Release*" and "Test*" folders (paths must be changed to comply with your system).

Manual compilation example (OS X 10.8.3, g++ >=4.7):

    g++ -I/usr/local/lib/boost_1_53_0/ -I"/Users/hirre/dev/maptag/include" -O3 -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"test/MapTagTest.d" -MT"test/MapTagTest.d" -o "test/MapTagTest.o" "../test/MapTagTest.cpp"
 
    g++ -I/usr/local/lib/boost_1_53_0/ -I"/Users/hirre/dev/maptag/include" -O3 -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"src/core/FlagHandler.d" -MT"src/core/FlagHandler.d" -o "src/core/FlagHandler.o" "../src/core/FlagHandler.cpp"
 
    g++ -I/usr/local/lib/boost_1_53_0/ -I"/Users/hirre/dev/maptag/include" -O3 -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"src/Main.d" -MT"src/Main.d" -o "src/Main.o" "../src/Main.cpp"
 
    g++ -L/usr/local/lib/boost_1_53_0/stage/lib -Bstatic -o "maptag"  ./test/MapTagTest.o  ./src/core/FlagHandler.o  ./src/Main.o  /Users/hirre/dev/maptag/lib/sqlite3.o -lboost_program_options -lboost_regex -lboost_system -lboost_filesystem

- Linker uses specific static boost libraries and sqlite3.o.
- It is required that if you use Windows, environmental variables %USERPROFILE% or %HOMEDRIVE% and %HOMEPATH% must be set (usually they are). In OSX or Linux $HOME must be set (usually is).
- If "TEST" is defined you need to dynamically link "boost_unit_test_framework" by setting the corresponding environmental variable for dynamic libraries (OS dependent).
