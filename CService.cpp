//
// Created by hwz on 10/9/21.
//

#include "CService.h"
CPoolThread* CService::m_pThreadPool= nullptr;
//get data from database
CCache* CService::m_pCache = nullptr;

CSql* CService::m_pSQL = nullptr;