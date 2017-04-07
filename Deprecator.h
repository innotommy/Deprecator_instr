#include <iostream>
#include <fstream>
#include <string>
#include "base/json/json_file_value_serializer.h"
#include "base/path_service.h"
#include "base/values.h"
#include "base/json/json_string_value_serializer.h"
#include "base/time/time.h"


#define config_doc "Deprecatorconfig.json"

//first version of controller
class DeprecatorClass {

public:
    static DeprecatorClass* getInstance() {}

    void LogFunction(std::string name) {}

private:
    // private constructor and destructor

    DeprecatorClass() {}

    ~DeprecatorClass() {}

    // private copy constructor and assignment operator
    DeprecatorClass(const DeprecatorClass&);
    DeprecatorClass& operator=(const DeprecatorClass&);
    //variable private
    static DeprecatorClass *m_instanceDeprecator;
    std::unique_ptr<base::DictionaryValue> dizionario;
};

DeprecatorClass* DeprecatorClass::m_instanceDeprecator = nullptr;
