export module eval_env;

import std;
import value.base;

export class EvalEnv: public std::enable_shared_from_this<EvalEnv> {
 public:

  std::shared_ptr<Value> eval(std::shared_ptr<Value> expr);

  void addSymbol(const std::string& name, std::shared_ptr<Value> value);

  std::shared_ptr<EvalEnv> createChild(const std::vector<std::string>& names, const std::vector<std::shared_ptr<Value>>& params);

  static std::shared_ptr<EvalEnv> createGlobal();

 private:
  std::unordered_map<std::string, std::shared_ptr<Value>> symbolMap_;
  std::shared_ptr<EvalEnv> parent_;

  EvalEnv();
  EvalEnv(std::shared_ptr<EvalEnv> parent);

  std::shared_ptr<Value> apply(
      std::shared_ptr<Value> proc,
      const std::vector<std::shared_ptr<Value>>& params);

  std::shared_ptr<Value> lookupBinding(const std::string& name);
};
