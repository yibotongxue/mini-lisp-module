export module eval_env;

import value;
import <memory>;
import <unordered_map>;
import <string>;
import <map>;

export class EvalEnv: public std::enable_shared_from_this<EvalEnv> {
 public:
  EvalEnv();
  EvalEnv(std::shared_ptr<EvalEnv> parent);

  std::shared_ptr<Value> eval(std::shared_ptr<Value> expr);

  void addSymbol(const std::string& name, std::shared_ptr<Value> value);

 private:
  std::unordered_map<std::string, std::shared_ptr<Value>> symbolMap_;
  std::shared_ptr<EvalEnv> parent_;

  std::shared_ptr<Value> apply(
      std::shared_ptr<Value> proc,
      const std::vector<std::shared_ptr<Value>>& params);

  std::shared_ptr<Value> lookupBinding(const std::string& name);
};
