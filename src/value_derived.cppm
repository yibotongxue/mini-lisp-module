export module value.derived;

import std;
import value.base;
import eval_env;

export class BooleanValue : public Value {
 public:
  BooleanValue(bool value) : Value(ValueType::kBoolean), value_(value) {}

  std::string toString() const override;
  bool getValue() const { return value_; }

 private:
  bool value_;
};

export class NumericValue : public Value {
 public:
  NumericValue(double value) : Value(ValueType::kNumeric), value_(value) {}

  std::string toString() const override;
  double getValue() const { return value_; }

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
  std::string getSymbol() const { return symbol_; }

 private:
  std::string symbol_;
};

export class PairValue : public Value {
 public:
  PairValue(std::shared_ptr<Value> left, std::shared_ptr<Value> right)
      : Value(ValueType::kPair), left_(left), right_(right) {}

  std::shared_ptr<Value> getLeft() const { return left_; }
  std::shared_ptr<Value> getRight() const { return right_; }

  std::string toString() const override;

 private:
  std::shared_ptr<Value> left_;
  std::shared_ptr<Value> right_;

  static std::string toStringRecursive(const Value* value);
};

export class CallableValue : public Value {
 public:
  CallableValue(ValueType type) : Value(type) {}

  virtual std::shared_ptr<Value> apply(const std::vector<std::shared_ptr<Value>>& params) = 0;
  std::string toString() const override;
};

export using BuiltinFuncType = std::function<std::shared_ptr<Value>(
    const std::vector<std::shared_ptr<Value>>&)>;

export class BuiltinProcValue : public CallableValue {
 public:
  BuiltinProcValue(BuiltinFuncType func)
      : CallableValue(ValueType::kBuiltinProc), func_(func) {}

  std::shared_ptr<Value> apply(const std::vector<std::shared_ptr<Value>>& params) override;

 private:
  BuiltinFuncType func_;
};

export class LambdaValue : public CallableValue {
 public:
  LambdaValue(const std::vector<std::string>& params,
              const std::vector<std::shared_ptr<Value>>& body,
              std::shared_ptr<EvalEnv> env);

  std::shared_ptr<Value> apply(const std::vector<std::shared_ptr<Value>>& params) override;

 private:
  std::vector<std::string> params_;
  std::vector<std::shared_ptr<Value>> body_;
  std::shared_ptr<EvalEnv> env_;
};
