#include <iostream>
#include "Error.h"

using namespace std;


Error::Error(string message, int lin, int col) : Node(lin, col)
{
    this->Message = message;
}

Errors::Errors()
{
    this->errorStack = new vector<Error *>;
    this->warningsStack = new vector<Error *>;
}

void Errors::AddError(string message, int lin, int col)
{
    Error *newError = new Error(message, lin, col);
    this->errorStack->push_back(newError);
}

void Errors::AddWarning(string message, int lin, int col)
{
    Error *newWarning = new Error(message, lin, col);
    this->warningsStack->push_back(newWarning);
}

void Errors::Print()
{
    int n = this->errorStack->size();
    cout << "** Found " << n << " Errors **" << endl;
    for (int i = 0; i < n; i++)
    {
        cout << "Error:" << this->errorStack->at(i)->line << ":" << this->errorStack->at(i)->column;
        cout << " Message: " << this->errorStack->at(i)->Message << endl;
    }
    cout << "Fix these then try to compile again ;)" << endl;
}
