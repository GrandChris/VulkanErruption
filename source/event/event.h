//
// \file       event.h
// \author     GrandChris
// \date       2020-10-14
// \brief      Function pointer to a member function
//

#pragma once

#include <vector>
#include <cstddef>

template<typename T>
class Event;

///
/// \class      Event.h
/// \author     GrandChris
/// \date       2020-10-14
/// \brief      Stores any member function
/// \param      R(Args)   function signature
/// \details     https://www.codeproject.com/Articles/11015/The-Impossibly-Fast-C-Events
///
template <typename... Args>
class Event<void(Args...)>
{
public:

///////////////////////////////////////////////////////////////////////////////
// Public Methods

    template<typename ...TArgs>
    void operator()(TArgs&&... args) const;

    template <auto Method, typename T>
    void add(T & object);

    template <typename T>
    void add(T & object);

    template <auto Function>
    void add();

    size_t size() const;

private:

///////////////////////////////////////////////////////////////////////////////
// Type Definitions

    using FunctionPtr = void (*)(void * objectPtr, Args&&... args);  // Function signature

///////////////////////////////////////////////////////////////////////////////
// Member Variables

    struct Element {
        void * object;             // Pointer to the object
        FunctionPtr function;      // Pointer to the function of the object
    };

    std::vector<Element> mElements;


///////////////////////////////////////////////////////////////////////////////
// Static Functions

    template <auto Method, typename T>
    static void callMethod(void* objectPtr, Args&&... args);

    template <class T>
    static void callFunctor(void* objectPtr, Args&&... args);

    template <auto Method>
    static void callFunction(void* objectPtr, Args&&... args);
};


///////////////////////////////////////////////////////////////////////////////
// Implementation

#include "type_traits_function_pointer.h"
#include <utility>

///
/// \brief      Calls the stored function
/// \author     GrandChris
/// \date       2020-10-14
/// \param      args   function arguments
/// \return     R      function return type
///
template <typename... Args>
template<typename ...TArgs>
inline void Event<void(Args...)>::operator()(TArgs&&... args) const
{
    for(auto & elem : mElements) {
        auto objectPtr = elem.object;
        auto functionPtr = elem.function;
        
        (*functionPtr)(objectPtr, std::forward<Args>(args)...);
    }
}

///
/// \brief      Checks if the Event is valid
/// \author     GrandChris
/// \date       2020-10-14
/// \return     True if it contains a function, false otherwise
///
template <typename... Args>
inline size_t Event<void(Args...)>::size() const
{
    return mElements.size();
}


///
/// \brief      adds a member function
/// \author     GrandChris
/// \date       2020-10-14
/// \param      T          Class type
/// \param      Method     Member function pointer
/// \param      objectPtr  Pointer to an object of this class
///
template <typename... Args>
template <auto Method, typename T>
inline void Event<void(Args...)>::add(T & object) 
{
    static_assert(std::is_member_function_pointer_v<decltype(Method)>);
    static_assert(Method != nullptr);

    mElements.push_back({
        object: &object,
        function: &callMethod<Method, T>
    });
}


///
/// \brief      adds a function object
/// \author     GrandChris
/// \date       2020-10-14
/// \param      T          Invocable class
/// \param      objectPtr  Pointer to an object of this class
///
template <typename... Args>
template <typename T>
inline void Event<void(Args...)>::add(T & object) 
{
    mElements.push_back({
        object: &object,
        function: &callFunctor<T>
    });
}

///
/// \brief      adds regular function
/// \author     GrandChris
/// \date       2020-11-21
/// \param      Function   Function pointer
///
template <typename... Args>
template <auto Function>
inline void Event<void(Args...)>::add() 
{
    static_assert(is_function_ptr_v<decltype(Function)>);
    static_assert(Function != nullptr);

    mElements.push_back({
        object: nullptr,
        function: &callFunction<Function>
    });
}

///
/// \brief      Call a member function like a function pointer
/// \author     GrandChris
/// \date       2020-10-14
/// \param      T          Class type
/// \param      Method     Class Method
/// \param      objectPtr  Pointer to an object of this class
/// \param      args       Arguments of the member function
///
template <typename... Args>
template <auto Method, typename T>
inline void Event<void(Args...)>::callMethod(void* objectPtr, Args&&... args)
{
    T* p = static_cast<T*>(objectPtr);
    return (p->*Method)(std::forward<Args>(args)...);
}

///
/// \brief      Call a member function like a function pointer
/// \author     GrandChris
/// \date       2020-10-14
/// \param      T          Class type
/// \param      TMethod    Class Method
/// \param      objectPtr  Pointer to an object of this class
/// \param      args       Arguments of the member function
///
template <typename... Args>
template <typename T>
inline  void Event<void(Args...)>::callFunctor(void* objectPtr, Args&&... args) 
{
    T* p = static_cast<T*>(objectPtr);
    return (*p)(std::forward<Args>(args)...);
}


///
/// \brief      Call a member function like a function pointer
/// \author     GrandChris
/// \date       2020-10-14
/// \param      T          Class type
/// \param      Function     Class Method
/// \param      objectPtr  Pointer to an object of this class
/// \param      args       Arguments of the member function
///
template <typename... Args>
template <auto Function>
inline void Event<void(Args...)>::callFunction(void* objectPtr, Args&&... args) 
{
    return (*Function)(std::forward<Args>(args)...);
}