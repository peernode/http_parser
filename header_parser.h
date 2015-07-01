/*
 * based on boost_1_51_0\doc\html\boost_asio\example\http\server3
 */

#ifndef HTTP_PARSER_HEADER_PARSER_HPP
#define HTTP_PARSER_HEADER_PARSER_HPP

struct http_header;

enum parse_state
{
	HEADER_ERROR = -1,
	HEADER_CONTINUE = 0,
	HEADER_OK = 1
};

/// Parser for incoming requests.
class header_parser
{
public:
	/// Construct ready to parse the http header.
	header_parser();

	void reset(void);

	/// Parse some data. The HEADER_OK return value is true when a complete HTTP HEADER
	/// has been parsed, HEADER_ERROR if the data is invalid, HEADER_CONTINUE when more
	/// data is required. The InputIterator return value indicates how much of the
	/// input has been consumed.
	int parse(http_header& resp, char* begin, char* end);

private:
	/// Handle the next character of input.
	int consume(http_header& resp, char input);

	/// Check if a byte is an HTTP character.
	static bool is_char(int c);

	/// Check if a byte is an HTTP control character.
	static bool is_ctl(int c);

	/// Check if a byte is defined as an HTTP tspecial character.
	static bool is_tspecial(int c);

	/// Check if a byte is a digit.
	static bool is_digit(int c);

	/// The current state of the parser.
	enum state
	{
		http_version_h,
		http_version_t_1,
		http_version_t_2,
		http_version_p,
		http_version_slash,
		http_version_major_start,
		http_version_major,
		http_version_minor_start,
		http_version_minor,
		http_status_code,
		http_code_decs,
		expecting_newline_1,
		header_line_start,
		header_lws,
		header_name,
		space_before_header_value,
		header_value,
		expecting_newline_2,
		expecting_newline_3
	} state_;
};

#endif // HTTP_PARSER_HEADER_PARSER_HPP

