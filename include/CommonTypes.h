#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <string> 

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

// TypeEnum GetTypeEnumFromTypeNode(Type* typeNode) {
//     if (!typeNode) return VOID;
//     if (auto* st = dynamic_cast<StdType*>(typeNode)) {
//         return st->type;
//     } else if (auto* at = dynamic_cast<Array*>(typeNode)) {
//         if (at->stdType) {
//             switch (at->stdType->type) {
//                 case INTTYPE: return INT_ARRAY;
//                 case REALTYPE: return REAL_ARRAY;
//                 case BOOLTYPE: return BOOL_ARRAY;
//                 default: return VOID;
//             }
//         }
//     }
//     return VOID; // Fallback
// }

#endif // COMMON_TYPES_H