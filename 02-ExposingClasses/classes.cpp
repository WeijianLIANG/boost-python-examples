
#include <boost/python.hpp>
#include <boost/python/list.hpp>
#include <boost/python/extract.hpp>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
class World
{
public:
    void set(std::string msg) { mMsg = msg; }
    void many(boost::python::list msgs) {
        long l = len(msgs);
        std::stringstream ss;
        for (long i = 0; i<l; ++i) {
            if (i>0) ss << ", ";
            std::string s = boost::python::extract<std::string>(msgs[i]);
            ss << s;
        }
        mMsg = ss.str();
    }
    std::string greet() { return mMsg; }
    void set_strain(boost::python::list strain_list)
    {
        int a = len(strain_list);
        for (int i =0; i < a;i++)
        {
            strain_list_.push_back(boost::python::extract<double>(strain_list[i]));
        }
    }
    void show_strain()
    {
        int a = strain_list_.size();
        for (int i=0;i<a;i++) std::cout <<strain_list_.at(i)<<"\n";
        
    }
private:
    std::string mMsg;
    std::vector<double> strain_list_;
};

// using namespace boost::python;

BOOST_PYTHON_MODULE(classes)
{
    boost::python::class_<World>("World")
        .def("greet", &World::greet)
        .def("set", &World::set)
        .def("many", &World::many)
        .def("set_strain",&World::set_strain)
        .def("show_strain",&World::show_strain)
    ;
};
