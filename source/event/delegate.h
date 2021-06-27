//
// \file       Delegate.h
// \author     GrandChris
// \date       2020-10-14
// \brief      Function pointer to a member function
//

#pragma once

template<typename T>
class Delegate;

///
/// \class      Delegate.h
/// \author     GrandChris
/// \date       2020-10-14
/// \brief      Stores any member function
/// \param      R(Args)   function signature
/// \details     https://www.codeproject.com/Articles/11015/The-Impossibly-Fast-C-Delegates
///
template <typename R, typename... Args>
class Delegate<R(Args...)>
{
public:

///////////////////////////////////////////////////////////////////////////////
// Public Methods

    template<typename ...TArgs>
    R operator()(TArgs&&... args) const;

    template <auto Method, typename T>
    void set(T & object);

    template <typename T>
    void set(T & object);

    template <auto Function>
    void set();

    operator bool() const;

private:

///////////////////////////////////////////////////////////////////////////////
// Type Definitions

    using FunctionPtr = R (*)(void * objectPtr, Args&&... args);  // Function signature

///////////////////////////////////////////////////////////////////////////////
// Member Variables

    void * mObject = nullptr;         // Pointer to the object
    FunctionPtr mFunction = nullptr;    // Pointer to the function of the object

///////////////////////////////////////////////////////////////////////////////
// Static Functions

    template <auto Method, typename T>
    static R callMethod(void* objectPtr, Args&&... args);

    template <class T>
    static R callFunctor(void* objectPtr, Args&&... args);

    template <auto Method>
    static R callFunction(void* objectPtr, Args&&... args);
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
template <typename R, typename... Args>
template<typename ...TArgs>
inline R Delegate<R(Args...)>::operator()(TArgs&&... args) const
{
    return (*mFunction)(mObject, std::forward<Args>(args)...);
}

///
/// \brief      Checks if the Delegate is valid
/// \author     GrandChris
/// \date       2020-10-14
/// \return     True if it contains a function, false otherwise
///
template <typename R, typename... Args>
inline Delegate<R(Args...)>::operator bool() const
{
    return mFunction != nullptr || mObject != nullptr;
}

///
/// \brief      Sets a member function
/// \author     GrandChris
/// \date       2020-10-14
/// \param      T          Class type
/// \param      Method     Member function pointer
/// \param      objectPtr  Pointer to an object of this class
///
template <typename R, typename... Args>
template <auto Method, typename T>
inline void Delegate<R(Args...)>::set(T & object) 
{
    static_assert(std::is_member_function_pointer_v<decltype(Method)>);
    static_assert(Method != nullptr);

    mObject = &object;
    mFunction = &callMethod<Method, T>;
}


///
/// \brief      Sets a function object
/// \author     GrandChris
/// \date       2020-10-14
/// \param      T          Invocable class
/// \param      objectPtr  Pointer to an object of this class
///
template <typename R, typename... Args>
template <typename T>
inline void Delegate<R(Args...)>::set(T & object) 
{
    mObject = &object;
    mFunction = &callFunctor<T>;
}

///
/// \brief      Sets regular function
/// \author     GrandChris
/// \date       2020-11-21
/// \param      Function   Function pointer
///
template <typename R, typename... Args>
template <auto Function>
inline void Delegate<R(Args...)>::set() 
{
    static_assert(is_function_ptr_v<decltype(Function)>);
    static_assert(Function != nullptr);

    mObject = nullptr;
    mFunction = callFunction<Function>;
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
template <typename R, typename... Args>
template <auto Method, typename T>
inline R Delegate<R(Args...)>::callMethod(void* objectPtr, Args&&... args)
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
template <typename R, typename... Args>
template <typename T>
inline  R Delegate<R(Args...)>::callFunctor(void* objectPtr, Args&&... args) 
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
template <typename R, typename... Args>
template <auto Function>
inline R Delegate<R(Args...)>::callFunction(void* objectPtr, Args&&... args) 
{
    return (*Function)(std::forward<Args>(args)...);
}