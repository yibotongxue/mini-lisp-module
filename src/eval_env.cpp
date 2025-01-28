module eval_env;

import error;
import <vector>;
import builtins;

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
  }
  if (auto name = expr->asSymbol()) {
    if (symbolMap_.find(*name) != symbolMap_.end()) {
      return symbolMap_[*name];
    }
    throw LispError("Variable " + *name + " not defined.");
  }
  throw LispError("Unimplemented");
}
