import tokenizer;
import value;
import parser;
import eval_env;
import std;

#include "rjsj_test.hpp"

struct TestCtx {
  std::shared_ptr<EvalEnv> env = EvalEnv::createGlobal();
  std::string eval(std::string input) {
    auto tokens = Tokenizer::tokenize(input);
    Parser parser(std::move(tokens));
    auto value = parser.parse();
    auto result = env->eval(std::move(value));
    return result->toString();
  }
};

int main() {
  RJSJ_TEST(TestCtx, Lv2, Lv3, Lv4, Lv5, Lv6);
  auto env = EvalEnv::createGlobal();
  while (true) {
    try {
      std::cout << ">>> ";
      std::string line;
      std::getline(std::cin, line);
      if (std::cin.eof()) {
        std::exit(0);
      }
      auto tokens = Tokenizer::tokenize(line);
      Parser parser(std::move(tokens));
      auto value = parser.parse();
      auto result = env->eval(std::move(value));
      std::cout << result->toString() << std::endl;

    } catch (std::runtime_error& e) {
      std::cerr << "Error: " << e.what() << std::endl;
    }
  }
}
