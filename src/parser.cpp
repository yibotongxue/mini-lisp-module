module parser;

import error;

std::shared_ptr<Value> Parser::parse() {
  auto token = std::move(tokens_[0]);
  if (token->getType() == TokenType::BOOLEAN_LITERAL) {
    return std::make_shared<BooleanValue>(dynamic_cast<BooleanLiteralToken&>(*token).getValue());
  }
  if (token->getType() == TokenType::NUMERIC_LITERAL) {
    return std::make_shared<NumericValue>(dynamic_cast<NumericLiteralToken&>(*token).getValue());
  }
  if (token->getType() == TokenType::STRING_LITERAL) {
    return std::make_shared<StringValue>(dynamic_cast<StringLiteralToken&>(*token).getValue());
  }
  if (token->getType() == TokenType::IDENTIFIER) {
    return std::make_shared<SymbolValue>(dynamic_cast<IdentifierToken&>(*token).getName());
  }
  throw SyntaxError("Unimplemented");
}
