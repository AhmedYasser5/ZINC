#include "visitor.hpp"
// A function to define how a binary expression accepts a visitor
// Call the visit function with this node as an argument
void BinaryExpr::accept(Visitor *visitor) { visitor->visit(this); }

// A function to define how an integer literal accepts a visitor
void IntLiteral::accept(Visitor *visitor) { visitor->visit(this); }

void Identifer::accept(Visitor *visitor) { visitor->visit(this); }

void Variable::accept(Visitor *visitor) { visitor->visit(this); }

void Declare::accept(Visitor *visitor) { visitor->visit(this); }

void Assign::accept(Visitor *visitor) { visitor->visit(this); }

void Statement::accept(Visitor *visitor) { visitor->visit(this); }

void Block::accept(Visitor *visitor) { visitor->visit(this); }
