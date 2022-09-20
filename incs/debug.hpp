/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 10:51:59 by etran             #+#    #+#             */
/*   Updated: 2022/09/19 15:24:26 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Debug hpp */

#ifndef DEBUG_HPP
# define DEBUG_HPP

# include <string>
# include <iostream>
# include <string.h>

# define NL '\n'
# define DEBUG(x) std::cout << " ### Debug ### : " << x << NL

#endif
