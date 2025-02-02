module value;

import eval_env;
import std;

import error;

bool Value::isList() const {
  if (type_ == ValueType::kNil) {
    return true;
  }
  if (type_ == ValueType::kPair) {
    return dynamic_cast<const PairValue*>(this)->getRight()->isList();
  }
  return false;
}

bool Value::isNotNilList() const { return !isNil() && isList(); }

std::optional<std::vector<std::shared_ptr<Value>>> Value::asVector() const {
  if (!isList()) {
    return std::nullopt;
  }
  if (type_ == ValueType::kNil) {
    return std::vector<std::shared_ptr<Value>>();
  }
  auto pair_ptr = dynamic_cast<const PairValue*>(this);
  std::vector<std::shared_ptr<Value>> result;
  result.push_back(pair_ptr->getLeft());
  auto right_vec = *pair_ptr->getRight()->asVector();
  result.insert(result.end(), right_vec.begin(), right_vec.end());
  return result;
}

std::optional<std::string> Value::asSymbol() const {
  if (type_ == ValueType::kSymbol) {
    return dynamic_cast<const SymbolValue*>(this)->getSymbol();
  }
  return std::nullopt;
}

std::optional<double> Value::asNumber() const {
  if (type_ == ValueType::kNumeric) {
    return dynamic_cast<const NumericValue*>(this)->getValue();
  }
  return std::nullopt;
}

bool Value::asBoolean() const {
  if (type_ == ValueType::kBoolean) {
    return dynamic_cast<const BooleanValue*>(this)->getValue();
  }
  return true;
}

std::string BooleanValue::toString() const { return value_ ? "#t" : "#f"; }

std::string NumericValue::toString() const {
  if (double(int(value_)) == value_) {
    return std::to_string(int(value_));
  }
  return std::to_string(value_);
}

std::string StringValue::toString() const {
  std::stringstream ss;
  ss << std::quoted(value_);
  return ss.str();
}

std::string NilValue::toString() const { return "()"; }

std::string SymbolValue::toString() const { return symbol_; }

std::string PairValue::toString() const {
  return "(" + toStringRecursive(this) + ")";
}

std::string PairValue::toStringRecursive(const Value* value) {
  if (value->isPair()) {
    auto pair_ptr = dynamic_cast<const PairValue*>(value);
    auto left = pair_ptr->left_, right = pair_ptr->right_;
    if (right->isPair()) {
      return left->toString() + " " + toStringRecursive(right.get());
    }
    if (right->isNil()) {
      return left->toString();
    }
    return left->toString() + " . " + right->toString();
  }
  return value->toString();
}

std::string CallableValue::toString() const { return "#<procedure>"; }

std::shared_ptr<Value> BuiltinProcValue::apply(const std::vector<std::shared_ptr<Value>>& params) {
  return func_(params);
}

LambdaValue::LambdaValue(const std::vector<std::string>& params,
              const std::vector<std::shared_ptr<Value>>& body,
              std::shared_ptr<EvalEnv> env)
      : CallableValue(ValueType::kLambda), params_(params), body_(body), env_(env) {}

std::shared_ptr<Value> LambdaValue::apply(const std::vector<std::shared_ptr<Value>>& params) {
  if (params.size() != params_.size()) {
    throw LispError("The parameters size not match in lambda");
  }
  auto env = env_->createChild(params_, params);
  for (int i = 0; i < body_.size(); i++) {
    auto eval_value = env->eval(body_[i]);
    if (i == body_.size() - 1) {
      return eval_value;
    }
  }
  return std::make_shared<NilValue>();
}
