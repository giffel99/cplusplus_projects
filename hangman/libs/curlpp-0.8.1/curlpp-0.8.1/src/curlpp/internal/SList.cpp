#include "curlpp/internal/SList.hpp"

#include <ostream>
#include <string>

namespace curlpp
{

	namespace internal
	{

		SList::SList(const SList &rhs)
			: mList(0), mData(rhs.mData)
		{
			update();
		}

		SList::SList(curl_slist *list)
			: mList(NULL)
		{
			constructFrom(list);
		}

		SList::SList(const list<string> &rhs)
			: mList(0), mData(rhs)
		{
			update();
		}

		SList::SList()
			: mList(0)
		{
		}

		SList::~SList()
		{
			clear();
		}

		void
		SList::clear()
		{
			if (mList != 0)
			{
				curl_slist_free_all(mList);
				mList = 0;
			}
		}

		void
		SList::constructFrom(curl_slist *list)
		{
			mData.clear();

			curl_slist *c = list;
			while (c)
			{
				mData.push_back(c->data);
				c = c->next;
			}

			update();
		}

		void
		SList::set(const list<string> &list)
		{
			mData = list;
			update();
		}

		void
		SList::update()
		{
			clear();

			for (list<string>::const_iterator pos = mData.begin();
				 pos != mData.end();
				 pos++)
			{
				mList = curl_slist_append(mList, (*pos).c_str());
			}
		}

		SList::operator list<string>()
		{
			return list();
		}

		SList &
		SList::operator=(const list<string> &list)
		{
			set(list);
			return (*this);
		}

		curl_slist *
		SList::cslist() const
		{
			return mList;
		}

		list<string>
		SList::list()
		{
			return mData;
		}

	} // namespace internal

} // namespace curlpp

ostream &operator<<(ostream &stream, const list<string> &value)
{
	for (list<string>::const_iterator pos = value.begin();
		 pos != value.end();
		 pos++)
	{
		if (pos != value.begin())
		{
			stream << ", ";
		}
		stream << (*pos);
	}

	return stream;
}
