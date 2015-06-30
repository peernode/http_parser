#include "header.h"
#include "header_parser.h"

header_parser::header_parser():state_(http_version_h)
{
}

int header_parser::parse(http_header& resp, char* begin, char* end)
{
	while (begin != end)
	{
		int result = consume_input(resp, *(begin++));
		if (result != HEADER_CONTINUE)
		{
			return result;
		}
	}

	return HEADER_ERROR;
}

int header_parser::consume_input(http_header& resp, char input)
{
	switch (state_)
	{
	case http_version_h:
		if (input == 'H')
		{
			state_ = http_version_t_1;
			return HEADER_CONTINUE;
		}
		else
		{
			return HEADER_ERROR;
		}

	case http_version_t_1:
		if (input == 'T')
		{
			state_ = http_version_t_2;
			return HEADER_CONTINUE;
		}
		else
		{
			return HEADER_ERROR;
		}

	case http_version_t_2:
		if (input == 'T')
		{
			state_ = http_version_p;
			return HEADER_CONTINUE;
		}
		else
		{
			return HEADER_ERROR;
		}

	case http_version_p:
		if (input == 'P')
		{
			state_ = http_version_slash;
			return HEADER_CONTINUE;
		}
		else
		{
			return HEADER_ERROR;
		}

	case http_version_slash:
		if (input == '/')
		{
			resp.http_version_major_ = 0;
			resp.http_version_minor_ = 0;
			state_ = http_version_major_start;
			return HEADER_CONTINUE;
		}
		else
		{
			return HEADER_ERROR;
		}
	case http_version_major_start:
		if (is_digit(input))
		{
			resp.http_version_major_ = resp.http_version_major_ * 10 + input - '0';
			state_ = http_version_major;
			return HEADER_CONTINUE;
		}
		else
		{
			return HEADER_ERROR;
		}

	case http_version_major:
		if (input == '.')
		{
			state_ = http_version_minor_start;
			return HEADER_CONTINUE;
		}
		else if (is_digit(input))
		{
			resp.http_version_major_ = resp.http_version_major_ * 10 + input - '0';
			return HEADER_CONTINUE;
		}
		else
		{
			return HEADER_ERROR;
		}

	case http_version_minor_start:
		if (is_digit(input))
		{
			resp.http_version_minor_ = resp.http_version_minor_ * 10 + input - '0';
			state_ = http_version_minor;
			return HEADER_CONTINUE;
		}

		else
		{
			return HEADER_ERROR;
		}

	case http_version_minor:
		if (input == ' ')
		{
			resp.status_code_ = 0;
			state_ = http_status_code;
			return HEADER_CONTINUE;
		}
		else if (is_digit(input))
		{
			resp.http_version_minor_ = resp.http_version_minor_ * 10 + input - '0';
			return HEADER_CONTINUE;
		}
		else
		{
			return HEADER_ERROR;
		}

	case http_status_code:
		if (input== ' ')
		{
			state_ = http_code_decs;
			return HEADER_CONTINUE;
		}
		else if(input=='\r')
		{
			state_ = expecting_newline_1;
			return HEADER_CONTINUE;
		}
		else if (is_digit(input))
		{
			resp.status_code_ = resp.status_code_ * 10 + input - '0';
			return HEADER_CONTINUE;
		}
		else
		{
			return HEADER_ERROR;
		}

	case http_code_decs:
		if(input=='\r')
		{
			state_ = expecting_newline_1;
			return HEADER_CONTINUE;
		}
		else
		{
			resp.phrase_.push_back(input);
			return HEADER_CONTINUE;
		}

	case expecting_newline_1:
		if (input == '\n')
		{
			state_ = header_line_start;
			return HEADER_CONTINUE;
		}
		else
		{
			return HEADER_ERROR;
		}

	case header_line_start:
		if (input == '\r')
		{
			state_ = expecting_newline_3;
			return HEADER_CONTINUE;
		}
		else if (!resp.headers_.empty() && (input == ' ' || input == '\t'))
		{
			state_ = header_lws;
			return HEADER_CONTINUE;
		}
		else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
		{
			return HEADER_ERROR;
		}
		else
		{
			resp.headers_.push_back(header());
			resp.headers_.back().name_.push_back(input);
			state_ = header_name;
			return HEADER_CONTINUE;
		}

	case header_lws:
		if (input == '\r')
		{
			state_ = expecting_newline_2;
			return HEADER_CONTINUE;
		}
		else if (input == ' ' || input == '\t')
		{
			return HEADER_CONTINUE;
		}
		else if (is_ctl(input))
		{

			return HEADER_ERROR;
		}
		else
		{
			state_ = header_value;
			resp.headers_.back().value_.push_back(input);
			return HEADER_CONTINUE;
		}

	case header_name:
		if (input == ':')
		{
			state_ = space_before_header_value;
			return HEADER_CONTINUE;
		}
		else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
		{
			return HEADER_ERROR;
		}
		else
		{
			resp.headers_.back().name_.push_back(input);
			return HEADER_CONTINUE;
		}

	case space_before_header_value:
		if (input == ' ')
		{
			state_ = header_value;
			return HEADER_CONTINUE;
		}
		else
		{
			return HEADER_ERROR;
		}

	case header_value:
		if (input == '\r')
		{
			state_ = expecting_newline_2;
			return HEADER_CONTINUE;
		}
		else if (is_ctl(input))
		{
			return HEADER_ERROR;
		}
		else
		{
			resp.headers_.back().value_.push_back(input);
			return HEADER_CONTINUE;
		}

	case expecting_newline_2:
		if (input == '\n')
		{
			state_ = header_line_start;
			return HEADER_CONTINUE;
		}
		else
		{
			return HEADER_ERROR;
		}

	case expecting_newline_3:
		return (input == '\n') ? HEADER_OK : HEADER_ERROR;

	default:
		return HEADER_ERROR;

	}
}

bool header_parser::is_char(int c)
{
	return c >= 0 && c <= 127;
}

bool header_parser::is_ctl(int c)
{
	return (c >= 0 && c <= 31) || (c == 127);
}

bool header_parser::is_tspecial(int c)
{
	switch (c)
	{
	case '(': case ')': case '<': case '>': case '@':
	case ',': case ';': case ':': case '\\': case '"':
	case '/': case '[': case ']': case '?': case '=':
	case '{': case '}': case ' ': case '\t':
		return true;
	default:
		return false;
	}
}

bool header_parser::is_digit(int c)
{
	return c >= '0' && c <= '9';
}
