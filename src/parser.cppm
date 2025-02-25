export module parser;

import token;
import value.base;
import std;

export class Parser {
 public:
  Parser(std::deque<std::unique_ptr<Token>> tokens)
      : tokens_(std::move(tokens)) {}

  std::shared_ptr<Value> parse();

 private:
  std::deque<std::unique_ptr<Token>> tokens_;

  std::shared_ptr<Value> parseTails();
};
