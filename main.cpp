#include <iostream>
#include <sstream>
#include <string>

#include "header.h"
#include "header_parser.h"

using namespace std;

int main()
{
	cout<<"http parse test..."<<endl;

	std::ostringstream  http_resp;
	http_resp<<"HTTP/1.1 302 Move Temp\r\n";
	http_resp<<"Server: bfe/1.0.8.5"<<"\r\n";
	http_resp<<" sdf sf\r\n";
	http_resp<<"Date: Tue, 30 Jun 2015 06:36:36 GMT"<<"\r\n";
	http_resp<<"Content-Length: 1638400"<<"\r\n";
	http_resp<<"Content-Range: bytes 123456-789123/123456789"<<"\r\n";
	http_resp<<"X-Cache: HIT from jingan-60.sina.com.cn"<<"\r\n\r\n";

	header_parser hparser;
	http_header resp;
	std::string str = http_resp.str();
	int n = hparser.parse(resp, (char*)str.data(), (char*)str.data()+str.length());

	resp.reset();

	return 0;
}