export module eval_env;

import value;
import <memory>;
import <unordered_map>;
import <string>;
import <map>;

export class EvalEnv {
 public:
  std::shared_ptr<Value> eval(std::shared_ptr<Value> expr);

 private:
  std::unordered_map<std::string, std::shared_ptr<Value>> symbolMap_;
};
