module parser;

import error;
import value;

std::shared_ptr<Value> Parser::parse() {
  auto token = std::move(tokens_[0]);
  tokens_.pop_front();
  if (token->getType() == TokenType::BOOLEAN_LITERAL) {
    return std::make_shared<BooleanValue>(
        dynamic_cast<BooleanLiteralToken&>(*token).getValue());
  }
  if (token->getType() == TokenType::NUMERIC_LITERAL) {
    return std::make_shared<NumericValue>(
        dynamic_cast<NumericLiteralToken&>(*token).getValue());
  }
  if (token->getType() == TokenType::STRING_LITERAL) {
    return std::make_shared<StringValue>(
        dynamic_cast<StringLiteralToken&>(*token).getValue());
  }
  if (token->getType() == TokenType::IDENTIFIER) {
    return std::make_shared<SymbolValue>(
        dynamic_cast<IdentifierToken&>(*token).getName());
  }
  if (token->getType() == TokenType::LEFT_PAREN) {
    return parseTails();
  }
  if (token->getType() == TokenType::QUOTE) {
    return std::make_shared<PairValue>(
        std::make_shared<SymbolValue>(std::string("quote")),
        std::make_shared<PairValue>(this->parse(),
                                    std::make_shared<NilValue>()));
  }
  if (token->getType() == TokenType::QUASIQUOTE) {
    return std::make_shared<PairValue>(
        std::make_shared<SymbolValue>(std::string("quasiquote")),
        std::make_shared<PairValue>(this->parse(),
                                    std::make_shared<NilValue>()));
  }
  if (token->getType() == TokenType::UNQUOTE) {
    return std::make_shared<PairValue>(
        std::make_shared<SymbolValue>(std::string("unquote")),
        std::make_shared<PairValue>(this->parse(),
                                    std::make_shared<NilValue>()));
  }
  throw SyntaxError("Unimplemented");
}

std::shared_ptr<Value> Parser::parseTails() {
  if (tokens_[0]->getType() == TokenType::RIGHT_PAREN) {
    tokens_.pop_front();
    return std::make_shared<NilValue>();
  }
  auto car = this->parse();
  if (tokens_[0]->getType() == TokenType::DOT) {
    tokens_.pop_front();
    auto cdr = this->parse();
    if (tokens_[0]->getType() != TokenType::RIGHT_PAREN) {
      throw SyntaxError("Should be right paren");
    }
    tokens_.pop_front();
    return std::make_shared<PairValue>(car, cdr);
  }
  auto cdr = this->parseTails();
  return std::make_shared<PairValue>(car, cdr);
}
