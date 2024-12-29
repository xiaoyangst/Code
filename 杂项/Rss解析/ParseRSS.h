/**
  ******************************************************************************
  * @file           : ParseRSS.h
  * @author         : xy
  * @brief          : None
  * @attention      : None
  * @date           : 2024/11/11
  ******************************************************************************
  */

#ifndef STUDY__PARSERSS_H_
#define STUDY__PARSERSS_H_

#include "tinyxml2.h"
#include <string>
#include <regex>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
using namespace tinyxml2;
using namespace std;

struct RssItem
{
  string m_title;
  string m_link;
  string m_description;
  string m_content;
  RssItem(string  title,string  link,string  description,string  content)
    :m_title(std::move(title)),m_link(std::move(link)),m_description(std::move(description)),m_content(std::move(content))
  {}
};

class ParseRSS {
 public:
  ParseRSS() = default;
  ParseRSS(const string& xmlPath,string  dst);
  void parseRSS();
  void dump();
 private:
  std::string RemoveHtmlTags(const std::string& input);
 private:
  const std::string m_dst;
  XMLDocument m_doc;
  vector<RssItem> m_rss;
};

#endif //STUDY__PARSERSS_H_
