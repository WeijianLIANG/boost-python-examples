char const* greet()
{
   return "hello, world";
}

#include <boost/python.hpp>

BOOST_PYTHON_MODULE(libhello)
{
    using namespace boost::python;
    def("greet", greet);
}
