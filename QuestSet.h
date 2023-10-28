/*--------------------------------------------------------------------------------------------------------------*/
/* File:    QuestSet.h                                                                                          */
/* Purpose: Class that represents a set of questions united by a common feature (for example difficulty)        */
/* Author:  Lasko M. Laskov                                                                                     */
/* E-mail:  llaskov@nbu.bg                                                                                      */
/*--------------------------------------------------------------------------------------------------------------*/

#ifndef QuestSet_h
#define QuestSet_h

/*--------------------------------------------------------------------------------------------------------------*/
// standard includes
#include <vector>
/*--------------------------------------------------------------------------------------------------------------*/

using namespace std;

/*--------------------------------------------------------------------------------------------------------------*/
// own includes
#include "Question.h"
/*--------------------------------------------------------------------------------------------------------------*/

class QuestSet
{
public:
    QuestSet();                     // default constructor
    QuestSet(                       // copy constructor
        const QuestSet &                    // object to duplicate
    );
    void addQuestion(               // add a question to the set
        const Question &                    // question to be added
    );
    string                                  // string containing the current question
        nextLaTeX();                // get current question string, and move the counter to the next
    void nextQuest(                 // get current question string, answers, and move the counter to the next
            string &,                       // current question string
            string &                        // current answer list string
    );
    void shuffle();                 // shuffle the questions of the set using the vector order

private:
    int numb_questions;             // number of questions in the set: the same as vectors sizes
    int counter;                    // cyclic counter for the next question to print
    vector <Question> set;          // contains the set of questions in their original order
    vector <int> order;             // order of the questions, to implement shuffle
};

#endif
