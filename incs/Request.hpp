#ifndef REQUEST_HPP
# define REQUEST_HPP

class   Request
{
    private:

    public:
        Request();
        Request(const Request& other);
        ~Request();
        Request&    operator=(const Request& other);

};

#endif