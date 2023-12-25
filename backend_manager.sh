#!/bin/sh

DIR_CONFIG_FILES="$PWD/config_files"
DIR_CONFIG_UNIT_TEST="$DIR_CONFIG_FILES/unit_tests"
DIR_CONFIG_RELEASE="$DIR_CONFIG_FILES/release"

DIR_TEST_BUILD="$PWD/tests/unit_tests/build_conan"
DIR_BUILD="$PWD/build_conan"
program_option='h'

# Check if an argument is passed
if [ $# = 1 ];
then
    if [ "$1" = "all" ]; then
        program_option="$1"
    elif [ "$1" = "configure" ]; then
        program_option="$1"
    elif [ "$1" = "clean" ]; then
        program_option="$1"
    elif [ "$1" = "build" ]; then
        program_option="$1"
    elif [ "$1" = "run" ]; then
        program_option="$1"
    elif [ "$1" = "rm" ]; then
        program_option="$1"
    elif [ "$1" = "test" ]; then
        program_option="$1"
    else
        echo "Invalid option!\n"
    fi
fi

if [ "$program_option" = "help" -o "$program_option" = "h" ]; then
  echo "Usage"
  echo "        ./backend_manager.sh [options]"
  echo "Options"
  echo "   help, h           = Show the help of the program."
  echo "   all               = Configure, build and run the project. This argument is launch by default if no arguments is passed."
  echo "   configure         = Configure the project."
  echo "   clean             = Clean the project."
  echo "   build             = Build the project."
  echo "   run               = Run the project."
  echo "   rm                = Remove all the files created"
  echo "   test              = Configure, build and run the unit tests of the project."
fi

# Check if the build folder is already created
if [ "$program_option" = "all" -o "$program_option" = "configure" ];
then
    if [ ! -d "$DIR_BUILD" ];
    then
        mkdir $DIR_BUILD
    fi
    #conan profile detect --force
    conan install $DIR_CONFIG_RELEASE --output-folder $DIR_BUILD/cmake-build --build=missing
    cmake -S $DIR_CONFIG_RELEASE -B $DIR_BUILD -DCMAKE_TOOLCHAIN_FILE=$DIR_BUILD/cmake-build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=release
fi

if [ "$program_option" = "all" -o "$program_option" = "clean" ];
then
    cd $DIR_BUILD
    make clean 
    cd ~
fi

if [ "$program_option" = "all" -o "$program_option" = "build" ];
then
    cmake --build $DIR_BUILD --config release
fi

if [ "$program_option" = "all" -o "$program_option" = "run" ];
then
    $DIR_BUILD/LogAnalyzer.bin
fi

if [ "$program_option" = "rm" ];
then
    if [ -d "$DIR_BUILD" ]; then
        rm -r $DIR_BUILD
    fi
    if [ -d "$DIR_TEST_BUILD" ]; then
        rm -r $DIR_TEST_BUILD
    fi
fi

if [ "$program_option" = "test" ];
then
    if [ ! -d "$DIR_TEST_BUILD" ];
    then
        mkdir $DIR_TEST_BUILD
    fi

    #conan profile detect --force
    conan install $DIR_CONFIG_UNIT_TEST --output-folder $DIR_TEST_BUILD/cmake-build --build=missing
    cmake -S $DIR_CONFIG_UNIT_TEST -B $DIR_TEST_BUILD -DCMAKE_TOOLCHAIN_FILE=$DIR_TEST_BUILD/cmake-build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=release
    cmake --build $DIR_TEST_BUILD --config release
    
    $DIR_TEST_BUILD/LogAnalyzer.bin
fi