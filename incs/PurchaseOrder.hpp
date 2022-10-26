#ifndef PURCHASE_ORDER_HPP
# define PURCHASE_ORDER_HPP

# include "utils.hpp"
# include <vector>

# define SESSION_TIMEOUT 1000

/******************************************************************************/
/*                                CLASS COOKIE                                */
/******************************************************************************/

class Purchase
{
	private:
		std::string		_name;
		std::string		_hamster;
		std::string		_color;

	public:
		Purchase();
		Purchase(const std::string& name, const std::string& hamster, const std::string& color);
		~Purchase();

		void	setPurchase(const std::string& name, const std::string& content);
		void	setPurchase(const std::string& name, const std::string& hamster, const std::string& color);
		void	display() const;
		bool	isEmpty() const;
};

class Order
{
	public:
	/***********************      MEMBER TYPES      *********************/
		typedef std::vector<class Purchase>	listOfPurchase;

	private:
	/**********************     MEMBER VARIABLES     ********************/
		std::string			_sessionId;
		listOfPurchase		_purchase;
		size_t				_time;

	public:
	/*********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*-------     Main    -------*/
		Order();
		Order(const std::string& sessionId);
		Order(const Order& other);
		Order&				operator=(const Order& other);
		~Order();
		void				fillOrder(const Order& other);

						/*-------    Setter   -------*/
		void				addPurchase(const std::string& name, const std::string& hamster, const std::string& color);

						/*-------    Getter   -------*/
		std::string			getSessionId() const;

						/*-------     Size   -------*/
		size_t				size() const;
		bool				isEmpty();
		void				checkSizeCookie(const std::string& name, const std::string& value);

						/*-------     Time   -------*/
		bool				sessionIsAlive();
		void				updateTime();

						/*-------     Utils   -------*/
		void				display() const;
};

#endif
