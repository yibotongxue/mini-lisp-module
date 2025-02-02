export module error;

import std;

export class SyntaxError : public std::runtime_error {
 public:
  using runtime_error::runtime_error;
};

export class LispError : public std::runtime_error {
 public:
  using runtime_error::runtime_error;
};
