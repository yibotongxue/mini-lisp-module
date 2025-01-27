module value;

import <string>;
import <memory>;
import <iomanip>;
import <sstream>;

std::string BooleanValue::toString() const { return value_ ? "#t" : "#f"; }

std::string NumericValue::toString() const {
  if (double(int(value_)) == value_) {
    return std::to_string(int(value_));
  }
  return std::to_string(value_);
}

std::string StringValue::toString() const {
  std::stringstream ss;
  ss << std::quoted(value_);
  return ss.str();
}

std::string NilValue::toString() const { return "()"; }

std::string SymbolValue::toString() const { return symbol_; }

std::string PairValue::toString() const {
  return "(" + toStringRecursive(this) + ")";
}

std::string PairValue::toStringRecursive(const Value* value) {
  if (value->isPair()) {
    auto pair_ptr = dynamic_cast<const PairValue*>(value);
    auto left = pair_ptr->left_, right = pair_ptr->right_;
    if (right->isPair()) {
      return left->toString() + " " + toStringRecursive(right.get());
    }
    if (right->isNil()) {
      return left->toString();
    }
    return left->toString() + " . " + right->toString();
  }
  return value->toString();
}
