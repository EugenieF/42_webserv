#ifndef PURCHASE_HPP
# define PURCHASE_HPP

# include "utils.hpp"
# include <vector>

/******************************************************************************/
/*                                CLASS COOKIE                                */
/******************************************************************************/

class Purchase
{
	private:
	/**********************     MEMBER VARIABLES     ********************/
		std::string		_name;
		std::string		_hamster;
		std::string		_color;

	public:
	/*********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*-------     Main    -------*/
		Purchase();
		Purchase(const std::string& name, const std::string& hamster, const std::string& color);
		Purchase(const Purchase& other);
		Purchase&		operator=(const Purchase& other);
		~Purchase();

						/*-------    Setter   -------*/
		void			setPurchase(const std::string& name, const std::string& content);
		void			setPurchase(const std::string& name, const std::string& hamster, const std::string& color);

						/*-------    Getter   -------*/
		std::string		getName() const;
		std::string		getHamster() const;
		std::string		getColor() const;

						/*-------     Utils   -------*/
		void			display() const;
		bool			isEmpty() const;
};

#endif
