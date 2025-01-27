export module error;

import <stdexcept>;

export class SyntaxError : public std::runtime_error {
 public:
  using runtime_error::runtime_error;
};
