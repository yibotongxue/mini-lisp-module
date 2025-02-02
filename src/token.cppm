export module token;

import std;

export enum class TokenType {
  LEFT_PAREN,
  RIGHT_PAREN,
  QUOTE,
  QUASIQUOTE,
  UNQUOTE,
  DOT,
  BOOLEAN_LITERAL,
  NUMERIC_LITERAL,
  STRING_LITERAL,
  IDENTIFIER,
};

export class Token {
 private:
  TokenType type;

 protected:
  Token(TokenType type) : type{type} {}

 public:
  virtual ~Token() = default;

  static std::unique_ptr<Token> fromChar(char c);
  static std::unique_ptr<Token> dot();

  TokenType getType() const { return type; }
  virtual std::string toString() const;
};

export class BooleanLiteralToken : public Token {
 private:
  bool value;

 public:
  BooleanLiteralToken(bool value)
      : Token(TokenType::BOOLEAN_LITERAL), value{value} {}

  static std::unique_ptr<BooleanLiteralToken> fromChar(char c);

  bool getValue() const { return value; }
  std::string toString() const override;
};

export class NumericLiteralToken : public Token {
 private:
  double value;

 public:
  NumericLiteralToken(double value)
      : Token(TokenType::NUMERIC_LITERAL), value{value} {}

  double getValue() const { return value; }
  std::string toString() const override;
};

export class StringLiteralToken : public Token {
 private:
  std::string value;

 public:
  StringLiteralToken(const std::string& value)
      : Token(TokenType::STRING_LITERAL), value{value} {}

  const std::string& getValue() const { return value; }
  std::string toString() const override;
};

export class IdentifierToken : public Token {
 private:
  std::string name;

 public:
  IdentifierToken(const std::string& name)
      : Token(TokenType::IDENTIFIER), name{name} {}

  const std::string& getName() const { return name; }
  std::string toString() const override;
};

export std::ostream& operator<<(std::ostream& os, const Token& token);
