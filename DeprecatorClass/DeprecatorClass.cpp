#include "base/DeprecatorClass/DeprecatorClass.h"
#include <iostream>
#include <string.h>



bool Log;
bool verbose;
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
  std::ofstream loading;
  if(Dictionary->GetBoolean("verbose", &verbose) & verbose){
    loading.open("loading.txt",std::ofstream::app);
    if (Dictionary == NULL)//error reading the json file
        { 
          loading << "Failed to parse configuration\n";
          loading << errormessage;
          loading << "\n";
          loading.close();
        }
    size_t dim;
    loading << "json file read with success\n";
    dim = deserializer2.get_last_read_size();
    loading << "Dimenzione ultimo Json letto(byte): ";
    loading << dim;
    loading << "\n";
    loading.close();
  }
  Dictionary->GetBoolean("Log", &Log);
}

bool DeprecatorClass::Check(std::string func)
{ 
  std::string block="";
  int i;
  bool first=true;
  char look;
  for(i=0;i<(int)func.length();i++){
    look=func.at(i);
    if((look == '#') & !first){
      break;
    }
    if(look == '#'){
      first=false;
      i++;//skip the second # and catch the next #
    }
  }
  std::string search=func.substr(0,i);
  if(Dictionary->GetStringWithoutPathExpansion(search, &block))
  {
    if(verbose){
    std::ofstream loading;
    loading.open("loading.txt",std::ofstream::app);
    loading << "ask: " + search;
    loading << "\n";
    loading << "returned: " + block;
    loading << "\n";
    loading.close();
    }
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
  else{
    if(verbose){
    std::ofstream loading;
    loading.open("loading.txt",std::ofstream::app);
    loading << "asked key:" + search;
    loading << "\n";
    loading << "not found the key in the Dictionary!!";
    loading << "\n";
    loading.close();
    }
  }
  return true;
}

DeprecatorClass::DeprecatorClass(){
  LoadConfig();
};
