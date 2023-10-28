/*--------------------------------------------------------------------------------------------------------------*/
/* File:    Question.h                                                                                          */
/* Purpose: Class that represents a single question in a test.                                                  */
/* Author:  Lasko M. Laskov                                                                                     */
/* E-mail:  llaskov@nbu.bg                                                                                      */
/*--------------------------------------------------------------------------------------------------------------*/

#ifndef Question_h
#define Question_h

/*--------------------------------------------------------------------------------------------------------------*/
// standard includes
#include <string>
#include <vector>
/*--------------------------------------------------------------------------------------------------------------*/

class Question
{
public:
    Question();                     // default constructor
    Question(                       // copy constructor
        const Question &                    // object to duplicate
    ); 
    Question &                              // return the current object
        operator =(                 // assignment operator
            const Question &                // object to duplicate
    );

    void setDifficulty(             // set test difficulty index
        int                                 // test difficulty index
    );    
    void setText(                   // set text of the question
        const string &                      // text of the question
    );   
    void addAnswer(                 // add an answer text and denote it as correct or incorrect
        const string &,                     // text of the answer 
        bool                                // correct (true) or incorrect (false) 
    );
    string                                  // string containing LaTeX itemize
        toLaTeX() const;            // transform the question to a LaTeX string / itemize
    string                                  // string containing the characters of the correct answers
        correctList() const;        // generate a string containing the characters of the correct answers

    void shuffle();                 // shuffle the answers of the question, keeping track which is correct

private:
    int difficulty;                 // question difficulty index
    int numb_answers;               // number of answers
    string text;                    // text of the question
    vector <string> answers;        // all the answers
    vector <bool> correct;          // each answer can be correct (true) or incorrect (false)
    vector <int> order;             // order of the answers
};

#endif
