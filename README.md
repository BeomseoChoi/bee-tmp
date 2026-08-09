# Bee TMP
A compile-time type-list manipulation header-only C Plus Plus 20 library.

## Requirements
- C Plus Plus 20

## Usage
```cpp
#include <bee/type_list.h>
```

## Examples
### Add and remove types
```cpp
using namespace bee::tmp;

// <int, float>
using list = type_list<int, float>;

// <int, float, double>
using with_double = push_back<list, double>::type;

// <char, int, float, double>
using with_char = push_front<with_double, char>::type;

// <int, float, double>
using result = pop_front<with_char>::type;

static_assert(
    same_type_list<result, type_list<int, float, double>>::value
);
```

### Access a type by index
```cpp
// <int, float, double>
using list = type_list<int, float, double>;

// float
using second = at<list, 1>::type;

static_assert(
    std::is_same_v<second, float>
);
```
### Transform a type list
```cpp
template <typename T>
struct add_pointer
{
    using type = T*;
};

// <int, float, double>
using input = bee::tmp::type_list<int, float, double>;

// <int*, float*, double*>
using output = bee::tmp::transform<input, add_pointer>::type;


```
### Filter a type list
```cpp
template <typename T>
struct is_integral : std::is_integral<T>
{
};

// <int, float, long, double>
using input = bee::tmp::type_list<int, float, long, double>;

// <int, long>
using output = bee::tmp::filter<input, is_integral>::type;

static_assert(
    bee::tmp::same_type_list<
        output,
        bee::tmp::type_list<int, long>
    >::value
);
```

### Convert to std::tuple
```cpp
// type_list<int, float, double>
using list = bee::tmp::type_list<int, float, double>;

// std::tuple<int, float, double>
using tuple = bee::tmp::to_tuple<list>::type;

static_assert(
    std::is_same_v<
        tuple,
        std::tuple<int, float, double>
    >
);
```

## Motivation
This library was developed for a type-safe compile-time C Plus Plus web-server router. If interseted, please see [this]()