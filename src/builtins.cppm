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

std::shared_ptr<Value> divide(
    const std::vector<std::shared_ptr<Value>>& params) {
  if (params.size() == 1) {
    auto num = params[0]->asNumber();
    if (!num) {
      throw LispError("Cannot divide a non-numeric value.");
    }
    if (*num == 0) {
      throw LispError("Divided by zero.");
    }
    return std::make_shared<NumericValue>(1.0 / *num);
  } else if (params.size() == 2) {
    auto x = params[0]->asNumber(), y = params[1]->asNumber();
    if (!x || !y) {
      throw LispError("Cannot divide a non-numeric value.");
    }
    if (*y == 0) {
      throw LispError("Divided by zero.");
    }
    return std::make_shared<NumericValue>(*x / *y);
  }
  throw LispError("The divide builtin should have one or two parameters.");
}

std::shared_ptr<Value> multiply(const std::vector<std::shared_ptr<Value>>& params) {
  double result = 1.0;
  for (auto&& i : params) {
    if (!i->isNumber()) {
      throw LispError("Cannot multiply a non-numeric value.");
    }
    result *= *(i->asNumber());
  }
  return std::make_shared<NumericValue>(result);
}

std::shared_ptr<Value> greater(const std::vector<std::shared_ptr<Value>>& params) {
  if (params.size() != 2) {
    throw LispError("The greater function requires exactly two arguments.");
  }
  auto x = params[0]->asNumber(), y = params[1]->asNumber();
  if (!x || !y) {
    throw LispError("Cannot compare non-numeric values.");
  }
  return std::make_shared<BooleanValue>(*x > *y);
}

std::shared_ptr<Value> print(
    const std::vector<std::shared_ptr<Value>>& params) {
  if (params.size() != 1) {
    throw LispError("Print function requires exactly one argument.");
  }
  std::cout << params[0]->toString() << std::endl;
  return std::make_shared<NilValue>();
}

export std::unordered_map<std::string, BuiltinFuncType> builtin_map = {
    {"+", add},
    {"/", divide},
    {"*", multiply},
    {">", greater},
    {"print", print},
};
