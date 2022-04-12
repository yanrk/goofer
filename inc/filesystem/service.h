/********************************************************
 * Description : system service class
 * Date        : 2019-01-11 18:11:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_SERVICE_H
#define GOOFER_SERVICE_H


#include "common/common.h"

NAMESPACE_GOOFER_BEGIN

struct ServiceRunAccount
{
    enum v_t
    {
        local_system,
        local_service,
        network_service
    };
};

class GOOFER_API SystemServiceBase
{
public:
    SystemServiceBase(ServiceRunAccount::v_t service_run_account);
    virtual ~SystemServiceBase();

public:
    bool run(const char * service_name, int argc, char * argv []);

public:
    virtual bool on_start(int argc, char * argv []) = 0;
    virtual bool on_stop() = 0;
    virtual bool running() = 0;
};

NAMESPACE_GOOFER_END


#endif // GOOFER_SERVICE_H
