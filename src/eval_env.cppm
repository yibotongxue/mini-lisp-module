export module eval_env;

import value;
import <memory>;

export class EvalEnv {
 public:
  std::shared_ptr<Value> eval(std::shared_ptr<Value> expr);
};
