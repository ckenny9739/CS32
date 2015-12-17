//
//  eval.cpp
//  HW2
//
//  Created by Connor Kenny on 1/29/15.
//  Copyright (c) 2015 Connor Kenny. All rights reserved.
//

#include <string>
#include <stack>
#include <iostream>
#include <cassert>
using namespace std;

// Evaluates a boolean expression
// Postcondition: If infix is a syntactically valid infix boolean
//   expression, then postfix is set to the postfix form of that
//   expression, result is set to the value of the expression (where
//   in that expression, each digit k represents element k of the
//   values array), and the function returns zero.  If infix is not a
//   syntactically valid expression, the function returns 1.  (In that
//   case, postfix may or may not be changed, but result must be
//   unchanged.)

int evaluate(string infix, const bool values[], string& postfix, bool& result)
{
    // Error Checking
    
    int Oparens = 0;
    int Cparens = 0;
    int ops = 0;
    int nums = 0;
    int opposite = 0;
    string numsString = "0123456789";
    string nInfix = "";
    
    for (int i = 0; i < infix.size(); i++)              // Delete spaces
        if (infix[i] != ' ')
            nInfix += infix[i];
    
    switch(nInfix[0])           // First value only, protect against accessing out of bounds
    {
        case '(': Oparens++;
            break;
        case ')': Cparens++;
            break;
        case '|':
        case '&': ops++;
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': nums++;
            break;
        case '!': opposite++;
            break;
        default: return 1;       // Not formed correctly
    }
    
    for (int i = 1; i < nInfix.size(); i++)
    {
        if (Cparens > Oparens)              // Check for more ')' than '('
            return 1;
        switch(nInfix[i])
        {
            case '(': Oparens++;
                if(numsString.find(nInfix[i-1]) != -1 || nInfix[i-1] == ')')
                    return 1; // Not formed correctly
                break;
            case ')': Cparens++;
                if(nInfix[i-1] == '(' || nInfix[i-1] == '&' || nInfix[i-1] == '|' || nInfix[i-1] == '!')
                    return 1; // Not formed correctly
                break;
            case '|':
            case '&': ops++;
                if(numsString.find(nInfix[i-1]) == -1 && nInfix[i-1] != ')')
                    return 1; // Not formed correctly
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9': nums++;
                break;
            case '!': opposite++;
                if(nInfix[i-1] != '(' && nInfix[i-1] != '!' && numsString.find(nInfix[i-1]) != -1)
                    return 1;        // Not formed correctly
                break;
            default: return 1;       // Not formed correctly
        }
    }
    
    if (Oparens != Cparens || nums <= 0 || ops >= nums || (ops <= 0 && nums > 1))
        return 1;
    
    // Conversion
    postfix = "";
    stack<char> op;
    
    for(int i = 0; i < infix.size(); i++)
    {
        char ch = infix[i];
        switch(ch)
        {
            case ' ':
                continue;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                postfix += ch;
                break;
            case '(':
            case '!':
                op.push(ch);
                break;
            case ')':
                if (op.empty())
                    return 1;           // No '(' before it in stack
                while (op.top() != '(')
                {
                    postfix += op.top();
                    op.pop();
                }
                op.pop();
                break;
            case '|':
                while (!op.empty() && op.top() != '(') // Always <= stack
                {
                    postfix += op.top();
                    op.pop();
                }
                op.push(ch);
                break;
            case '&':
                while (!op.empty() && op.top() != '(' && op.top() != '|') // & <= & and !
                {
                    postfix += op.top();
                    op.pop();
                }
                op.push(ch);
                break;
        }
    }
        while (!op.empty())
        {
            postfix += op.top();
            op.pop();
        
    }
    
    // Evaluation
    stack<bool> operand;
    stack<char> operate;
    
    if (nums == 1 && opposite == 0)      // Just a number, no '!'
    {
        result = values[postfix[0] - '0'];
        return 0;
    }

    for(int i = 0; i < postfix.size(); i++)
    {
        char ch = postfix[i];
        if (ch == '!')
        {
            bool op = operand.top();
            operand.pop();
            operand.push(!op);
        }
        else if (ch != '|' && ch != '&')
        {
            operand.push(values[ch - '0']);
        }
        else if (!operate.empty() && operate.top() == '!')
        {
            bool op = operand.top();
            operand.pop();
            operand.push(!op);
        }
        else
        {
            bool operand2 = operand.top();
            operand.pop();
            bool operand1 = operand.top();
            operand.pop();
            if (ch == '|')
                operand.push(operand1 || operand2);
            else
                operand.push(operand1 && operand2);
        }
    }
    result = operand.top();
    return 0;
}
/*

int main()
{
    bool ba[10] = {
        //  0      1      2      3      4      5      6      7      8      9
        true,  true,  true,  false, false, false, true,  false, true,  false
    };
    string pf;
    bool answer;
    assert(evaluate("2| 3", ba, pf, answer) == 0  &&  pf == "23|" &&  answer);
    assert(evaluate("8|", ba, pf, answer) == 1);
    assert(evaluate("4 5", ba, pf, answer) == 1);
    assert(evaluate("01", ba, pf, answer) == 1);
    assert(evaluate("()", ba, pf, answer) == 1);
    assert(evaluate("2(9|8)", ba, pf, answer) == 1);
    assert(evaluate("2(&8)", ba, pf, answer) == 1);
    assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
    assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0
           &&  pf == "43!03&&|"  &&  !answer);
    assert(evaluate("", ba, pf, answer) == 1);
    assert(evaluate(" 9  ", ba, pf, answer) == 0  &&  pf == "9"  &&  !answer);
    assert(evaluate("()()()()", ba, pf, answer) == 1);
    assert(evaluate("1 & 2 | 4 | 4 | 4", ba, pf, answer) == 0  &&  pf == "12&4|4|4|" &&  answer);
    assert(evaluate("s", ba, pf, answer) == 1);
    assert(evaluate("3)", ba, pf, answer) == 1);
    assert(evaluate("!3", ba, pf, answer) == 0  &&  pf == "3!" &&  answer);
    assert(evaluate("1)3&4!(", ba, pf, answer) == 1);
    assert(evaluate("(4)3&4)5&(", ba, pf, answer) == 1);
    assert(evaluate("(2&2)&(2&2)", ba, pf, answer) == 0  &&  pf == "22&22&&" &&  answer);
    assert(evaluate(" 1&!(9|1&1|9) | !!!(9&1&9)", ba, pf, answer) == 0 && answer);
    assert(evaluate("!(7|8)", ba, pf, answer) == 0 && !answer);
 
    ba[2] = false;
    ba[9] = true;
    assert(evaluate("((9))", ba, pf, answer) == 0  &&  pf == "9"  &&  answer);
    assert(evaluate("2| 3", ba, pf, answer) == 0  &&  pf == "23|" &&  !answer);
    
    
    cout << "Passed all tests" << endl;
}

*/