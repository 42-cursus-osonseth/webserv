#include <request.hpp>

const t_location	*Request::findMatchingLocation()
{
	const std::list<t_location>	&locs = _matchingServer->getLocations();

	for (std::list<t_location>::const_iterator it = locs.begin(); it != locs.end(); it++) {
		if (_path.find((*it).uri, 0) == 0) {
			return &(*it);
		}
	}
	return (0);
}

std::string	Request::replaceRoot(const std::string &location, const std::string &root)
{
	std::string	res = _path;

	res.replace(0, location.size(), root + "/");
	return res;
}
bool Request::isProcessPath()
{
	std::string str = _fullPath.substr(_root.size(), _processDir.size());
	return (str == _processDir);
}

void	Request::getRessourcePath()
{
	const t_location	*loc = findMatchingLocation();
	if (!loc)
		throw Request::ErrcodeException(NOT_FOUND, *this);
	_root = loc->root;
	if (!loc || loc->root.empty())
		throw Request::ErrcodeException(NOT_FOUND, *this);
	_fullPath = replaceRoot(loc->uri, loc->root);
	if (std::find(loc->methods.begin(), loc->methods.end(), _method) == loc->methods.end())
		throw Request::ErrcodeException(METHOD_NOT_ALLOWED, *this);
	if (loc->redir.first != 0)
		throw Request::Redirection(*this, loc->redir);
	if (Utils::pathIsDir(_fullPath) && !isProcessPath()) {
		std::cout << BLUE << "LE CHEMIN EST UN REPERTOIRE" << RESET << std::endl;
		if (!loc->index.empty())
			_fullPath += loc->index;
		else if (!loc->dirListing)
			throw Request::ErrcodeException(FORBIDDEN, *this);
		else {
			_path = Utils::cleanupPath(_fullPath);
			throw Request::AutoIndexHandle(*this);
		}
	} else if (access(_fullPath.c_str(), F_OK))
			throw Request::ErrcodeException(NOT_FOUND, *this);
	if(((_data["Content-Type"].find("multipart/form-data") != std::string::npos || _data["transfer-encoding"] == "chunked")) && !loc->upload)
		throw Request::ErrcodeException(FORBIDDEN, *this);
	_path = Utils::cleanupPath(_fullPath);
	_clientRef.setPath(_path);
	_clientRef.setMethod(_method);
	_clientRef.setUploadPath(loc->uploadPath);



}
