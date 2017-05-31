#include "base/DeprecatorClass/DeprecatorClass.h"


bool Log;
std::unique_ptr<base::DictionaryValue> Dictionary;

DeprecatorClass* DeprecatorClass::GetInstance(){
  std::ofstream log;
  log.open("log.txt",std::ofstream::app);
  log << base::Singleton<DeprecatorClass>::get();
  log.close();
  return base::Singleton<DeprecatorClass>::get();
}

void DeprecatorClass::LoadConfig()
{
  base::FilePath original_file_path;
  PathService::Get(base::DIR_CURRENT, &original_file_path);
  original_file_path = original_file_path.AppendASCII(config_doc);
  JSONFileValueDeserializer deserializer2(original_file_path);
  int errorcode=0;
  std::string errormessage;
  Dictionary = base::DictionaryValue::From(deserializer2.Deserialize(&errorcode, &errormessage));
  Dictionary->GetBoolean("Log", &Log);
}

bool DeprecatorClass::Check(std::string func)
{ 
  std::string block="";
  Dictionary->GetString("func", &block);
  if(Log)
  {
    std::ofstream log;
    log.open("log.txt",std::ofstream::app);
    log << "time:";
    log << base::Time::Now();
    log << "##";
    log << block;
    log << "##";
    log << " instrance of DeprecatorClass: ";
    log << this;
    log << "\n";
    log.close();

    if(block == "Block")
    {
      return false;
    }
    else if(block == "Allow")
    {
      return true;
    }
    else if(block == "Ask")
    {
      return true;
    }
  }
  else
  {
    if(block == "Block")
    {
      return false;
    }
    else if(block == "Allow")
    {
      return true;
    }
    else if(block == "Ask")
    {
      return true;
    }
  }
  return false;
}

DeprecatorClass::DeprecatorClass(){};
