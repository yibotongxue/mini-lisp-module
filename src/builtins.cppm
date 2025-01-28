export module builtins;

import <unordered_map>;
import <string>;
import <memory>;
import <vector>;
import <functional>;
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

export std::unordered_map<std::string, BuiltinFuncType> builtin_map = {
  {"+", add},
};
