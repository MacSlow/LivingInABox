#pragma once


template<class T>
struct Box
{
    T min;
    T max;
};

template<class T>
inline Box<T> make_box(T min, T max) 
{ 
    Box<T> b; 
    b.min = min; 
    b.max = max; 
    return b;
}

template<class T>
inline Box<T> make_box(T t) 
{ 
    Box<T> b; 
    b.min = t; 
    b.max = t; 
    return b;
}

