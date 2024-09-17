#include "ConnectionMgr.h"

ConnectionMgr::ConnectionMgr() {

}

ConnectionMgr& ConnectionMgr::GetInstance()
{
    static ConnectionMgr instance_;
    return instance_;
}

void ConnectionMgr::AddConnection(std::shared_ptr<Connection> conptr)
{
    map_cons_[conptr->GetUid()] = conptr;
}

void ConnectionMgr::RmvConnection(std::string uuid)
{
    map_cons_.erase(uuid);
}
