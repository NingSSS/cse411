

#include <stdio.h>
#include <cpprest/uri.h>
#include <cpprest/http_listener.h>
#include <cpprest/asyncrt_utils.h>
#pragma comment(lib, "cpprest_2_7.lib")
#pragma comment(lib, "bcrypt.lib")
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "httpapi.lib")

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/types.hpp>
#include <bsoncxx/json.hpp>

#include <cstdint>
#include <iostream>
#include <vector>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

class CommandHandler
{
public:
    CommandHandler() {}
    CommandHandler(utility::string_t url);
    pplx::task<void> open() { return m_listener.open(); }
    pplx::task<void> close() { return m_listener.close(); }
private:
    void handle_get_or_post(http_request message);
    http_listener m_listener;
};

CommandHandler::CommandHandler(utility::string_t url) : m_listener(url)
{
    m_listener.support(methods::GET, std::bind(&CommandHandler::handle_get_or_post, this, std::placeholders::_1));
    m_listener.support(methods::POST, std::bind(&CommandHandler::handle_get_or_post, this, std::placeholders::_1));
}

void CommandHandler::handle_get_or_post(http_request message)
{
    ucout << "Method: " << message.method() << std::endl;
    ucout << "URI: " << http::uri::decode(message.relative_uri().path()) << std::endl;
    ucout << "Query: " << http::uri::decode(message.relative_uri().query()) << std::endl << std::endl;
    message.reply(status_codes::OK, "ACCEPTED");
};
//int argc, char argv[]

void print_all(mongocxx::v_noabi::collection &c) {
    std::cout<<"printing all documents"<<std::endl;
    auto cursor =c.find({});
    for(auto&&doc : cursor)
        std::cout<<bsoncxx::to_json(doc) << std::endl;
}

core::v1::optional<mongocxx::v_noabi::result::insert_one>
insert_simple_document(mongocxx::v_noabi::collection &collection) {
    std::cout<<"Creating a simple document"<<std::endl;
    bsoncxx::builder::stream::document document{};// make a document {key:"value"}, and put it in the collection
    document <<"key" <<"value";
    return collection.insert_one(document.view());
}


int main()
{
    try
    {
        mongocxx::instance inst{};
        std::string username = "NingSong" ;
        std::string pass = "Sn123456";
        mongocxx::client conn{
                mongocxx::uri{"mongodb://"+username+":"+pass+"@ds123399.mlab.com:23399/cse411"}
        };

        auto collection = conn["cse411"]["NingSong"];

        auto res =insert_simple_document(collection);
        if(res) {
            //print_all(collection);
            //delete_by_id(collection, res->inserted_id());
            //print_all(collection);
        }

        std::string query_name = "eashanrrr";
        auto cursor = collection.find(bsoncxx::builder::stream::document{}
                                              << "key" << query_name << bsoncxx::builder::stream::finalize);

        for (auto doc : cursor){
            std::cout << bsoncxx::to_json(doc) << std::endl;
        }

        utility::string_t address = U("http://localhost/index.html");
        uri_builder uri(address);
        auto addr = uri.to_uri().to_string();
        CommandHandler handler(addr);
        handler.open().wait();
        ucout << utility::string_t(U("Listening for requests at: ")) << addr << std::endl;
        ucout << U("Press ENTER key to quit...") << std::endl;
        std::string line;
        std::getline(std::cin, line);
        handler.close().wait();
    }
    catch (std::exception& ex)
    {
        ucout << U("Exception: ") << ex.what() << std::endl;
        ucout << U("Press ENTER key to quit...") << std::endl;
        std::string line;
        std::getline(std::cin, line);
    }




    return 0;
}