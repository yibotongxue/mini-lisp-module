export module tokenizer;

import token;
import std;

export class Tokenizer {
 private:
  std::unique_ptr<Token> nextToken(int& pos);
  std::deque<std::unique_ptr<Token>> tokenize();

  std::string input;
  Tokenizer(const std::string& input) : input{input} {}

 public:
  static std::deque<std::unique_ptr<Token>> tokenize(const std::string& input);
};
