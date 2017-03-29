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
    static DeprecatorClass* getInstance() 
    {
        if(m_instanceDeprecator == nullptr)
        {
            return m_instanceDeprecator;
        }
        else
        {
            return m_instanceDeprecator;
        }
    }

    void LogFunction(std::string name)
    {   
    	std::ofstream log;
    	log.open("log.txt",std::fstream::app);
        log << name;
        log << " ";
        log << "time:";
        log << base::Time::Now();
        log << "\n";
        log.close();
    }

private:
    // private constructor and destructor

    DeprecatorClass() 
    {
        if(m_instanceDeprecator == nullptr)
        {
        m_instanceDeprecator= this;
        }
        '''
        base::FilePath original_file_path;
        PathService::Get(base::DIR_CURRENT, &original_file_path);
        original_file_path = original_file_path.AppendASCII(config_doc);
    	JSONFileValueDeserializer deserializer2(original_file_path);
    	int errorcode=0;
    	std::string errormessage;
  		dep_config = base::DictionaryValue::From(deserializer2.Deserialize(&errorcode, &errormessage));
		if (dep_config == NULL)//error reading the json file
			{
   			 	return;//error reading configuration file
			}
        '''
    }

    ~DeprecatorClass() {}

    // private copy constructor and assignment operator
    DeprecatorClass(const DeprecatorClass&);
    DeprecatorClass& operator=(const DeprecatorClass&);
    //variable private
    static DeprecatorClass *m_instanceDeprecator;
    std::unique_ptr<base::DictionaryValue> dizionario;
};

DeprecatorClass* DeprecatorClass::m_instanceDeprecator = nullptr;
