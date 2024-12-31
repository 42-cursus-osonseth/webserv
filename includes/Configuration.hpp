/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekherbo <mekherbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 16:44:14 by mekherbo          #+#    #+#             */
/*   Updated: 2024/12/31 19:44:50 by mekherbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstdlib>
#include <string>
class Configuration
{
	private:
		static int nbServer;
		int nbPort;
	public:
		Configuration();
		~Configuration();
		static void parseFile(const std::string &filename);
};



#endif