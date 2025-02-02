import tokenizer;
import value;
import eval_env;
import parser;
import error;
import std;
import rjsj_mini_lisp_test;

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
  RjsjTest<TestCtx>({"Lv2", "Lv3", "Lv4", "Lv5", "Lv6"});
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

    } catch (const SyntaxError& e) {
      std::cerr << "Syntax Error: " << e.what() << std::endl;
    } catch (const LispError& e) {
      std::cerr << "Lisp Error: " << e.what() << std::endl;
    }
  }
}
