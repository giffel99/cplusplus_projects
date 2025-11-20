/*
 *    Copyright (c) <2002-2009> <Jean-Philippe Barrette-LaPierre>
 *
 *    Permission is hereby granted, free of charge, to any person obtaining
 *    a copy of this software and associated documentation files
 *    (curlpp), to deal in the Software without restriction,
 *    including without limitation the rights to use, copy, modify, merge,
 *    publish, distribute, sublicense, and/or sell copies of the Software,
 *    and to permit persons to whom the Software is furnished to do so,
 *    subject to the following conditions:
 *
 *    The above copyright notice and this permission notice shall be included
 *    in all copies or substantial portions of the Software.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 *    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 *    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 *    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef CURLPP_SLIST_HPP
#define CURLPP_SLIST_HPP

#include <curl/curl.h>

#include <list>
#include <string>

namespace curlpp
{

	namespace internal
	{

		/**
		 * This class is binding the curl_slist struct.
		 */

		class SList
		{

		public:
			SList();
			SList(const SList &rhs);

			/**
			 * The list passed in as an argument is now possessed by the class.
			 */
			SList(curl_slist *list);

			explicit SList(const list<string> &list);
			~SList();

			SList &operator=(const list<string> &list);
			operator list<string>();

			curl_slist *cslist() const;
			list<string> list();

		private:
			void set(const list<string> &list);
			void update();
			void clear();
			void constructFrom(curl_slist *list);

			curl_slist *mList;
			list<string> mData;
		};

	} // namespace internal

} // namespace curlpp

namespace cURLpp = curlpp;

ostream &operator<<(ostream &stream, const list<string> &value);

#endif // #ifndef CURLPP_SLIST_HPP
