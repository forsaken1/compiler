#ifndef EXPRESSION_H
#define EXPRESSION_H

class Expression {
	

public:
	
};

//--ExpressionVar---
class ExpressionVar: public Expression {
	

public:
	
};

//--ExpressionConst--
class ExpressionConst: public Expression {
	

public:
	
};

class IntegerOperation: public ExpressionConst {
	

public:
	
};

class FloatOperation: public ExpressionConst {
	

public:
	
};

class StringOperation: public ExpressionConst {
	

public:
	
};

//--ExpressionOperation--
class ExpressionOperation: public Expression {
	

public:
	
};

class BinaryOperation: public ExpressionOperation {
	

public:
	
};

class UnaryOperation: public ExpressionOperation {
	

public:
	
};

//--ExpressionFunction--
class ExpressionFunction: public Expression {
	

public:
	
};

#endif EXPRESSION_H