# include "Purchase.hpp"

Purchase::Purchase() {}

Purchase::Purchase(const std::string& name, const std::string& hamster, const std::string& color)
{
	setPurchase(name, hamster, color);
}

Purchase::Purchase(const Purchase& other)
{
	*this = other;
}

Purchase&	Purchase::operator=(const Purchase& other)
{
	if (this != &other)
	{
		_name = other.getName();
		_hamster = other.getHamster();
		_color = other.getColor();
	}
	return (*this);
}


Purchase::~Purchase() {}

void	Purchase::setPurchase(const std::string& name, const std::string& content)
{
    if (name == "name")
        _name = content;
    else if (name == "hamster")
        _hamster = content;
    else if (name == "color")
        _color = content;
}

void	Purchase::setPurchase(const std::string& name, const std::string& hamster, const std::string& color)
{
    _name = name;
    _hamster = hamster;
    _color = color;
}

void	Purchase::display() const
{
    std::cout << "name = " << _name << std::endl;    
    std::cout << "hamster = " << _hamster << std::endl;    
    std::cout << "color = " << _color << std::endl;    
}

bool	Purchase::isEmpty() const
{
    return (_name.empty() || _hamster.empty() || _color.empty());
}

std::string		Purchase::getName() const
{
    return (_name);
}

std::string		Purchase::getHamster() const
{
    return (_hamster);
}

std::string		Purchase::getColor() const
{
    return (_color);
}
