export module value;

import <string>;
import <memory>;

export enum class ValueType {
  kBoolean,
  kNumeric,
  kString,
  kNil,
  kSymbol,
  kPair
};

export class Value {
 public:
  Value(ValueType type) : type_(type) {}

  virtual ~Value() = default;

  virtual std::string toString() const = 0;

  constexpr ValueType getType() const { return type_; }
  constexpr bool isNil() const { return type_ == ValueType::kNil; }
  constexpr bool isPair() const { return type_ == ValueType::kPair; }
  constexpr bool isSelfEvaluating() const {
    return type_ == ValueType::kBoolean || type_ == ValueType::kNumeric || type_ == ValueType::kString;
  }

 protected:
  ValueType type_;
};

export class BooleanValue : public Value {
 public:
  BooleanValue(bool value) : Value(ValueType::kBoolean), value_(value) {}

  std::string toString() const override;

 private:
  bool value_;
};

export class NumericValue : public Value {
 public:
  NumericValue(double value) : Value(ValueType::kNumeric), value_(value) {}

  std::string toString() const override;

 private:
  double value_;
};

export class StringValue : public Value {
 public:
  StringValue(const std::string& value)
      : Value(ValueType::kString), value_(value) {}

  std::string toString() const override;

 private:
  std::string value_;
};

export class NilValue : public Value {
 public:
  NilValue() : Value(ValueType::kNil) {}

  std::string toString() const override;
};

export class SymbolValue : public Value {
 public:
  SymbolValue(const std::string& symbol)
      : Value(ValueType::kSymbol), symbol_(symbol) {}

  std::string toString() const override;

 private:
  std::string symbol_;
};

export class PairValue : public Value {
 public:
  PairValue(std::shared_ptr<Value> left, std::shared_ptr<Value> right)
      : Value(ValueType::kPair), left_(left), right_(right) {}

  std::string toString() const override;

 private:
  std::shared_ptr<Value> left_;
  std::shared_ptr<Value> right_;

  static std::string toStringRecursive(const Value* value);
};
