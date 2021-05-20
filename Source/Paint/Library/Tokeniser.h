#pragma once

/// <summary>
/// Tokeniser Class.
/// </summary>
class Tokeniser {
public:
  /// <summary>
  /// Split a haystack into tokens separated by needle.
  /// </summary>
  /// <param name="haystack"></param>
  /// <param name="needle"></param>
  /// <returns></returns>
  static std::vector<std::string> split(std::string haystack, const std::string& needle) {
    std::vector<std::string> tokens;

    size_t index = 0;

    while ((index = haystack.find(needle)) != std::string::npos) {
      tokens.push_back(haystack.substr(0, index));

      haystack.erase(0, index + needle.length());
    }

    tokens.push_back(haystack);

    return tokens;
  }
};