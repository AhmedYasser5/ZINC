#include "visitor.hpp"

void Block::accept(Visitor *visitor) { visitor->visit(this); }

void Comparison::accept(Visitor *visitor) { visitor->visit(this); }

void Expression::accept(Visitor *visitor) { visitor->visit(this); }

void Term::accept(Visitor *visitor) { visitor->visit(this); }

void Unary::accept(Visitor *visitor) { visitor->visit(this); }

void DoubleLiteral::accept(Visitor *visitor) { visitor->visit(this); }

void StringLiteral::accept(Visitor *visitor) { visitor->visit(this); }

void Identifer::accept(Visitor *visitor) { visitor->visit(this); }

void Let::accept(Visitor *visitor) { visitor->visit(this); }

void Print::accept(Visitor *visitor) { visitor->visit(this); }

void Input::accept(Visitor *visitor) { visitor->visit(this); }

void If::accept(Visitor *visitor) { visitor->visit(this); }

void While::accept(Visitor *visitor) { visitor->visit(this); }

void Goto::accept(Visitor *visitor) { visitor->visit(this); }

void Label::accept(Visitor *visitor) { visitor->visit(this); }
