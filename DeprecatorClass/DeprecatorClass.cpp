#include "base/DeprecatorClass/DeprecatorClass.h"


bool Log;
std::unique_ptr<base::DictionaryValue> Dictionary;

DeprecatorClass* DeprecatorClass::GetInstance(){
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
    log << func;
    log << "##";
    log << block;
    log << "\n";
    log.close();

    if(block == "Block")
    {
      return true;
    }
    else if(block == "Allow")
    {
      return false;
    }
    else if(block == "Ask")
    {
      return false;
    }
  }
  else
  {
    if(block == "Block")
    {
      return true;
    }
    else if(block == "Allow")
    {
      return false;
    }
    else if(block == "Ask")
    {
      return false;
    }
  }
  return true;
}

DeprecatorClass::DeprecatorClass(){
  LoadConfig();
};
