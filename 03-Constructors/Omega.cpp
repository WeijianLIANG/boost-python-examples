#include <Eigen/Dense>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>

#include "boost/python.hpp"
#include "boost/python/stl_iterator.hpp"
#include "boost/shared_ptr.hpp"

std::mutex mutex;

template <typename T>
inline std::vector<T> py_list_to_std_vector(
    const boost::python::object& iterable) {
  return std::vector<T>(boost::python::stl_input_iterator<T>(iterable),
                        boost::python::stl_input_iterator<T>());
}

template <class T>
inline boost::python::list std_vector_to_py_list(std::vector<T> vector) {
  typename std::vector<T>::iterator iter;
  boost::python::list list;
  for (iter = vector.begin(); iter != vector.end(); ++iter) {
    list.append(*iter);
  }
  return list;
}

class Scene {
 public:
  Scene() {}

  void set_strain(std::vector<double>& strain) {
    strain_.resize(strain.size());
    // std::lock_guard<std::mutex> lock(mutex);
    for (int i = 0; i < strain.size(); i++) {
      auto a = strain.at(i);
      strain_(i) = a;
    }
  }

  Eigen::VectorXd& strain() {
    // std::lock_guard<std::mutex> lock(mutex);
    return strain_;
  }

  Eigen::VectorXd stress() {
    // std::lock_guard<std::mutex> lock(mutex);
    return stress_;
  }

  void compute_stress() {
    // std::lock_guard<std::mutex> lock(mutex);
    stress_ = 100 * strain_;
  }

 private:
  Eigen::VectorXd stress_;
  Eigen::VectorXd strain_;
};

class Omega {
 public:
  std::shared_ptr<Scene> get_scene() { return scene_; }

  int creat_scene() {
    scene_ = std::make_shared<Scene>();
    my_scenes_.push_back(scene_);
    return my_scenes_.size() - 1;
  }

  void checkout_scene(int i) {
    std::lock_guard<std::mutex> lock(mutex);
    scene_ = my_scenes_.at(i);
  }
  void load_scene(std::shared_ptr<Scene> scene) { scene_ = scene; }

  boost::python::list get_stress() {
    scene_->compute_stress();
    auto stress = scene_->stress();
    std::vector<double> a;
    for (int i = 0; i < stress.size(); i++) {
      a.push_back(stress(i));
    }
    return std_vector_to_py_list(a);
  }

  void set_dstrain(boost::python::list dstrain) {
    std::lock_guard<std::mutex> lock(mutex);
    std::vector<double> a;

    a = py_list_to_std_vector<double>(dstrain);
    scene_->set_strain(a);
    // std::lock_guard<std::mutex> unlock(mutex);
  }

 private:
  std::vector<std::shared_ptr<Scene>> my_scenes_;
  std::shared_ptr<Scene> scene_;
  int currentSceneNb;
};

BOOST_PYTHON_MODULE(Omega) {
  //   boost::python::class_<Scene>("Scene")
  //       //.def(init<double, double>())
  //       .def("set_strain", &Scene::set_strain)
  //       .def("strain", &Scene::strain)
  //       .def("compute_stress", &Scene::compute_stress);

  boost::python::class_<Omega>("Omega")
      //.def(init<double, double>())
      .def("creat_scene", &Omega::creat_scene)
      .def("get_scene", &Omega::get_scene)
      .def("checkout_scene", &Omega::checkout_scene)
      .def("get_stress", &Omega::get_stress)
      .def("set_dstrain", &Omega::set_dstrain)
      .def("load_scene", &Omega::load_scene);
}