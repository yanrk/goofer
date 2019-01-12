/********************************************************
 * Description : system service class
 * Data        : 2019-01-11 18:11:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#include <string>

#ifdef _MSC_VER
    #include <cstring>
    #include <windows.h>
    #include <process.h>
#else
    #include <errno.h>
    #include <unistd.h>
    #include <syslog.h>
#endif // _MSC_VER

#include "filesystem/service.h"

static Goofer::SystemServiceBase  * s_service = nullptr;
static std::string                  s_service_name;

#ifdef _MSC_VER

static bool install_system_service(const char * service_path, const char * service_name, const char * display_name, const char * dependencies = "", const char * start_name = "NT AUTHORITY\\LocalService", const char * password = nullptr)
{
    if (nullptr == service_path || nullptr == service_name || nullptr == display_name)
    {
        return (false);
    }

    bool ret = false;
    SC_HANDLE service_control_manager = nullptr;
    SC_HANDLE service_control_service = nullptr;

    do
    {
        service_control_manager = OpenSCManagerA(nullptr, nullptr, SC_MANAGER_CONNECT | SC_MANAGER_CREATE_SERVICE);
        if (nullptr == service_control_manager)
        {
            break;
        }

        service_control_service = CreateServiceA(service_control_manager, service_name, display_name, SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, SERVICE_AUTO_START, SERVICE_ERROR_NORMAL, service_path, nullptr, nullptr, dependencies, start_name, password);
        if (nullptr == service_control_service)
        {
            ret = (ERROR_SERVICE_EXISTS == GetLastError());
            break;
        }

        ret = true;
        break;
    } while (false);

    if (nullptr != service_control_service)
    {
        CloseServiceHandle(service_control_service);
    }
    if (nullptr != service_control_manager)
    {
        CloseServiceHandle(service_control_manager);
    }

    return (ret);
}

static bool uninstall_system_service(const char * service_name)
{
    if (nullptr == service_name)
    {
        return (false);
    }

    bool ret = false;
    SC_HANDLE service_control_manager = nullptr;
    SC_HANDLE service_control_service = nullptr;

    do
    {
        service_control_manager = OpenSCManagerA(nullptr, nullptr, SC_MANAGER_CONNECT);
        if (nullptr == service_control_manager)
        {
            break;
        }

        service_control_service = OpenServiceA(service_control_manager, service_name, SERVICE_STOP | SERVICE_QUERY_STATUS | DELETE);
        if (nullptr == service_control_service)
        {
            ret = (ERROR_SERVICE_DOES_NOT_EXIST == GetLastError());
            break;
        }

        SERVICE_STATUS service_status = { 0x0 };
        if (ControlService(service_control_service, SERVICE_CONTROL_STOP, &service_status))
        {
            Sleep(1000);
            while (QueryServiceStatus(service_control_service, &service_status))
            {
                if (SERVICE_STOP_PENDING != service_status.dwCurrentState)
                {
                    break;
                }
                Sleep(1000);
            }
        }

        if (!DeleteService(service_control_service))
        {
            ret = (ERROR_SERVICE_MARKED_FOR_DELETE == GetLastError());
            break;
        }

        ret = true;
        break;
    } while (false);

    if (nullptr != service_control_service)
    {
        CloseServiceHandle(service_control_service);
    }
    if (nullptr != service_control_manager)
    {
        CloseServiceHandle(service_control_manager);
    }

    return (ret);
}

struct service_args_t
{
    int     argc;
    char ** argv;
};

static service_args_t               s_service_args = { 0x0 };
static SERVICE_STATUS               s_service_status;
static SERVICE_STATUS_HANDLE        s_service_status_handle;

static void write_event_log(const char * message, WORD event_type)
{
    if (nullptr == message)
    {
        return;
    }

    HANDLE event_source_handle = RegisterEventSourceA(nullptr, s_service_name.c_str());
    if (nullptr == event_source_handle)
    {
        return;
    }

    const char * strings[] = { s_service_name.c_str(), message };
    ReportEventA(event_source_handle, event_type, 0, 0, nullptr, sizeof(strings) / sizeof(strings[0]), 0, strings, nullptr);
    DeregisterEventSource(event_source_handle);
}

static void write_info_event(const char * message)
{
    write_event_log(message, EVENTLOG_INFORMATION_TYPE);
}

static void write_error_event(const char * message)
{
    write_event_log(message, EVENTLOG_ERROR_TYPE);
}

static void write_error_event(const char * message, DWORD error_code)
{
    if (nullptr == message)
    {
        return;
    }
    char buffer[512] = { 0x0 };
    _snprintf(buffer, sizeof(buffer) - 1, "%s, errno (%u)", message, static_cast<unsigned int>(error_code));
    write_error_event(buffer);
}

static unsigned int __stdcall run_service(void *)
{
    if (nullptr == s_service_args.argv || nullptr == s_service)
    {
        return (-1);
    }
    write_info_event("service in on_start");
    if (!s_service->on_start(s_service_args.argc, s_service_args.argv))
    {
        write_error_event("service on_start failed");
        return (-1);
    }
    while (s_service->running())
    {
        Sleep(1000);
    }
    return (0);
}

static bool create_service_thread()
{
    HANDLE thread_handle = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, run_service, nullptr, 0, nullptr));
    if (nullptr == thread_handle)
    {
        return (false);
    }
    else
    {
        CloseHandle(thread_handle);
        return (true);
    }
}

static void set_service_status(DWORD new_status, DWORD exit_code = 0)
{
    if (nullptr == s_service_status_handle)
    {
        return;
    }

    static DWORD s_check_point = 0;

    s_service_status.dwCurrentState = new_status;
    s_service_status.dwWin32ExitCode = exit_code;
    s_service_status.dwCheckPoint = (SERVICE_RUNNING == new_status || SERVICE_STOPPED == new_status ? 0 : ++s_check_point);

    SetServiceStatus(s_service_status_handle, &s_service_status);
}

static void start_service()
{
    try
    {
        set_service_status(SERVICE_START_PENDING);
        if (!create_service_thread())
        {
            write_error_event("service create thread failed");
            throw "create service thread failed";
        }
        set_service_status(SERVICE_RUNNING);
    }
    catch (DWORD error_code)
    {
        write_error_event("service start failed", error_code);
        set_service_status(SERVICE_STOPPED, error_code);
    }
    catch (...)
    {
        write_error_event("service start failed");
        set_service_status(SERVICE_STOPPED);
    }
}

static void stop_service()
{
    DWORD old_status = s_service_status.dwCurrentState;
    try
    {
        set_service_status(SERVICE_STOP_PENDING);
        write_info_event("service in on_stop");
        if (!s_service->on_stop())
        {
            write_error_event("service on_stop failed");
            throw "on_stop failed";
        }
        set_service_status(SERVICE_STOPPED);
    }
    catch (DWORD error_code)
    {
        write_error_event("service stop failed", error_code);
        set_service_status(old_status, error_code);
    }
    catch (...)
    {
        write_error_event("service stop failed");
        set_service_status(old_status);
    }
}

static void shutdown_service()
{
    try
    {
        write_info_event("service in on_stop");
        if (!s_service->on_stop())
        {
            write_error_event("service on_stop failed");
            throw "on_stop failed";
        }
        set_service_status(SERVICE_STOPPED);
    }
    catch (DWORD error_code)
    {
        write_error_event("service shutdown failed", error_code);
    }
    catch (...)
    {
        write_error_event("service shutdown failed");
    }
}

static void WINAPI service_ctrl(DWORD ctrl)
{
    switch (ctrl)
    {
        case SERVICE_CONTROL_STOP:
        {
            stop_service();
            break;
        }
        case SERVICE_CONTROL_SHUTDOWN:
        {
            shutdown_service();
            break;
        }
        default:
        {
            break;
        }
    }
}

static void WINAPI service_main(DWORD, LPSTR *)
{
    s_service_status_handle = RegisterServiceCtrlHandlerA(s_service_name.c_str(), service_ctrl);
    if (nullptr == s_service_status_handle)
    {
        write_error_event("service register ctrl failed");
        throw GetLastError();
    }
    start_service();
}

#else

static bool                         s_log_has_open = false;

static void open_event_log()
{
    if (!s_log_has_open)
    {
        s_log_has_open = true;
        openlog(s_service_name.c_str(), LOG_PID, LOG_DAEMON);
    }
}

static void close_event_log()
{
    if (s_log_has_open)
    {
        closelog();
        s_log_has_open = false;
    }
}

static void write_info_event(const char * message)
{
    open_event_log();

    syslog(LOG_INFO, "%s", message);
}

static void write_error_event(const char * message)
{
    open_event_log();

    syslog(LOG_ERR, "%s", message);
}

static void write_error_event(const char * message, int error_code)
{
    open_event_log();

    syslog(LOG_ERR, "%s, errno (%u)", message, error_code);
}

/*

the below comments show how to create linux service

step1) vim /etc/init.d/system_service_testd

    #!/bin/bash

    #chkconfig: 0123456 95 95
    #description: system service tester

    path=/home/yanrk/codes/test/test_daemon

    start() {
        $path
    }

    stop() {
        ps -eo pid,cmd | grep $path | grep -v grep | awk '{print $1}' | xargs kill -9 > /dev/null 2>&1
    }

    status() {
        ps -eo pid,cmd | grep $path | grep -v grep
    }

    restart() {
        stop
        start
    }

    usage() {
        echo "usage: $0 [start|stop|status|restart]"
    }

    case $1 in
        start):
            start
            ;;
        stop):
            stop
            ;;
        status):
            status
            ;;
        restart):
            restart
            ;;
        reload | force-reload):
            echo "not support '$1'" >&2
            exit 1
            ;;
        *):
            usage
            ;;
    esac

step2) install/uninstall system_service_testd

    way 1)
        cd /etc/init.d
        chmod +x ./system_service_testd
        chkconfig --add system_service_testd
        chkconfig system_service_testd on
        chkconfig system_service_testd off
        chkconfig --del system_service_testd
    way 2)
        cd /etc/init.d
        sudo chmod +x ./system_service_testd
        sudo update-rc.d system_service_testd default 95
        sudo update-rc.d -f system_service_testd remove
    way 3)
        vim /etc/rc.local
          or
        vim /etc/init.d/rc.local
            /home/yanrk/codes/test/test_daemon

*/

