/*--------------------------------------------------------------------------------------------------------------*/
/* File:    TestPool.h                                                                                          */
/* Purpose: Class that represents the whole database of questions loaded from the xml file                      */
/*          Reads the xml file and stores all the questions data in different sets with respect to difficulty   */
/*          Parsing is based on RAPIDXML 1.13: http://rapidxml.sourceforge.net/                                 */
/*          Generates all the variants and stores them in a tex file                                            */
/* Author:  Lasko M. Laskov                                                                                     */
/* E-mail:  llaskov@nbu.bg                                                                                      */
/*--------------------------------------------------------------------------------------------------------------*/

#ifndef TestPool_h
#define TestPool_h

/*--------------------------------------------------------------------------------------------------------------*/
// standard includes
#include <string>
#include <vector>
#include <fstream>
/*--------------------------------------------------------------------------------------------------------------*/

using namespace std;

/*--------------------------------------------------------------------------------------------------------------*/
// own includes
#include "QuestSet.h"
/*--------------------------------------------------------------------------------------------------------------*/

class TestPool
{
public:
    TestPool(                       // parameter constructor
        const string &                      // file name of the input xml file
    );
    void parse();                   // parse the input xml file
    void generate(                  // generate test variants
            const string &,                 // output LaTeX file
            const string &                  // output ASCII answer file
    );

private:
    string in_file_name;            // file name of the input xml
    string signature;               // course signature
    string course;                  // course name
    string semester;                // semester (year and season)
    string test_name;               // semester / exam session, and which number
    int points;                     // maximum number of points for the test
    int variants;                   // number of variants to be generated
    int numb_sets;                  // number of different sets of questions, and number of questions in variant
    vector <QuestSet> pool;         // contains all the questions; each element is a set of questions
    ofstream out_var_file;          // output file stream for the LaTeX file

    inline void printPreamble();    // print preamble of the LaTeX file
    inline void printVarMeta(       // print variant meta data including variant number
            int                             // variant number
    );
};

#endif
