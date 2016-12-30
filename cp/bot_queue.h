#ifndef _BOT_QUEUE_H_
#define _BOT_QUEUE_H_

#include <deque>
using namespace std;

class BotQueue
{
public:
	typedef deque<string>			TQueue;
	typedef TQueue::iterator		iterator;
	typedef TQueue::const_iterator	const_iterator;

	inline iterator begin() { return mQueue.begin(); }
	inline iterator end()	{ return mQueue.end(); }

	inline const_iterator begin() const { return mQueue.begin(); }
	inline const_iterator end()   const { return mQueue.end(); }

private:
	TQueue	mQueue;


public:
	inline void push_front(const string& s)		{ mQueue.push_front(s); }
	inline void push_back(const string& s)		{ mQueue.push_back(s); }
	inline void pop_front()				{ if (size()) mQueue.pop_front(); }
	inline void pop_back()				{ if (size()) mQueue.pop_back(); }
	inline bool empty() const					{ return mQueue.empty(); }
	inline string& front()					{ return mQueue.front(); }
	inline int size() const				{ return mQueue.size(); }
	inline void clear()					{ mQueue.clear(); }

	inline string& operator[](int i)		{ return mQueue[i]; };

	inline void repushFront(const string& sCmd)
	{
		if (size()) pop_front();
		push_front(sCmd);
	}

};

#endif