#endif // _MSC_VER

NAMESPACE_GOOFER_BEGIN

SystemServiceBase::SystemServiceBase()
{

}

SystemServiceBase::~SystemServiceBase()
{

}

bool SystemServiceBase::run(const char * service_name, int argc, char * argv [])
{
    if (nullptr == service_name || argc <= 0 || nullptr == argv)
    {
        return (false);
    }

    if (nullptr != s_service)
    {
        return (false);
    }

#ifdef _MSC_VER
    if (argc > 1)
    {
        if (0 == strcmp("-install", argv[1]) || 0 == strcmp("/install", argv[1]))
        {
            return (install_system_service(argv[0], service_name, service_name));
        }
        else if (0 == strcmp("-remove", argv[1]) || 0 == strcmp("/remove", argv[1]))
        {
            return (uninstall_system_service(service_name));
        }
    }

    s_service = this;

    s_service_name = service_name;

    s_service_args.argc = argc;
    s_service_args.argv = argv;

    s_service_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    s_service_status.dwCurrentState = SERVICE_START_PENDING;
    s_service_status.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    s_service_status.dwWin32ExitCode = NO_ERROR;
    s_service_status.dwServiceSpecificExitCode = 0;
    s_service_status.dwCheckPoint = 0;
    s_service_status.dwWaitHint = 0;

    s_service_status_handle = nullptr;

    SERVICE_TABLE_ENTRYA service_table[] = 
    {
        { const_cast<char *>(s_service_name.c_str()), service_main },
        { nullptr, nullptr }
    };

    if (!StartServiceCtrlDispatcherA(service_table))
    {
        write_error_event("service start dispatcher failed");
        return (false);
    }

    return (true);
#else
    bool ret = false;

    s_service = this;

    s_service_name = service_name;

    do
    {
        if (0 != daemon(0, 0))
        {
            write_error_event("service daemon failed", errno);
            break;
        }

        write_info_event("service in on_start");

        if (!on_start(argc, argv))
        {
            write_error_event("service on_start failed");
            break;
        }

        write_info_event("service on_start ok");

        while (s_service->running())
        {
            sleep(1);
        }

        write_info_event("service in on_stop");

        if (!on_stop())
        {
            write_error_event("service on_stop failed");
            break;
        }

        write_info_event("service on_stop ok");

        ret = true;
        break;
    } while (false);

    close_event_log();

    return (ret);
#endif // _MSC_VER
}

NAMESPACE_GOOFER_END
