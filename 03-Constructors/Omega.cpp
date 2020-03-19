#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <mutex>

std::mutex mutex;

class Scene
{
public:
    void set_strain(double strain)
    {
        // std::lock_guard<std::mutex> lock(mutex);
        strain_ = strain;
    }

    double strain()
    {   
        // std::lock_guard<std::mutex> lock(mutex);
        return strain_;
    }
    double compute_stress()
    {   
        // std::lock_guard<std::mutex> lock(mutex);
        stress_ = 100 * strain_;
        return stress_;
    }

private:
    double stress_{0};
    double strain_{0};
};


class Omega
{
public:
    std::shared_ptr<Scene> get_scene()
    {
        return scene_;
    }
    int creat_scene()
    {
        scene_ = std::make_shared<Scene>();
        my_scenes_.push_back(scene_);
        return my_scenes_.size() - 1;
    }
    void checkout_scene(int i)
    {   
        std::lock_guard<std::mutex> lock(mutex);
        scene_ = my_scenes_.at(i);
    }
    void load_scene(std::shared_ptr<Scene> scene)
    {
        scene_ = scene;
    }

    double get_stress()
    {   
        std::lock_guard<std::mutex> lock(mutex);
        return scene_->compute_stress();
    }
    double set_dstrain(double dstrain)

    {   
        std::lock_guard<std::mutex> lock(mutex);
        auto a =scene_->strain() + dstrain; 
        scene_->set_strain(a);
        // std::lock_guard<std::mutex> unlock(mutex);
    }

private:
    std::vector<std::shared_ptr<Scene>> my_scenes_;
    std::shared_ptr<Scene> scene_;
    int currentSceneNb;

};

#include <boost/python.hpp>
//using namespace boost::python;

BOOST_PYTHON_MODULE(O)
{
    boost::python::class_<Scene>("Scene")
    //.def(init<double, double>())
    .def("set_strain", &Scene::set_strain)
    .def("strain", &Scene::strain)
    .def("compute_stress", &Scene::compute_stress)
    ;
    boost::python::class_<Omega>("Omega")
    //.def(init<double, double>())
    .def("creat_scene", &Omega::creat_scene)
    .def("get_scene", &Omega::get_scene)
    .def("checkout_scene", &Omega::checkout_scene)
    .def("get_stress", &Omega::get_stress)
    .def("set_dstrain", &Omega::set_dstrain)
    .def("load_scene", &Omega::load_scene)
    ;

}


