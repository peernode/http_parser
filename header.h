#ifndef HTTP_PARSER_HEADER_H
#define HTTP_PARSER_HEADER_H

#include <string>
#include <vector>

struct header
{
  std::string name_;
  std::string value_;
};

/// A http header received from a server.
struct http_header
{
  int http_version_major_;
  int http_version_minor_;
  int status_code_;
  std::string phrase_;
  std::vector<header> headers_;

};

#endif // HTTP_PARSER_HEADER_H

