//
// Created by hwz on 1/16/22.
//

#include "CServiceRegister.h"
std::map<std::string,std::function<void*()> > CServiceRegister::m_FuncMap;

CServiceRegister::CServiceRegister(){

}

CServiceRegister::~CServiceRegister(){

}

CServiceRegister::CServiceRegister(std::string className, std::function<void*()> func)
{
    m_FuncMap.insert(std::pair<std::string, std::function<void*()>>(className, func));
}

