/**
 * @file Error.h
 * @brief Error handling classes for the Pascal-like programming language compiler
 *
 * This header defines classes for representing and managing compilation errors.
 * It provides functionality to capture error messages along with their source
 * location information (line and column numbers) and maintain a collection
 * of errors that can be reported to the user.
 *
 * Key components include:
 * - Error: Individual error representation with message and location
 * - Errors: Error collection manager for accumulating and displaying errors
 */
#ifndef ERROR_H
#define ERROR_H

#include <vector>
#include "ast.h"
using namespace std;

/**
 * @class Error
 * @brief Represents a single compilation error / warning 
 * 
 * Extends the Node class to capture error information including
 * the error message and source location where the error occurred.
 */
class Error : public Node
{
public:
	string Message; ///< The error message describing what went wrong
	/**
	 * @brief Constructor for Error
	 * @param message The error message describing the issue
	 * @param lin Line number in source code where error occurred
	 * @param col Column number in source code where error occurred
	 */
	Error(string message, int lin, int col);
};

/**
 * @class Errors
 * @brief Manages a collection of compilation errors and warnings
 * 
 * Provides functionality to accumulate multiple errors during compilation
 * and display them all at once to provide comprehensive error reporting.
 */
class Errors
{
public:
	vector<Error *> *errorStack; ///< Collection of errors encountered during compilation
	vector<Error *> *warningsStack; ///< Collection of warnings encountered during compilation
	/**
	 * @brief Constructor for Errors
	 * 
	 * Initializes an empty error and warning collections.
	 */
	Errors();
	/**
	 * @brief Adds a new error to the error collection
	 * @param message The error message describing the issue
	 * @param lin Line number in source code where error occurred
	 * @param col Column number in source code where error occurred
	 */
	void AddError(string message, int lin, int col);
	/**
	 * @brief Adds a new warning to the warning collection
	 * @param message The warning message describing the issue
	 * @param lin Line number in source code where error occurred
	 * @param col Column number in source code where error occurred
	 */
	void AddWarning(string message, int lin, int col);
	/**
	 * @brief Prints all accumulated errors to output
	 * 
	 * Displays all errors and warnings in the collection with their associated
	 * line and column information for debugging purposes.
	 */
	void Print();
};
#endif