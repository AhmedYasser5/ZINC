#pragma once
#include "keywords.hpp"
#include "lexer.hpp"
#include <memory>
#include <vector>

std::unique_ptr<ASTNode> parse(std::vector<Token>::const_iterator begin,
                               std::vector<Token>::const_iterator end);
