#include <request.hpp>

void	Request::notImplementedPlaceholder()
{
	_errcode = NOT_IMPLEMENTED;
	_responseBody = "<!DOCTYPE html><html>501</html>\r\n";
	generateHeader();
}
