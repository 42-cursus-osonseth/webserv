#include <request.hpp>

const t_location	*Request::findMatchingLocation()
{
	const std::list<t_location>	&locs = _matchingServer->getLocations();

	for (std::list<t_location>::const_iterator it = locs.begin(); it != locs.end(); it++) {
		if (_path.find((*it).uri, 0) == 0) {
			std::cerr << (*it).uri << " seems to match" << std::endl;
			return &(*it);
		}
	}
	std::cerr << "No match" << std::endl;
	return (0);
}

void	Request::getRessourcePath()
{
	size_t	mark;
	const t_location	*loc = findMatchingLocation();
	std::string	fullPath;

	if (!loc || loc->root.empty())
		throw Request::ErrcodeException(NOT_FOUND, *this);
	fullPath = loc->root + "/" + _path;
	std::cerr << fullPath << std::endl;
	mark = fullPath.find("?");
	if (mark != std::string::npos)
		fullPath = fullPath.substr(0, mark);

	if (std::find(loc->methods.begin(), loc->methods.end(), _method) == loc->methods.end())
		throw Request::ErrcodeException(METHOD_NOT_ALLOWED, *this);
	if (access(fullPath.c_str(), F_OK))
		throw Request::ErrcodeException(NOT_FOUND, *this);
	else if (Utils::pathIsDir(fullPath)) {
		if (!loc->index.empty())
			fullPath += loc->index;
		else if (!loc->dirListing)
			throw Request::ErrcodeException(FORBIDDEN, *this);
		else {
			_path = Utils::cleanupPath(fullPath);
			throw Request::AutoIndexHandle(*this);
		}
	}
	_path = Utils::cleanupPath(fullPath);
}
