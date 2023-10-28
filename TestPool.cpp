/*--------------------------------------------------------------------------------------------------------------*/
/* File:    TestPool.cpp                                                                                        */
/* Purpose: Class that represents the whole database of questions loaded from the xml file                      */
/*          Reads the xml file and stores all the questions data in different sets with respect to difficulty   */
/*          Parsing is based on RAPIDXML 1.13: http://rapidxml.sourceforge.net/                                 */
/*          Generates all the variants and stores them in a tex file                                            */
/* Author:  Lasko M. Laskov                                                                                     */
/* E-mail:  llaskov@nbu.bg                                                                                      */
/*--------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------*/
// standard includes
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
/*--------------------------------------------------------------------------------------------------------------*/

using namespace std;

/*--------------------------------------------------------------------------------------------------------------*/
// own includes
#include "TestPool.h"
#include "QuestSet.h"
#include "Question.h"
#include "rapidxml/rapidxml.hpp"
/*--------------------------------------------------------------------------------------------------------------*/

using namespace rapidxml;

// parameter constructor
TestPool::TestPool(const string &in_file_name_par):
    in_file_name(in_file_name_par)
{
    points = 0;                     // maximum number of points for the test
    variants = 0;                   // number of variants to be generated
    numb_sets = 0;                  // number of different sets of questions, and number of questions in variant

    srand(time(0));                 // initialize random seed
}//TestPool

// parse the input xml file
void TestPool::parse()
{
    // init parser ----------------------------------------------------------------------------------------------
    // open the input xml file
    ifstream in_file;
    in_file.open(in_file_name.c_str());
    if (in_file.fail())
    {
        cout << "Error: file " << in_file_name << " cannot be opened." << endl;
        exit(EXIT_FAILURE);
    }

    // read the contents of the xml file and store it into a string
    string file_text;
    while (!in_file.eof())
    {
        string tmp;
        getline(in_file, tmp);
        file_text.append(tmp);
        file_text.append("\n");
    }

    // close the input file
    in_file.close();

    // copy the string to pointer to char that can be modified
    
    char* ptr_text = new char[file_text.size() + 1];
    copy(file_text.begin(), file_text.end(), ptr_text);
    ptr_text[file_text.size()] = '\0';

    // parse the xml data ---------------------------------------------------------------------------------------
    // the parser used is RAPIDXML v.1.13
    xml_document<> doc;             // character type defaults to char
    doc.parse<
        parse_trim_whitespace       // white spaces are trimmed
        >(ptr_text);         

    // load test pool attributes --------------------------------------------------------------------------------
    xml_node<>* node = doc.first_node("test");
    xml_attribute<>* attr = 0;

    // course signature
    attr = node->first_attribute("signature");
    signature = attr->value();
    cout << "Course signature: " << signature << endl;

    // course signature
    attr = node->first_attribute("course");
    course = attr->value();
    cout << "Course name: " << course << endl;

    // semester
    attr = node->first_attribute("semester");
    semester = attr->value();
    cout << "Semester: " << semester << endl;

    // test name: semester / exam session, and which number
    attr = node->first_attribute("test_name");
    test_name = attr->value();
    cout << "Test name: " << test_name << endl;

    // maximum number of points for the test
    attr = node->first_attribute("points");
    points = atoi(attr->value());
    cout << "Points: " << points << endl;

    // number of different sets of questions, and number of questions in variant
    attr = node->first_attribute("variants");
    variants = atoi(attr->value());
    cout << "Variants: " << variants << endl;

    // load questions in different sets -------------------------------------------------------------------------
    
    for (xml_node<>* quest_node = node->first_node("question");
            quest_node;
            quest_node = quest_node->next_sibling())
    {
        // read question difficulty
        xml_attribute<>* quest_attr = quest_node->first_attribute("difficulty");
        int curr_difficulty = atoi(quest_attr->value());
        Question quest;
        quest.setDifficulty(curr_difficulty);

        // read question text
        xml_node<>* text_node = quest_node->first_node("text");
        string text_str = text_node->value();
        quest.setText(text_str); 

        // read question answers
        for (xml_node<>* ans_node = quest_node->first_node("answer");
                ans_node;
                ans_node = ans_node->next_sibling())
        {
            xml_attribute<>* ans_attr = ans_node->first_attribute("correct");
            string true_str = "true";
            bool correct = (true_str == ans_attr->value()) ? true : false;
            quest.addAnswer(ans_node->value(), correct);
        }  
        
        // if the current difficulty is greater then the pool size, resize
        if (pool.size() < curr_difficulty)
        {
            pool.resize(curr_difficulty);
        }  

        // add question to the pool according its difficulty
        pool[curr_difficulty - 1].addQuestion(quest); 
    }

    numb_sets = pool.size();
    cout << "Test pool size: " << pool.size() << endl; 

    // clear memory ---------------------------------------------------------------------------------------------
    delete [] ptr_text;
    ptr_text = 0;
    doc.clear();
}//parse


