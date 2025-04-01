#include <Server.hpp>

int	get_depth(const std::string &l1)
{
	int	depth = 0;

	for (int i = 0; l1[i]; i++) {
		if (l1[i] == '/')
			depth += 1;
	}
	return (depth);
}

bool	compare_location(const t_location &l1, const t_location &l2)
{
	return get_depth(l1) > get_depth(l2);
}

void	Server::sortLocations()
{
	locations.sort(location_compare);
}