/**
  ******************************************************************************
  * @file           : WordConverter.h
  * @author         : xy
  * @brief          : None
  * @attention      : None
  * @date           : 2024/11/20
  ******************************************************************************
  */

#ifndef STUDY__WORDCONVERTER_H_
#define STUDY__WORDCONVERTER_H_

#include <string>
#include <unordered_map>

class WordConverter {
 public:
  WordConverter(std::string m_target, std::string m_rule);
 private:
  void buildRule();
  void transferData();
  std::string findRule(std::string key);
 private:
  std::string m_rule;
  std::string m_target;
  std::unordered_map<std::string, std::string> m_ruleMap;
};

#endif //STUDY__WORDCONVERTER_H_
