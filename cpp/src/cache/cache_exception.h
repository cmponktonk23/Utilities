#pragma once

#include <stdexcept>

class CacheException : public std::runtime_error {
 public:
  explicit CacheException(const std::string &message) : std::runtime_error(message) {}
};