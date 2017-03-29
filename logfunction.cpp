#include "chrome/browser/Deprecator.cpp"


std::string name= "name of function";
DeprecatorClass *singleton=nullptr;
singleton = singleton->getInstance();
singleton->LogFunction(name);