/*--------------------------------------------------------------------------------------------------------------*/
/* Program: Generator for random tests                                                                          */
/* Version: 01                                                                                                  */
/* Author:  Lasko M. Laskov                                                                                     */
/* E-mail:  llaskov@nbu.bg                                                                                      */
/*--------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------*/
// standard includes
#include <iostream>
#include <cstdlib>
#include <string>
/*--------------------------------------------------------------------------------------------------------------*/

using namespace std;

/*--------------------------------------------------------------------------------------------------------------*/
// own includes
#include "TestPool.h"
//#include "QuestSet.h"
//#include "Question.h"
/*--------------------------------------------------------------------------------------------------------------*/

// read the name of the input xml file
inline void readFileName(string &);

// assure that the file name is valid
inline bool isValidName(const string &);

int main(int argc, char* argv[])
{
    cout << "Test generator Ver. 01." << endl;

    string in_file_name;            // input xml file name

    // read input xml file name ---------------------------------------------------------------------------------
    switch (argc)
    {
        case 1:
            // ask the use for the file name
            readFileName(in_file_name);
            break;
        case 2:
            // file name is passed as a command line argument
            in_file_name = argv[1];
            if (!isValidName(in_file_name))
            {
                cout << "Illegal file name." << endl;
                cout << "Error: Quiting." << endl;
                exit(EXIT_FAILURE);
            }
            break;
        default:
            cout << "Error: Illegal number of parameters." << endl;
            cout << "Quiting." << endl;
            exit(EXIT_FAILURE);
    }

    // invoke ---------------------------------------------------------------------------------------------------
    TestPool test_pool(in_file_name);
    test_pool.parse();
    test_pool.generate("result/out.tex", "result/ans.txt");

    return 0;
}//main

inline void readFileName(string &in_file_name)
{
    cout << "Enter the .xml file name:" << endl;
    do
    {
        cout << "> ";
        getline(cin, in_file_name);
    }
    while(!isValidName(in_file_name));
}//readFileName

inline bool isValidName(const string& in_file_name)
{
    return (in_file_name.rfind(".xml") == -1) ? false : true;
}//isValidName
