# chaining

Provides a way to chaining function with return value control. Similar using std::expected in C++23, but use C++17.

Example:

You have some function
```cpp
std::optional<int> f1()
{
    std::cout << "call f1" << std::endl;
    return 3;
}

std::optional<std::string> f2(int val)
{
    std::cout << "call f2 with arg " << val << std::endl;
    return "some string";
}

std::optional<std::string> f3(double t, std::string v)
{
    std::cout << "call f3 with args " << t << " and " << v << std::endl;
    return "string";
}
```

and use 
```cpp
using namespace metafunc;

tryThis(f1)
  .than(f2)
  .than(f3, 0.1, "some other");
```

output:
```
call f1
call f2 with arg 3
call f3 with args 0.1 and some other
```
