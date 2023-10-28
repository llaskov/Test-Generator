/*--------------------------------------------------------------------------------------------------------------*/
/* File:    Question.cpp                                                                                        */
/* Purpose: Class that represents a single question in a test.                                                  */
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
#include "Question.h"
/*--------------------------------------------------------------------------------------------------------------*/

// default constructor
Question::Question()
{
    difficulty = 0;
    numb_answers = 0;
}//Question

// copy constructor
Question::Question(const Question& quest) :
    text(quest.text),
    answers(quest.answers),
    correct(quest.correct),
    order(quest.order)
{
    difficulty = quest.difficulty;
    numb_answers = quest.numb_answers;
}//Question

// assignment operator
Question &Question::operator =(const Question &quest)
{
    difficulty = quest.difficulty;
    numb_answers = quest.numb_answers;
    text = quest.text;
    answers = quest.answers;
    correct = quest.correct;
    order = quest.order;

    return *this;
}//operator =

// set test difficulty index
void Question::setDifficulty(int difficulty)
{
    this->difficulty = difficulty;
}//setDifficulty

// set text of the question
void Question::setText(const string &text)
{
    this->text = text;
}//setText

// add an answer text and denote it as correct or incorrect
void Question::addAnswer(const string &ans_text, bool ans_correct)
{
    order.push_back(numb_answers);
    numb_answers++;
    answers.push_back(ans_text);
    correct.push_back(ans_correct);
}//addAnswer

// transform the question to a LaTeX string / itemize
string Question::toLaTeX() const
{
    string result = text;
    result += "\n";
    result += "\\begin{enumerate}[(a)]";
    result += "\n";
    for (int indx_ans = 0; indx_ans < answers.size(); indx_ans++)
    {
        result += "\t";
        result += "\\item ";
        result += answers[order[indx_ans]];
        result += "\n";
    } 
    result += "\\end{enumerate}";

    return result;
}//toLaTeX

// generate a string containing the characters of the correct answers
string Question::correctList() const
{
    string result = "";

    char item_ch = 'a';             // item character
    for (int indx = 0; indx < order.size(); indx++)
    {
        if (correct[order[indx]])
        {
            result += item_ch;
            result += " ";
        }
        item_ch++;
    }

    return result;
}//correctList

// shuffle the answers of the question, keeping track which is correct
void Question::shuffle()
{
    //srand(time(0));
    for (int indx_curr = order.size() - 1; indx_curr > 0; indx_curr--)
    {
        int indx_swap = rand() % (indx_curr + 1);   // replacement index
        int tmp = order[indx_curr];                 // swap two integers
        order[indx_curr] = order[indx_swap];
        order[indx_swap] = tmp;
    }   
}//shuffle
