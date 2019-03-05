//
// Created by Ning Song on 2018/12/12.


#include <cpprest/http_listener.h>
#include <cpprest/filestream.h>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

class MiniHTTPServer
{
public:
    explicit MiniHTTPServer(utility::string_t strUrl);//根据地址创建http_listener
    ~MiniHTTPServer(); //关闭服务器
    //设定文档根目录
    void setDocumentRoot(const utility::string_t& strWWW) { m_strWWW = strWWW;};

public:
    pplx::task<void> start();  //启动服务器
    pplx::task<void> stop(); //关闭服务器
    //处理错误
    static void handle_error(pplx::task<void>& t);
private:
    utility::string_t m_strWWW;//根路径
    std::unique_ptr<web::http::experimental::listener::http_listener> m_listener;
};

MiniHTTPServer::MiniHTTPServer(utility::string_t strUrl)
        :m_strWWW(U(".")),m_listener(new http_listener(strUrl))
{
}

pplx::task<void> MiniHTTPServer::start()
{
    return m_listener->open().then([](auto t){ handle_error(t); });
}

pplx::task<void> MiniHTTPServer::stop()
{
    return m_listener->close().then([](auto t){ handle_error(t); });
}
