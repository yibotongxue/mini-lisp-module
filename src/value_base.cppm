export module value.base;

import std;

export enum class ValueType {
  kBoolean,
  kNumeric,
  kString,
  kNil,
  kSymbol,
  kPair,
  kBuiltinProc,
  kLambda
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
    return type_ == ValueType::kBoolean || type_ == ValueType::kNumeric ||
           type_ == ValueType::kString;
  }
  constexpr bool isNumber() const { return type_ == ValueType::kNumeric; }

  bool isList() const;
  bool isNotNilList() const;

  std::optional<std::vector<std::shared_ptr<Value>>> asVector() const;
  std::optional<std::string> asSymbol() const;
  std::optional<double> asNumber() const;
  bool asBoolean() const;

 protected:
  ValueType type_;
};
