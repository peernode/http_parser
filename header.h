#ifndef HTTP_PARSER_HEADER_H
#define HTTP_PARSER_HEADER_H

#include <string>
#include <vector>

struct header_item
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
  int content_length_;
  int range_start_;
  int range_end_;

  std::vector<header_item> headers_;

  int header_size_;
  bool header_ok_;

  http_header()
  {
	  reset();
  }

  void reset()
  {
	  http_version_major_ = 0;
	  http_version_major_ = 0;
	  status_code_ = 0;
	  phrase_ = "";
	  content_length_ = -1;
	  range_start_ = -1;
	  range_end_ = -1;
	  headers_.clear();

	  header_size_ = 0;
	  header_ok_ = false;
  }
};

#endif // HTTP_PARSER_HEADER_H

