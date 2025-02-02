module eval_env;

import error;
import builtins;
import forms;
import value;

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
