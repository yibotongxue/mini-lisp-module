module value.base;

import value.derived;

bool Value::isList() const {
  if (type_ == ValueType::kNil) {
    return true;
  }
  if (type_ == ValueType::kPair) {
    return dynamic_cast<const PairValue*>(this)->getRight()->isList();
  }
  return false;
}

bool Value::isNotNilList() const { return !isNil() && isList(); }

std::optional<std::vector<std::shared_ptr<Value>>> Value::asVector() const {
  if (!isList()) {
    return std::nullopt;
  }
  if (type_ == ValueType::kNil) {
    return std::vector<std::shared_ptr<Value>>();
  }
  auto pair_ptr = dynamic_cast<const PairValue*>(this);
  std::vector<std::shared_ptr<Value>> result;
  result.push_back(pair_ptr->getLeft());
  auto right_vec = *pair_ptr->getRight()->asVector();
  result.insert(result.end(), right_vec.begin(), right_vec.end());
  return result;
}

std::optional<std::string> Value::asSymbol() const {
  if (type_ == ValueType::kSymbol) {
    return dynamic_cast<const SymbolValue*>(this)->getSymbol();
  }
  return std::nullopt;
}

std::optional<double> Value::asNumber() const {
  if (type_ == ValueType::kNumeric) {
    return dynamic_cast<const NumericValue*>(this)->getValue();
  }
  return std::nullopt;
}

bool Value::asBoolean() const {
  if (type_ == ValueType::kBoolean) {
    return dynamic_cast<const BooleanValue*>(this)->getValue();
  }
  return true;
}
