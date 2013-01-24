#include "lib.h"

string GetRandomId(string str) {
	string random = str;

	for(int j = 0; j < 10; j++)
		random += to_string( (_Longlong)(rand() % 10) );

	return random;
}

string GetTokenName(TokenValue _tv) {
	switch(_tv) {
		case KEYWORD_RETURN:			return "return";
		case KEYWORD_CONTINUE:			return "continue";
		case KEYWORD_BREAK:				return "break";
		case KEYWORD_GOTO:				return "goto";

		case OPER_BINARY_AND:			return "&";
		case OPER_AND:					return "&&";
		case OPER_BINARY_OR:			return "|";
		case OPER_OR:					return "||";
		case OPER_PLUS:					return "+";
		case OPER_MINUS:				return "-";
		case OPER_MULTIPLY:				return "*";
		case OPER_DIVIDE:				return "/";
		case OPER_DIVIDE_BY_MOD:		return "%";
		case OPER_ASSIGN:				return "=";
		case COMMA:						return ",";
		case OPER_EQUAL:				return "==";
		case OPER_NOT_EQUAL:			return "!=";
		case OPER_MORE:					return ">";
		case OPER_LESS:					return "<";
		case OPER_MORE_OR_EQUAL:		return ">=";
		case OPER_LESS_OR_EQUAL:		return "<=";
		case OPER_PLUS_ASSIGN:			return "+=";
		case OPER_MINUS_ASSIGN:			return "-=";
		case OPER_MULTIPLY_ASSIGN:		return "*=";
		case OPER_DIVIDE_ASSIGN:		return "/=";
		case OPER_DIVIDE_BY_MOD_ASSIGN:	return "%=";
		case OPER_BINARY_AND_ASSIGN:	return "&=";
		case OPER_BINARY_OR_ASSIGN:		return "|=";
		case OPER_SHIFT_LEFT:			return "<<";
		case OPER_SHIFT_RIGHT:			return ">>";
		case OPER_EXCLUSIVE_OR:			return "^";
		case OPER_EXCLUSIVE_OR_ASSIGN:	return "^=";
		case OPER_ARROW:				return "->";
		case OPER_POINT:				return ".";
		case OPER_DEC:					return "--";
		case OPER_INC:					return "++";
		case OPER_BINARY_NOT:			return "~";
		case OPER_NOT:					return "!";
		case OPER_XCHANGE:				return "<>";

		case SIZE_OF:					return "sizeof";
		case CAST_INT:					return "int";
		case CAST_CHAR:					return "char";
		case CAST_FLOAT:				return "float";
		case ARRAY_INDEX:				return "[]";

		default:						return "";
	}
}