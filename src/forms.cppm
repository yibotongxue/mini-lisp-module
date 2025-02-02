export module forms;

import std;
import value;
import error;

export using SpecialFormType = std::function<std::shared_ptr<Value>(
    const std::vector<std::shared_ptr<Value>>&, EvalEnv&)>;

std::shared_ptr<Value> defineForm(
    const std::vector<std::shared_ptr<Value>>& args, EvalEnv& env) {
  if (auto name = args[0]->asSymbol()) {
    env.addSymbol(*name, env.eval(args[1]));
    return std::make_shared<NilValue>();
  }
  if (auto nameAndParams = args[0]->asVector()) {
    if (auto name = (*nameAndParams)[0]->asSymbol()) {
      std::vector<std::string> params;
      std::transform(nameAndParams->begin() + 1, nameAndParams->end(),
                     std::back_inserter(params),
                     [](std::shared_ptr<Value> param) -> std::string {
                       if (auto param_name = param->asSymbol()) {
                         return *param_name;
                       }
                       throw LispError("The param should be symbol.");
                     });
      std::vector<std::shared_ptr<Value>> body(args.begin() + 1, args.end());
      env.addSymbol(*name, std::make_shared<LambdaValue>(params, body, env.shared_from_this()));
      return std::make_shared<NilValue>();
    }
  }
  throw LispError("Malformed define.");
}

std::shared_ptr<Value> quoteForm(
    const std::vector<std::shared_ptr<Value>>& args, EvalEnv& env) {
  return args[0];
}

std::shared_ptr<Value> ifForm(const std::vector<std::shared_ptr<Value>>& args,
                              EvalEnv& env) {
  if (env.eval(args[0])->asBoolean()) {
    return env.eval(args[1]);
  } else {
    return env.eval(args[2]);
  }
}

std::shared_ptr<Value> andForm(const std::vector<std::shared_ptr<Value>>& args,
                               EvalEnv& env) {
  for (int i = 0; i < args.size(); i++) {
    auto eval_arg = env.eval(args[i]);
    if (!eval_arg->asBoolean()) {
      return std::make_shared<BooleanValue>(false);
    }
    if (i == args.size() - 1) {
      return eval_arg;
    }
  }
  return std::make_shared<BooleanValue>(true);
}

std::shared_ptr<Value> orForm(const std::vector<std::shared_ptr<Value>>& args,
                              EvalEnv& env) {
  for (auto&& arg : args) {
    auto eval_arg = env.eval(arg);
    if (eval_arg->asBoolean()) {
      return eval_arg;
    }
  }
  return std::make_shared<BooleanValue>(false);
}

std::shared_ptr<Value> lambdaForm(
    const std::vector<std::shared_ptr<Value>>& args, EvalEnv& env) {
  auto param_vec = args[0]->asVector();
  if (!param_vec) {
    throw LispError("The args of lambda should be a list.");
  }
  std::vector<std::string> params;
  std::ranges::transform(*param_vec, std::back_inserter(params),
                         [](std::shared_ptr<Value> param) -> std::string {
                           if (auto param_name = param->asSymbol()) {
                             return *param_name;
                           }
                           throw LispError("The param should be symbol.");
                         });
  std::vector<std::shared_ptr<Value>> body(args.begin() + 1, args.end());
  return std::make_shared<LambdaValue>(params, body, env.shared_from_this());
}

export std::unordered_map<std::string, SpecialFormType> form_map = {
    {"define", defineForm}, {"quote", quoteForm}, {"if", ifForm},
    {"and", andForm},       {"or", orForm},       {"lambda", lambdaForm},
};
