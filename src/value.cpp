module value;

import std;
import error;
import builtins;
import forms;

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

std::shared_ptr<Value> EvalEnv::eval(std::shared_ptr<Value> expr) {
  if (expr->isSelfEvaluating()) {
    return expr;
  }
  if (expr->isNil()) {
    throw LispError("Evaluating nil is prohibited.");
  }
  if (expr->isNotNilList()) {
    auto vec = *expr->asVector();
    if (auto name = vec[0]->asSymbol()) {
      if (form_map.contains(*name)) {
        std::vector<std::shared_ptr<Value>> args(vec.begin() + 1, vec.end());
        return form_map[*name](args, *this);
      }
    }
    auto proc = eval(vec[0]);
    std::vector<std::shared_ptr<Value>> params(vec.size() - 1);
    std::transform(
        vec.begin() + 1, vec.end(), params.begin(),
        [this](std::shared_ptr<Value> value) -> std::shared_ptr<Value> {
          return this->eval(value);
        });
    return apply(proc, params);
  }
  if (auto name = expr->asSymbol()) {
    if (auto value = lookupBinding(*name)) {
      return value;
    }
    throw LispError("Variable " + *name + " not defined.");
  }
  throw LispError("Unimplemented");
}

void EvalEnv::addSymbol(const std::string& name, std::shared_ptr<Value> value) {
  symbolMap_[name] = value;
}

std::shared_ptr<EvalEnv> EvalEnv::createChild(
    const std::vector<std::string>& names,
    const std::vector<std::shared_ptr<Value>>& params) {
  std::shared_ptr<EvalEnv> env =
      std::shared_ptr<EvalEnv>(new EvalEnv(this->shared_from_this()));
  for (int i = 0; i < names.size(); i++) {
    env->addSymbol(names[i], params[i]);
  }
  return env;
}

std::shared_ptr<EvalEnv> EvalEnv::createGlobal() {
  static std::shared_ptr<EvalEnv> instance(new EvalEnv);
  return instance;
}

EvalEnv::EvalEnv() : parent_(nullptr) {
  for (const auto& [key, value] : builtin_map) {
    symbolMap_[key] = std::make_shared<BuiltinProcValue>(value);
  }
}

EvalEnv::EvalEnv(std::shared_ptr<EvalEnv> parent) : parent_(parent) {
  for (const auto& [key, value] : builtin_map) {
    symbolMap_[key] = std::make_shared<BuiltinProcValue>(value);
  }
}

std::shared_ptr<Value> EvalEnv::apply(
    std::shared_ptr<Value> proc,
    const std::vector<std::shared_ptr<Value>>& params) {
  auto proc_ptr = std::dynamic_pointer_cast<CallableValue>(proc);
  if (proc_ptr == nullptr) {
    throw LispError("Try to apply a not callable proc value to params");
  }
  return proc_ptr->apply(params);
}

std::shared_ptr<Value> EvalEnv::lookupBinding(const std::string& name) {
  if (symbolMap_.contains(name)) {
    return symbolMap_[name];
  }
  if (parent_ != nullptr) {
    return parent_->lookupBinding(name);
  }
  return nullptr;
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
