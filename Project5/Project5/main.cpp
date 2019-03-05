#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>
#include <twitcurl.h>
#include <json/json.h>

#include "twitterClient.h"
#include <cstdlib>
#include <stdexcept>
#include <json/Reader.h>
#include <json/Value.h>


#include <vector>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/types.hpp>


using std::string;  //使用string对象
using std::vector;  //使用vector


using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
using namespace web::json;                                  // JSON library

void Split(const string& src, const string& separator, vector<string>& dest)
{
    string str = src;
    string substring;
    string::size_type start = 0, index;
    dest.clear();
    index = str.find_first_of(separator,start);
    do
    {
        if (index != string::npos)
        {
            substring = str.substr(start,index-start );
            dest.push_back(substring);
            start =index+separator.size();
            index = str.find(separator,start);
            if (start == string::npos) break;
        }
    }while(index != string::npos);

    //the last part
    substring = str.substr(start);
    dest.push_back(substring);
}


int main(int argc, char* argv[])
{
    auto fileStream = std::make_shared<ostream>();

    // Open stream to output file.
    pplx::task<void> requestTask = fstream::open_ostream(U("results.html")).then([=](ostream outFile)
        {
         *fileStream = outFile;

         // Create http_client to send the request.
         http_client client(U("http://www.bing.com/"));

         twitCurl twitterObj;
         std::string tmpStr, tmpStr2;
         std::string replyMsg;
         char tmpBuf[1024];

         twitterObj.setTwitterUsername( "nis318@lehigh.edu" );
         twitterObj.setTwitterPassword( "Sn123456" );

         twitterObj.getOAuth().setConsumerKey( std::string( "c2ySbY6lodrFR1ePbzuJK0LLz" ) );
         twitterObj.getOAuth().setConsumerSecret( std::string( "csKh6a13PBENlgy1UuJbLnJz8zWrDIEEsJEEW7mB1NcnZZ1DGN" ) );

         std::string myOAuthAccessTokenKey("");
         std::string myOAuthAccessTokenSecret("");
         std::ifstream oAuthTokenKeyIn;
         std::ifstream oAuthTokenSecretIn;
         std::string value;

         oAuthTokenKeyIn.open( "twitterClient_token_key.txt" );
         oAuthTokenSecretIn.open( "twitterClient_token_secret.txt" );

         memset( tmpBuf, 0, 1024 );
         oAuthTokenKeyIn >> tmpBuf;
         myOAuthAccessTokenKey = tmpBuf;

         memset( tmpBuf, 0, 1024 );
         oAuthTokenSecretIn >> tmpBuf;
         myOAuthAccessTokenSecret = tmpBuf;

         oAuthTokenKeyIn.close();
         oAuthTokenSecretIn.close();

         if( myOAuthAccessTokenKey.size() && myOAuthAccessTokenSecret.size() )
         {
             /* If we already have these keys, then no need to go through auth again */
             printf( "\nUsing:\nKey: %s\nSecret: %s\n\n", myOAuthAccessTokenKey.c_str(), myOAuthAccessTokenSecret.c_str() );

             twitterObj.getOAuth().setOAuthTokenKey( myOAuthAccessTokenKey );
             twitterObj.getOAuth().setOAuthTokenSecret( myOAuthAccessTokenSecret );
         }
         else {
             /* Step 2: Get request token key and secret */
             std::string authUrl;
             twitterObj.oAuthRequestToken( authUrl );

             /* Step 3: Get PIN  */
             memset( tmpBuf, 0, 1024 );
             printf( "\nDo you want to visit twitter.com for PIN (0 for no; 1 for yes): " );
             fgets( tmpBuf, sizeof( tmpBuf ), stdin );
             tmpStr = tmpBuf;
             if( std::string::npos != tmpStr.find( "1" ) )
             {
                 /* Ask user to visit twitter.com auth page and get PIN */
                 memset( tmpBuf, 0, 1024 );
                 printf( "\nPlease visit this link in web browser and authorize this application:\n%s", authUrl.c_str() );
                 printf( "\nEnter the PIN provided by twitter: " );
                 fgets( tmpBuf, sizeof( tmpBuf ), stdin );
                 tmpStr = tmpBuf;
                 twitterObj.getOAuth().setOAuthPin( tmpStr );
             }
             else {
                 /* Else, pass auth url to twitCurl and get it via twitCurl PIN handling */
                 twitterObj.oAuthHandlePIN( authUrl );
             }

             /* Step 4: Exchange request token with access token */
             twitterObj.oAuthAccessToken();

             /* Step 5: Now, save this access token key and secret for future use without PIN */
             twitterObj.getOAuth().getOAuthTokenKey( myOAuthAccessTokenKey );
             twitterObj.getOAuth().getOAuthTokenSecret( myOAuthAccessTokenSecret );

             /* Step 6: Save these keys in a file or wherever */
             std::ofstream oAuthTokenKeyOut;
             std::ofstream oAuthTokenSecretOut;

             oAuthTokenKeyOut.open( "twitterClient_token_key.txt" );
             oAuthTokenSecretOut.open( "twitterClient_token_secret.txt" );

             oAuthTokenKeyOut.clear();
             oAuthTokenSecretOut.clear();

             oAuthTokenKeyOut << myOAuthAccessTokenKey.c_str();
             oAuthTokenSecretOut << myOAuthAccessTokenSecret.c_str();

             oAuthTokenKeyOut.close();
             oAuthTokenSecretOut.close();
         }
            /* OAuth flow ends */
            /* Account credentials verification */

         if( twitterObj.accountVerifyCredGet() )
         {
             twitterObj.getLastWebResponse( replyMsg );
             printf( "\ntwitterClient:: twitCurl::accountVerifyCredGet web response:\n%s\n", replyMsg.c_str() );
         }
         else {
             twitterObj.getLastCurlError( replyMsg );
             printf( "\ntwitterClient:: twitCurl::accountVerifyCredGet error:\n%s\n", replyMsg.c_str() );
         }

         /* Search a string */
         printf( "\nEnter string to search: " );
         memset( tmpBuf, 0, 1024 );
         fgets( tmpBuf, sizeof( tmpBuf ), stdin );
         tmpStr = tmpBuf;

         std::map <std::string, int> m1;
         std::map <std::string, int>::iterator m1_Iter;
         std::string final;


         int a = tmpStr.size();
         tmpStr = tmpStr.substr(0,a-1);
         printf( "\nLimit search results to: " );
         memset( tmpBuf, 0, 1024 );
         fgets( tmpBuf, sizeof( tmpBuf ), stdin );
         tmpStr2 = tmpBuf;
         int b = tmpStr2.size();
         tmpStr2 = tmpStr2.substr(0,b-1);
         replyMsg = "";
         if( twitterObj.search( tmpStr, tmpStr2 ) )
         {
             twitterObj.getLastWebResponse( replyMsg );
             Json::Reader reader;
             Json::Value temp;
             reader.parse(replyMsg.c_str(),temp);
             std::cout << temp.toStyledString()<<std::endl;
             int file_size = temp["statuses"].size();
             for(int i =0; i<file_size; i++){
                 //std::cout<< i<<std::endl;
                 Json::Value status = temp["statuses"][i]["created_at"];
                 //std::cout<< status.toStyledString() <<std::endl;
                 vector<string> Data;
                 Split(status.toStyledString().c_str()," ",Data);

//               for (int i=0;i<Data.size();i++)
//               {
//                   printf("%s ",Data[i].c_str());
//               }
                 tm tm_month;
                 tm tm_year;
                 //std::cout<< Data[1].c_str()<<std::endl;
                 strptime(Data[1].c_str(), "%b", &tm_month);
                 strptime(Data[5].c_str(), "%Y\"", &tm_year);
                 //value = value + std::to_string(tm_month.tm_mon+1) + "-" + Data[2].c_str() + "-" + std::to_string(tm_year.tm_year+1900) + ";";
                 value = std::to_string(tm_month.tm_mon+1) + "-" + Data[2].c_str() + "-" + std::to_string(tm_year.tm_year+1900) + ":";
                 if(m1.count(value)==0)
                 {
                     m1[value]=1;
                 } else
                 {
                     m1[value]++;
                 }

             }
             for(m1_Iter=m1.begin();m1_Iter!=m1.end();m1_Iter++)
             {
                 final=final+m1_Iter->first+std::to_string(m1_Iter->second)+" ";
             }
             value = final;
             std::cout<<value<<std::endl;
             //std::cout<< value<<std::endl;
             //printf( "\ntwitterClient:: twitCurl::search web response:\n%s\n", replyMsg.c_str() );


             mongocxx::instance inst{};
             std::string username = "NingSong" ;
             std::string pass = "Sn123456";
             mongocxx::client conn{
                 mongocxx::uri{"mongodb://"+username+":"+pass+"@ds123399.mlab.com:23399/cse411"}
             };

             auto collection = conn["cse411"]["NingSong"];
             bsoncxx::builder::stream::document document{};
             document << "key"<<tmpStr << "value"<< value;
             collection.insert_one(document.view());

         }
         else {
             twitterObj.getLastCurlError( replyMsg );
             printf( "\ntwitterClient:: twitCurl::search error:\n%s\n", replyMsg.c_str() );
         }




         // Build request URI and start the request.
         uri_builder builder(U("/search"));
         builder.append_query(U("q"), U("cpprestsdk github"));
         return client.request(methods::GET, builder.to_string());
         })

                    // Handle response headers arriving.
        .then([=](http_response response)
        {
          //printf("Received response status code:%u\n", response.status_code());
          std::cout << "Received response status code: " << response.status_code() <<std::endl;
          // Write response body into the file.
          return response.body().read_to_end(fileStream->streambuf());
        })

                // Close the file stream.
        .then([=](size_t)
        {
          return fileStream->close();
        });

    // Wait for all the outstanding I/O to complete and handle any exceptions
        try
        {
            requestTask.wait();
        }
        catch (const std::exception &e)
        {
            //printf("Error exception:%s\n", e.what());
            std::cerr<<"Exception: "<<e.what() <<std::endl;
        }



    return 0;
}