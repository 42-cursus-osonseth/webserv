/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekherbo <mekherbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 16:44:14 by mekherbo          #+#    #+#             */
/*   Updated: 2025/01/06 01:22:27 by mekherbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <cstdlib>
#include <string>
#include <map>
#include "colors.hpp"
#include "utils.hpp"
class Configuration
{
	private:
		static int nbServer;
		static std::ifstream infile;
		int nbPort;
		static bool	insideServer;
	public:
		Configuration();
		~Configuration();
		static void parseFile(const std::string &filename);
		static bool	handleToken(const std::string &token);
		static bool	handleServer();
		static void	parseBlock();
};



#endif