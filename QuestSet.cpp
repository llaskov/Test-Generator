/*--------------------------------------------------------------------------------------------------------------*/
/* File:    QuestSet.cpp                                                                                        */
/* Purpose: Class that represents a set of questions united by a common feature (for example difficulty)        */
/* Author:  Lasko M. Laskov                                                                                     */
/* E-mail:  llaskov@nbu.bg                                                                                      */
/*--------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------*/
// standard includes
#include <cstdlib>
#include <ctime>
/*--------------------------------------------------------------------------------------------------------------*/

using namespace std;

/*--------------------------------------------------------------------------------------------------------------*/
// own includes
#include "QuestSet.h"
#include "Question.h"
/*--------------------------------------------------------------------------------------------------------------*/

// default constructor
QuestSet::QuestSet()
{
    numb_questions = 0;
    counter = 0;   
}//QuestSet

// copy constructor
QuestSet::QuestSet(const QuestSet &quest_set):
    set(quest_set.set),
    order(quest_set.order)
{
    numb_questions = quest_set.numb_questions;
    counter = quest_set.counter;
}//QuestSet

// add a question to the set
void QuestSet::addQuestion(const Question &quest)
{
    order.push_back(numb_questions);
    numb_questions++;
    set.push_back(quest);
}//addQuestion

// get current question string, and move the counter to the next
string QuestSet::nextLaTeX()
{
    int old_counter = counter;
    counter = (counter + 1) % numb_questions;
    set[order[old_counter]].shuffle();

    return set[order[old_counter]].toLaTeX();
}//nextLaTeX

// get current question string, answers, and move the counter to the next
void QuestSet::nextQuest(string &laTeX, string &answer)
{
    int old_counter = counter;
    counter = (counter + 1) % numb_questions;
    set[order[old_counter]].shuffle();

    laTeX = set[order[old_counter]].toLaTeX();
    answer = set[order[old_counter]].correctList();
}//nextQuest

// shuffle the questions of the set using the vector order
void QuestSet::shuffle()
{
    //srand(time(0)); // must be carefult with the number of calls
    for (int indx_curr = order.size() - 1; indx_curr > 0; indx_curr--)
    {
        int indx_swap = rand() % (indx_curr + 1);   // replacement index
        int tmp = order[indx_curr];                 // swap two integers
        order[indx_curr] = order[indx_swap];
        order[indx_swap] = tmp;
    }   
}//shuffle
