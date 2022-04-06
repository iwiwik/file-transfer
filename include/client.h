
#include<array>
#include<fstream>
#include<boost/asio.hpp>
#include<boost/asio/io_service.hpp>
#include<string>
#include<iostream>
//#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#include<boost/filesystem/path.hpp>
//#undef BOOST_NO_CXX11_SCOPED_ENUMS

using namespace boost::asio;

class client{

    public:
        client(io_service& t_ioservice,ip::tcp::resolver::iterator t_endpointiterator,std::string const& t_path);

    private:
        void open_file(std::string const& t_path);
        void doconnect();
        void dowrite_file(const boost::system::error_code& t_ec);
        template<typename Buffer>
        void writeBuffer(Buffer& );

        ip::tcp::resolver m_ioservice;
        ip::tcp::socket m_sockte;
        ip::tcp::resolver::iterator m_endpointIterator;
        enum {MessageSize = 1024};
        std::array<char,MessageSize>m_buf;
        boost::asio::streambuf m_request;
        std::ifstream m_sourceFile;
        std::string m_path;

};