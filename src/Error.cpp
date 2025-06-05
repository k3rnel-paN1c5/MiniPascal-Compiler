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

void Errors::PrintWarnings()
{
    int n = this->warningsStack->size();
    if(n == 0)
        return;
    cout << "** Found " << n << " Warnings **" << endl;
    for (int i = 0; i < n; i++)
    {
        cout << "Warnning:" << this->warningsStack->at(i)->line << ":" << this->warningsStack->at(i)->column;
        cout << " Message: " << this->warningsStack->at(i)->Message << endl;
    }
    if(this->errorStack->size() == 0)
        cout << "Not a problem, but might face unexpected behaviour" << endl;
}

void Errors::PrintErrors()
{
    int n = this->errorStack->size();
    if(n == 0)
        return;
    cout << "** Found " << n << " Errors **" << endl;
    for (int i = 0; i < n; i++)
    {
        cout << "Error:" << this->errorStack->at(i)->line << ":" << this->errorStack->at(i)->column;
        cout << " Message: " << this->errorStack->at(i)->Message << endl;
    }
    cout << "Fix these then try to compile again ;)" << endl;
}
