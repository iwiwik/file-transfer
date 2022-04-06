
#include"include/client.h"

using namespace boost::asio;



template<typename Buffer>
void client::writeBuffer(Buffer& t_buffer){
    boost::asio::async_write(m_sockte,t_buffer,[this](boost::system::error_code ec,size_t){
        dowrite_file(ec);
    });
}

client::client(io_service& t_ioservice,ip::tcp::resolver::iterator t_endpointiterator,std::string const& t_path):
        m_ioservice(t_ioservice),m_sockte(t_ioservice),
        m_endpointIterator(t_endpointiterator),m_path(t_path){
            doconnect();
            open_file(m_path);
        }

void client::open_file(std::string const& t_path){
    m_sourceFile.open(t_path,std::ios_base::binary | std::ios_base::ate);
    if(m_sourceFile.fail()){
        throw std::fstream::failure("failed while opening file" + t_path);
    }

    m_sourceFile.seekg(0,m_sourceFile.end);
    auto filesize = m_sourceFile.tellg();
    m_sourceFile.seekg(0,m_sourceFile.beg);

    std::ostream requestStream(&m_request);
    boost::filesystem::path p(t_path);
    requestStream << p.filename().string() << "\n" << filesize << "\n\n";

}

void client::doconnect(){
    async_connect(m_sockte,m_endpointIterator,[this](boost::system::error_code ec,ip::tcp::resolver::iterator){
        if(!ec){
            writeBuffer(m_request);
        }else{
            std::cout << "Coudn't connect to host .Please run server""or check network connection.\n";
        }
    });
}

void client::dowrite_file(const boost::system::error_code& t_ec){
    if(!t_ec){
        if(m_sourceFile){
            m_sourceFile.read(m_buf.data(),m_buf.size());
            if(m_sourceFile.fail() && !m_sourceFile.eof()){
                auto msg = "Failed while reading file";
                throw std::fstream::failure(msg);
            }
            std::stringstream ss;
            ss << "Send" << m_sourceFile.gcount() << "bytes,total: " << m_sourceFile.tellg() << " bytes";

            std::cout << ss.str() << std::endl;

            auto buf = boost::asio::buffer(m_buf.data(),static_cast<size_t>(m_sourceFile.gcount()));
            writeBuffer(buf);
        }
        }else{
            std::cout << "Error: "<<t_ec.message();
    }
}

int main(int argc,char* argv[]){
    if(argc != 4){
        std::cerr << "Usage:client<address> <port> <filePath>\n";
        return 1;
    }

    auto address = argv[1];
    auto port = argv[2];
    auto filepath = argv[3];

    try{
        boost::asio::io_service ioService;

        boost::asio::ip::tcp::resolver res(ioService);

        auto endpoint = res.resolve(address,port);
        client cclient(ioService,endpoint,filepath);
        ioService.run();

    }catch(std::fstream::failure& e){
        std::cerr << e.what() << "\n";

    }catch(std::exception& e){
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
