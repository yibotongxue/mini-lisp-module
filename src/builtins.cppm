export module builtins;

import <unordered_map>;
import <string>;
import <memory>;
import <vector>;
import <functional>;
import <iostream>;
import value;
import error;

std::shared_ptr<Value> add(const std::vector<std::shared_ptr<Value>>& params) {
  double result = 0.0;
  for (auto&& i : params) {
    if (!i->isNumber()) {
      throw LispError("Cannot add a non-numeric value.");
    }
    result += *(i->asNumber());
  }
  return std::make_shared<NumericValue>(result);
}

std::shared_ptr<Value> print(const std::vector<std::shared_ptr<Value>>& params) {
  if (params.size() != 1) {
    throw LispError("Print function requires exactly one argument.");
  }
  std::cout << params[0]->toString() << std::endl;
  return std::make_shared<NilValue>();
}

export std::unordered_map<std::string, BuiltinFuncType> builtin_map = {
  {"+", add},
  {"print", print},
};
