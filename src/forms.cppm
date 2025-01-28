export module forms;

import <memory>;
import <vector>;
import <functional>;
import <unordered_map>;
import <string>;
import value;
import eval_env;
import error;

export using SpecialFormType = std::function<std::shared_ptr<Value>(const std::vector<std::shared_ptr<Value>>&, EvalEnv&)>;

std::shared_ptr<Value> defineForm(const std::vector<std::shared_ptr<Value>>& args, EvalEnv& env) {
  if (auto name = args[0]->asSymbol()) {
    env.addSymbol(*name, env.eval(args[1]));
    return std::make_shared<NilValue>();
  }
  throw LispError("Malformed define.");
}

std::shared_ptr<Value> quoteForm(const std::vector<std::shared_ptr<Value>>& args, EvalEnv& env) {
  return args[0];
}

std::shared_ptr<Value> ifForm(const std::vector<std::shared_ptr<Value>>& args, EvalEnv& env) {
  if (env.eval(args[0])->asBoolean()) {
    return env.eval(args[1]);
  } else {
    return env.eval(args[2]);
  }
}

std::shared_ptr<Value> andForm(const std::vector<std::shared_ptr<Value>>& args, EvalEnv& env) {
  for (auto&& arg : args) {
    if (!env.eval(arg)->asBoolean()) {
      return std::make_shared<BooleanValue>(false);
    }
  }
  return std::make_shared<BooleanValue>(true);
}

std::shared_ptr<Value> orForm(const std::vector<std::shared_ptr<Value>>& args, EvalEnv& env) {
  for (auto&& arg : args) {
    if (env.eval(arg)->asBoolean()) {
      return std::make_shared<BooleanValue>(true);
    }
  }
  return std::make_shared<BooleanValue>(false);
}

export std::unordered_map<std::string, SpecialFormType> form_map = {
  {"define", defineForm},
  {"quote", quoteForm},
  {"if", ifForm},
  {"and", andForm},
  {"or", orForm},
};
