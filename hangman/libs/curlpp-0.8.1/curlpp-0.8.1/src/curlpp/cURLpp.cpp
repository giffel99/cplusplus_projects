#include "curlpp/cURLpp.hpp"
#include "curlpp/Exception.hpp"

#include <cstdlib>

namespace
{

   struct libcURLInstance
   {
      libcURLInstance();

      ~libcURLInstance()
      {
         curlpp::terminate();
      }
   };
}

void curlpp::initialize(long flags)
{
   CURLcode code;
   code = curl_global_init(flags);
   libcurlRuntimeAssert("cannot initialize curlpp", code);
}

void curlpp::terminate()
{
   curl_global_cleanup();
}

curlpp::Cleanup::Cleanup()
{
   curlpp::initialize();
}

curlpp::Cleanup::~Cleanup()
{
}

string
curlpp::escape(const string &url)
{
   string buffer;
   char *p = curl_escape(url.c_str(), (int)url.size());
   if (!p)
   {
      throw runtime_error("unable to escape the string"); // we got an error
   }
   else
   {
      buffer = p;
      curl_free(p);
   }
   return buffer;
}

string
curlpp::unescape(const string &url)
{
   string buffer;
   char *p = curl_unescape(url.c_str(), (int)url.size());
   if (!p)
   {
      throw RuntimeError("unable to escape the string"); // we got an error
   }
   else
   {
      buffer = p;
      curl_free(p);
   }
   return buffer;
}

string
curlpp::getenv(const string &name)
{
   string buffer;
   char *p = curl_getenv(name.c_str());
   if (!p)
   {
      throw RuntimeError("unable to get the environnement string"); // we got an error
   }
   else
   {
      buffer = p;
      free(p);
   }
   return buffer;
}

string
curlpp::libcurlVersion()
{
   char *p = curl_version();
   if (!p)
   {
      throw RuntimeError("unable to get the libcurl version"); // we got an error
   }

   return string(p);
}

time_t
curlpp::getdate(const string &date, time_t *now)
{
   time_t return_value = curl_getdate(date.c_str(), now);
   if (!return_value)
   {
      throw RuntimeError("unable to get the date");
   }

   return return_value;
}
