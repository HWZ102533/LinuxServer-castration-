//
// Created by hwz on 1/16/22.
//

#ifndef HWZ_SERVERDFC_CSERVICEREGISTER_H
#define HWZ_SERVERDFC_CSERVICEREGISTER_H

#include <functional>
#include <map>

class CServiceRegister {
public:
    CServiceRegister(void);
    ~CServiceRegister(void);

    CServiceRegister(std::string className, std::function<void*()> func);
    static std::map<std::string,std::function<void*()> > m_FuncMap;
};

#define REGISTERNEW(className)                                             \
	className* Clone##className(){                              \
	return new className;                                 \
}                                          \

#define REGISTER(className)                                             \
	className* Clone##className(){                                            \
    static className s_className;\
	return &s_className;                                 \
}

#define IMPLEMENT_CLASS(Tagname,className)\
	CServiceRegister g_Register##Tagname(                        \
#Tagname,Clone##className);



#endif //HWZ_SERVERDFC_CSERVICEREGISTER_H
