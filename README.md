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
- Boost >=1.53
- SQLite (Sqlite3.o) >=3.7.16.2 (download "amalgamation" from http://www.sqlite.org/download.html and compile your own .o-file with e.g. gcc)

### COMPILATION
Example: Windows 7, MinGW & g++ >=4.7

g++ "-IC:\\lib\\boost\\boost_1_53_0" "-IC:\\Users\\me\\dev\\ctag\\include" -O3 -c -fmessage-length=0 -std=c++11 -o "src\\Main.o" "..\\src\\Main.cpp"
 
g++ "-LC:\\lib\\boost\\boost_1_53_0\\stage\\lib" -Bstatic -o ctag "src\\core\\CTagHandler.o" "src\\Main.o" "..\\lib\\sqlite3.o" -lboost_filesystem-mgw47-mt-1_53 -lboost_system-mgw47-mt-1_53 -lboost_program_options-mgw47-mt-1_53

(Linker uses specific static boost libraries and sqlite3.o)

If you use Windows, environmental variables $USERPROFILE or $HOMEDRIVE and $HOMEPATH must be set (usually they are). In OSX or Linux $HOME must be set (usually is).
 
### TODO
- Room for improved command descriptions in help.
- Testing different inputs.
- Testing on Linux.