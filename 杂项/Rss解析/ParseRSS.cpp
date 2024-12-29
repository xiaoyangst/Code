#include "ParseRSS.h"
#include <utility>
#include <fstream>
#include <iostream>
#include <regex>

using namespace std;
using namespace tinyxml2;

ParseRSS::ParseRSS(const string& xmlPath, string dst)
    : m_dst(std::move(dst)) {
  if (m_doc.LoadFile(xmlPath.c_str()) != XML_SUCCESS) {
    cerr << "LoadFile error" << endl;
  }
}

void ParseRSS::parseRSS() {
  XMLElement* channel = m_doc.FirstChildElement("rss")->FirstChildElement("channel");
  if (!channel) {
    cerr << "<channel> element not found." << endl;
    return;
  }

  for (auto item = channel->FirstChildElement("item"); item != nullptr; item = item->NextSiblingElement("item")) {
    const char* title = item->FirstChildElement("title") ? item->FirstChildElement("title")->GetText() : "";
    const char* link = item->FirstChildElement("link") ? item->FirstChildElement("link")->GetText() : "";
    const char* content = item->FirstChildElement("content") ? item->FirstChildElement("content")->GetText() : "";
    const char* descriptionText = item->FirstChildElement("description") ? item->FirstChildElement("description")->GetText() : "";

    string m_title = title ? title : "";
    string m_link = link ? link : "";
    string m_content = content ? content : "";
    string m_description = RemoveHtmlTags(descriptionText ? descriptionText : "");

    m_rss.emplace_back(m_title, m_link, m_description, m_content);
  }
}

void ParseRSS::dump() {
  ofstream file(m_dst, ios::out);  // 覆盖写入
  if (!file.is_open()) {
    cerr << "Failed to open file for writing." << endl;
    return;
  }

  int docid = 0;
  for (const auto& item : m_rss) {
    file << "<doc>\n";
    file << "\t<docid>" << ++docid << "</docid>\n";
    file << "\t<title>" << item.m_title << "</title>\n";
    file << "\t<link>" << item.m_link << "</link>\n";
    file << "\t<description>" << item.m_description << "</description>\n";
    file << "\t<content>" << item.m_content << "</content>\n";
    file << "</doc>\n";
  }
  file.close();
}

std::string ParseRSS::RemoveHtmlTags(const std::string& input) {
  // 去除 HTML 标签
  std::string noHtml = std::regex_replace(input, std::regex("<[^>]*>"), "");

  // 替换常见的 HTML 编码
  std::unordered_map<std::string, std::string> htmlEntities = {
      {"&#8211;", "-"},
      {"&#8217;", "'"},
      // 可以添加更多 HTML 编码替换
  };
  for (const auto& entity : htmlEntities) {
    noHtml = std::regex_replace(noHtml, std::regex(entity.first), entity.second);
  }

  // 去除重复的 "Read More" 字样
  noHtml = std::regex_replace(noHtml, std::regex("Read More"), "");

  // 去除多余的空格
  noHtml = std::regex_replace(noHtml, std::regex("\\s+"), " ");

  return noHtml;
}
