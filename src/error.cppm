export module error;

import std;

export class ExceptionWithLocation : public std::runtime_error {
 public:
  ExceptionWithLocation(
      std::string message,
      std::source_location location = std::source_location::current())
      : std::runtime_error(std::format("{}\nThrown from line {} in file {}",
                                       message, location.line(),
                                       location.file_name())) {}

  using runtime_error::what;
};

export class SyntaxError : public ExceptionWithLocation {
 public:
  using ExceptionWithLocation::ExceptionWithLocation;
};

export class LispError : public ExceptionWithLocation {
 public:
  using ExceptionWithLocation::ExceptionWithLocation;
};
