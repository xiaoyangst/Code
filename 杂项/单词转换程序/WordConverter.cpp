#include <fstream>
#include <iostream>
#include <sstream>
#include "WordConverter.h"


WordConverter::WordConverter(std::string m_target, std::string m_rule)
    : m_target(std::move(m_target)), m_rule(std::move(m_rule)) {
  buildRule();
  transferData();
}

void WordConverter::buildRule() {
  std::ifstream open_rule(m_rule);
  if (!open_rule.is_open()) {
    std::cerr << "open " << m_rule << "failed" << std::endl;
  }

  std::string line;
  while (std::getline(open_rule,line)){ // 读取文本内容一行的形式为 [key value]
    std::istringstream is(line);
    std::string key;
    std::string value;
    is >> key >> value;

    m_ruleMap.insert(std::make_pair(key,value));
  }
}
void WordConverter::transferData() {
  std::ifstream open_target(m_target);
  if (!open_target.is_open()) {
    std::cerr << "open " << m_target << "failed" << std::endl;
  }
  std::string tmp = "tmp.txt";
  std::ofstream write_target(tmp,std::ios::app);
  if (!write_target.is_open()) {
    std::cerr << "open " << tmp << "failed" << std::endl;
  }


  std::string line;
  while (std::getline(open_target,line)){ // 读取文本内容一行的形式为 [key value]
    std::istringstream is(line);
    std::string value;
    std::string data;
    std::ostringstream oss;
    while (is >> value){
      oss << findRule(value) << " ";
    }
    write_target << oss.str() << "\n";
  }
  // 删除 target.txt，把 tmp.txt 更名为 target.txt
  open_target.close();
  write_target.close();
  std::remove(m_target.c_str());
  std::rename(tmp.c_str(),m_target.c_str());
}
std::string WordConverter::findRule(std::string key) {
  auto item = m_ruleMap.find(key);
  if (item != m_ruleMap.end()){
    return item->second;
  }
  return key;
}
