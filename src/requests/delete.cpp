#include <request.hpp>

void	Request::deleteReq()
{
	getRessourcePath();
	if (access(_path.c_str(), W_OK)) {
		if (remove(_path.c_str()) < 0)
			throw Request::ErrcodeException(INTERNAL_SERVER_ERROR, *this);
		_errcode = NO_CONTENT;
		_responseBody = "";
	} else
		throw Request::ErrcodeException(FORBIDDEN, *this);
}
