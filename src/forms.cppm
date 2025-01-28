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

export std::unordered_map<std::string, SpecialFormType> form_map = {
  {"define", defineForm},
};
