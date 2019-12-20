///////////////////////////////////////////////////////////////////////////////
// File:		 Event.h
// Revision:	 1.0
// Date:		 11.11.2019
// Author:		 Christian Steinbrecher
// Description:  Callback functions can be registered and called
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <functional>
#include <vector>

template<typename TFunc>
class Event
{
public:
	void add(std::function<TFunc> const func);

	template<typename TArg>
	void operator()(TArg const& arg) const;


private:
	std::vector<std::function<TFunc>> mFunctions;
};



// #######+++++++ Implementation +++++++#######

template<typename TFunc>
inline void Event<TFunc>::add(std::function<TFunc> const func)
{
	if (func != nullptr)
	{
		mFunctions.push_back(std::move(func));
	}
}


template<typename TFunc>
template<typename TArg>
inline void Event<TFunc>::operator()(TArg const& arg) const
{
	for (auto const& func : mFunctions)
	{
		func(arg);
	}
}
