module eval_env;

import error;
import <vector>;
import builtins;
import <algorithm>;

EvalEnv::EvalEnv() {
  for (const auto& [key, value] : builtin_map) {
    symbolMap_[key] = std::make_shared<BuiltinProcValue>(value);
  }
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
    if (*vec[0]->asSymbol() == "define") {
      if (auto name = vec[1]->asSymbol()) {
        symbolMap_[*name] = eval(vec[2]);
        return std::make_shared<NilValue>();
      }
      throw LispError("Malformed define.");
    }
    auto proc = eval(vec[0]);
      std::vector<std::shared_ptr<Value>> params(vec.size() - 1);
      std::transform(vec.begin() + 1, vec.end(), params.begin(), [this](std::shared_ptr<Value> value) -> std::shared_ptr<Value> { return this->eval(value); });
      return apply(proc, params);
  }
  if (auto name = expr->asSymbol()) {
    if (symbolMap_.contains(*name)) {
      return symbolMap_[*name];
    }
    throw LispError("Variable " + *name + " not defined.");
  }
  throw LispError("Unimplemented");
}

std::shared_ptr<Value> EvalEnv::apply(std::shared_ptr<Value> proc, const std::vector<std::shared_ptr<Value>>& params) {
  auto proc_ptr = std::dynamic_pointer_cast<BuiltinProcValue>(proc);
  if (proc == nullptr) {
    throw LispError("Try to apply a not builtin proc value to params");
  }
  return proc_ptr->getFunc()(params);
}
