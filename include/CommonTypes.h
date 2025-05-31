// include/CommonTypes.h
#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <string> // For string in TypeEnumToString

/**
 * @enum TypeEnum
 * @brief Represents the basic types supported in the language
 */
enum TypeEnum
{
    INTTYPE, ///< Integer type
    REALTYPE, ///< Real (floating point) type
    BOOLTYPE,  ///< Boolean type
    INT_ARRAY,
    REAL_ARRAY,
    BOOL_ARRAY,
    VOID // This is the 'VOID' that is undeclared.
};



inline std::string TypeEnumToString(TypeEnum t){
    switch (t)
    {
    case  INTTYPE:
        return  "Int";
    case  REALTYPE:
        return  "Real";
    case  BOOLTYPE:
        return  "Bool";
    case  INT_ARRAY:
        return  "IntArray";
    case  REAL_ARRAY:
        return  "RealArray";
    case  BOOL_ARRAY:
        return  "BoolArray";
    case  VOID:
        return  "Void";
    default:
        return "INVALID Enum";
    }
}


#endif // COMMON_TYPES_H