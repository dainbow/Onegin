#ifndef _ONEGIN_H_
#define _ONEGIN_H_

#include <sys\stat.h>

const char END_OF_PART[] = "------------------------------------------------------\n";
const char INPUT_FILE[]  = "Onegin.txt";
const char OUTPUT_FILE[] = "output.txt";
const char HELP_FILE[]   = "help.txt";

//-------------------------------------------------------------------------------------------------------
//! Read all command line arguemnts of program
//!
//! @param [in] argc amount of command line arguments
//! @param [in] argv array of command line arguments
//!
//! @note Program always has 1 argument - its location
//!       If program doesn't find help.txt it will print REALLY short manual, otherwise it prints full description
//-------------------------------------------------------------------------------------------------------

void ReadCommandLineArgs(int argc, char *argv[]);

//-------------------------------------------------------------------------------------------------------
//! If "/?" argument was used, prints help.txt or short description if help.txt not found.
//-------------------------------------------------------------------------------------------------------

void PrintHelp();

#endif
