#include <string>
#include <stack>
#include <cassert>
#include <iostream>
#include "Map.h"

using namespace std;

// helper functions
int performOperation(char task, int val1, int val2);
bool validSyntax(const char& prev, char curr, const bool& first, const bool& last);
bool isOperand(char ch);
bool isOperator(char ch);
bool secondOperatorGreater (char op1, char op2);
bool matchingParantheses (string str);

int evaluate(string infix, const Map& values, string& postfix, int& result){
    bool first = true;
    bool last = false;
    char prev;
    
    postfix = "";
    stack <char> operatorStack;
    
    if(infix == "" || infix == " ")
        return 1;
    
    if(infix.find('(') != -1 || infix.find(')') != -1){
        if(!matchingParantheses(infix))
            return 1;
    }
           
    for(int i = 0; i < infix.size(); i++){
        if(infix[i] == ' ') // skip over spaces
            continue;
        if(i == infix.size() - 1)
            last = true;
        if(!validSyntax(prev, infix[i], first, last)) // syntax checking!
            return 1;
        if(isOperand(infix[i])) // if value, add to postfix
            postfix += infix[i];
        else if(infix[i] == '(')
            operatorStack.push(infix[i]);
        else if(infix[i] == ')'){
            while(operatorStack.top() != '('){
                postfix += operatorStack.top();
                operatorStack.pop();
            }
            operatorStack.pop();
        }
        else if(isOperator(infix[i])){
            while(!operatorStack.empty() && operatorStack.top() != '(' && secondOperatorGreater(infix[i], operatorStack.top())){
                postfix += operatorStack.top();
                operatorStack.pop();
            }
            operatorStack.push(infix[i]);
        }
        prev = infix[i];
        first = false;
    }
    while(!operatorStack.empty()){
        postfix += operatorStack.top();
        operatorStack.pop();
    }
    
    stack <char> operandStack;
    for(int i = 0; i < postfix.size(); i++){
        if(isOperand(postfix[i])){
            int opValue;
            if(!values.get(postfix[i], opValue))
                return 2;
            operandStack.push(opValue);
        }
        else{
            int op2 = operandStack.top();
            operandStack.pop();
            int op1 = operandStack.top();
            operandStack.pop();
            if(postfix[i] == '/' && op2 == 0)
                return 3;
            int operationResult = performOperation(postfix[i], op1, op2);
            operandStack.push(operationResult);
        }
    }
    result = operandStack.top();
    operandStack.pop();
    return 0;
}

bool matchingParantheses(string str){
    int open = 0;
    int close = 0;
    for(int i = 0; i < str.size(); i++){
        if(str[i] == '(')
            open++;
        else if(str[i] == ')')
            close++;
    }
    return open == close;
}

int performOperation(char task, int val1, int val2){
    if(task == '+')
        return val1 + val2;
    else if(task == '-')
        return val1 - val2;
    else if(task == '*')
        return val1 * val2;
    else{
        return val1/val2;
    }
}

bool secondOperatorGreater(char op1, char op2){
    if((op1 == '+' || op1 == '-') && (op2 == '+' || op2 == '-')) // if both low-level operations
        return true;
    if(op2 == '/' || op2 == '*') // if 2nd operator is high level, nothing can be greater
       return true;
    return false;
}

bool validSyntax(const char& prev, char curr, const bool& first, const bool& last){
    if(!isOperand(curr) && !isOperator(curr) && curr != '(' && curr != ')')
        // check if it's operator, operand, or paranthesis
        return false;
    
    if(first)
        return isOperand(curr) || curr == '('; // first could either be operand or (
    if(isOperand(prev) && isOperand(curr)) // no consecutive operands
        return false;
    if(isOperator(prev) && isOperator(curr)) // no consecutive operators
        return false;
    if(curr == '(' && !isOperator(prev) && prev != '(') // should be operator before or (
        return false;
    if(curr == ')' && !isOperand(prev) && prev != ')') // should be operand before or )
        return false;
    if(last)
        return isOperand(curr) || curr == ')'; // last should be operand or )
    return true;
}

bool isOperator(char ch){
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/');
}

bool isOperand(char ch){
    return (ch >= 'a' && ch <= 'z');
}

int main() {
    char var[] = {'a', 'c', 'l', 'u', 'o'};
    int val[] = {3, 5, 2, 11, 0};
    Map map;
    for(int i = 0; i < 5; i++)
        map.insert(var[i], val[i]);
    string postfix_trash;
    int ans;
    assert(evaluate(" u-c+l*a", map, postfix_trash, ans) == 0 && ans == 12);
    assert(evaluate("u  ", map, postfix_trash, ans) == 0 && ans == 11);
    assert(evaluate("a+c-l*u/a", map, postfix_trash, ans) == 0 && ans == 1);
    assert(evaluate("(", map, postfix_trash, ans) == 1 && ans == 1);
    assert(evaluate(") ", map, postfix_trash, ans) == 1);
    assert(evaluate(" ", map, postfix_trash, ans) == 1);
    assert(evaluate("c+*o", map, postfix_trash, ans) == 1);
    assert(evaluate("c+*z", map, postfix_trash, ans) == 1);
    assert(evaluate("a/(c+)", map, postfix_trash, ans) == 1 && ans == 1);
    assert(evaluate("a*(c+(l*u))", map, postfix_trash, ans) == 0 && ans == 81);
    assert(evaluate("8 + a", map, postfix_trash, ans) == 1 && ans == 81);
    assert(evaluate("C/o", map, postfix_trash, ans) == 1);
    assert(evaluate("c/o", map, postfix_trash, ans) == 3 && ans == 81);
    assert(evaluate("+", map, postfix_trash, ans) == 1);
    assert(evaluate("a & c", map, postfix_trash, ans) == 1);
    assert(evaluate("((((a)))", map, postfix_trash, ans) == 1 && ans == 81);
    
    char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
    int  vals[] = {  3,  -9,   6,   2,   4,   1  };
    Map m;
    for (int k = 0; vars[k] != '#'; k++)
        m.insert(vars[k], vals[k]);
    string pf;
    int answer;
    assert(evaluate("a+ e", m, pf, answer) == 0  && pf == "ae+"  &&  answer == -6);
    answer = 999;
    assert(evaluate("", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a+", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a i", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("ai", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("()", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("()o", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("y(o+u)", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("y(*o)", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a+E", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("(a+(i-o)", m, pf, answer) == 1  &&  answer == 999);
    // unary operators not allowed:
    assert(evaluate("-a", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a*b", m, pf, answer) == 2  && pf == "ab*"  &&  answer == 999);
    assert(evaluate("y +o *(a-u)  ", m, pf, answer) == 0  && pf == "yoau-*+"  &&  answer == -1);
    answer = 999;
    assert(evaluate("o/(y-y)", m, pf, answer) == 3  && pf == "oyy-/"  &&  answer == 999);
    assert(evaluate(" a  ", m, pf, answer) == 0  && pf == "a"  &&  answer == 3);
    assert(evaluate("((a))", m, pf, answer) == 0  && pf == "a"  &&  answer == 3);
    cerr << "Passed all tests." << endl;
}
