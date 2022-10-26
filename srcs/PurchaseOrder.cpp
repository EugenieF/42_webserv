# include "PurchaseOrder.hpp"

Purchase::Purchase() {}

Purchase::Purchase(const std::string& name, const std::string& hamster, const std::string& color)
{
    _name = name;
    _hamster = hamster;
    _color = color;
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

void    Order::addPurchase(const std::string& name, const std::string& hamster, const std::string& color)
{
    _purchase.insert(_purchase.end(), Purchase(name, hamster, color));
}
