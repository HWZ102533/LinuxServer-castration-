//
// Created by admin on 3/29/22.
//

#include "CFileAio.h"
#include <fcntl.h>
#include <cstring>
#include <sys/eventfd.h>
#include <unistd.h>
#include "CSocket.h"

CFileAio::CFileAio() {

}

CFileAio::~CFileAio() {

}

void CFileAio::aio_callback(io_context_t ctx, struct iocb *iocb, long res, long res2){

}

bool CFileAio::Init() {
    return true;
}

void CFileAio::readFile(void* pData) {
    {
        std::unique_lock<std::mutex> lock(m_queue_mutex);
        m_command.emplace(pData);
    }
    m_condition.notify_one();
}

void CFileAio::writeFile(void* pData) {
    {
        std::unique_lock<std::mutex> lock(m_queue_mutex);
        m_command.emplace(pData);
    }
    m_condition.notify_one();
}