// generate test variants
void TestPool::generate(const string &var_file_name, const string &ans_file_name)
{
    // shuffle the question sets
    for (int indx_pool = 0; indx_pool < pool.size(); indx_pool++)
    {
        pool[indx_pool].shuffle();
    } 

    // open output file stream for the variants
    out_var_file.open(var_file_name.c_str());
    if (out_var_file.fail())
    {
        cout << "Error: file " << out_var_file << " cannot be opened." << endl;
        exit(EXIT_FAILURE);
    }

    // print preamble of the LaTeX file
    printPreamble();

    out_var_file << "% body of the document" << endl;
    out_var_file << "\\begin{document}" << endl;

    // output answer file stream
    ofstream out_ans_file;
    out_ans_file.open(ans_file_name.c_str());

    for (int indx_var = 0; indx_var < variants; indx_var++)
    {
        out_var_file << endl << "% Variant" << indx_var + 1 << endl;
        out_ans_file << "Variant" << indx_var + 1 << endl;
        printVarMeta(indx_var + 1);
        out_var_file << "\\begin{enumerate}" << endl;
        for (int diff_indx = 0; diff_indx < pool.size(); diff_indx++)
        {
            string next_quest;
            string next_ansli;
            pool[diff_indx].nextQuest(next_quest, next_ansli);
            out_var_file << "\\item " << next_quest << endl;
            out_ans_file << diff_indx + 1 << " " << next_ansli << endl;
        }
        out_var_file << "\\end{enumerate}" << endl;
        out_var_file << "\\clearpage" << endl;
    }

    out_var_file << "\\end{document}" << endl;

    // close output file streams
    out_var_file.close();
    out_ans_file.close();
}//generate

// print preamble of the LaTeX file 
inline void TestPool::printPreamble()
{
    out_var_file << "\\documentclass[10pt,a4paper,twocolumn]{article}" << endl << endl;
    out_var_file << "% packages" << endl;
    out_var_file << "\\usepackage{amsmath, amsthm, amssymb}" << endl;
    out_var_file << "\\usepackage[T1]{fontenc}" << endl;
    out_var_file << "\\usepackage[bitstream-charter]{mathdesign}" << endl;
    out_var_file << "\\usepackage{dsfont}" << endl;
    out_var_file << "\\usepackage{verbatim}" << endl;
    out_var_file << "\\usepackage[left=1.5cm, right=1.5cm, top=4cm, foot=1.5cm, headheight=0.5cm]{geometry}" << endl;
    out_var_file << "\\usepackage{verbatim}" << endl;
    out_var_file << "\\usepackage{fancyhdr}" << endl;
    out_var_file << "\\usepackage{enumerate}" << endl;
    out_var_file << "\\title{} \\author{} \\date{}" << endl << endl;
}//printPreamble

inline void TestPool::printVarMeta(int var_number)
{
    out_var_file << "\t" << "\\thispagestyle{fancy}" << endl;
    out_var_file << "\t" << "\\fancyhf{}" << endl;
    out_var_file << "\t" << "\\fancyhead[L]{\\vspace{-0.5cm}\\large Name and faculty number: ...................................................................................................................." << endl;
    out_var_file << "\t" << "\\\\" << endl;
    out_var_file << "\t" << "\\vspace{0.5cm} \\normalsize \\textbf{" << signature;
    out_var_file << "} \\emph{" << course;                  // course name
    out_var_file << ", " << test_name;                      // name of the test
    out_var_file << "} \\textbf{" << semester;              // semester: year and season
    out_var_file << "}}" << endl;
    out_var_file << "\\fancyhead[R]{Variant " << var_number;// variant number
    out_var_file << "}" << endl;
    out_var_file << "\\fancyfoot[R]{Each correct answer brings ";
    out_var_file << (1.0 * points) / numb_sets;             // points per correct question
    out_var_file << " pt.; maximum points for the test is ";
    out_var_file << points;                                 // maximum points for the whole test
    out_var_file << " pt.}" << endl;
}//printVarMeta
