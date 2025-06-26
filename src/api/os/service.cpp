#include <api/std.h>

#include <api/os/service.h>
#include <windows.h>
#include <shellapi.h>
#include <winsvc.h>
#include <sstream>
#include <iostream>
#include <string>
#include <spdlog/spdlog.h>
#include <api/runtime.h>

namespace service {

    namespace {
        ServiceConfig g_api_service_config;
    }

    // UTF-8 -> UTF-16 转换函数
    std::wstring utf8_to_utf16(const std::string& str) {
        if (str.empty()) return {};
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
        std::wstring result(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &result[0], size_needed);
        return result;
    }

    std::string utf16_to_utf8(const std::wstring& wstr) {
        if (wstr.empty()) return {};
        int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
        std::string result(size_needed, 0);
        WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &result[0], size_needed, NULL, NULL);
        return result;
    }

    std::string get_last_error_string() {
        DWORD error = GetLastError();
        if (error == 0) {
            return "No error.";
        }

        LPWSTR messageBuffer = nullptr;
        size_t size = FormatMessageW(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            error,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPWSTR)&messageBuffer,
            0,
            NULL);

        std::wstring messageW(messageBuffer, size);
        LocalFree(messageBuffer);

        // Convert wide string to UTF-8
        int bufferSize = WideCharToMultiByte(CP_UTF8, 0, messageW.c_str(), -1, NULL, 0, NULL, NULL);
        std::string messageA(bufferSize, 0);
        WideCharToMultiByte(CP_UTF8, 0, messageW.c_str(), -1, &messageA[0], bufferSize, NULL, NULL);

        // 替换所有 \r\n 和 \n 为 空格
        messageA.erase(
            std::remove(messageA.begin(), messageA.end(), '\r'),
            messageA.end()
        );
        messageA.erase(
            std::remove(messageA.begin(), messageA.end(), '\n'),
            messageA.end()
        );

        // Trim newline
        if (!messageA.empty() && messageA.back() == '\n') {
            messageA.pop_back();
        }
        if (!messageA.empty() && messageA.back() == '\r') {
            messageA.pop_back();
        }

        std::ostringstream oss;
        oss << messageA << " (" << error << ")";
        return oss.str();
    }

    bool is_admin() {
        BOOL fRet = FALSE;
        HANDLE hToken = NULL;
        if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
            TOKEN_ELEVATION_TYPE elevType;
            DWORD cbSize;
            if (GetTokenInformation(hToken, TokenElevationType, &elevType, sizeof(elevType), &cbSize)) {
                fRet = (elevType == TokenElevationTypeFull);
            }
        }
        if (hToken) CloseHandle(hToken);
        return fRet;
    }

    namespace {
        std::string GetTempFileNameForOutput() {
            char tempPath[MAX_PATH];
            GetTempPathA(MAX_PATH, tempPath);

            char tempFile[MAX_PATH];
            GetTempFileNameA(tempPath, "out", 0, tempFile);
            return std::string(tempFile);
        }

        std::string ReadFileContent(const std::string& filePath) {
            std::ifstream file(filePath);
            if (!file) return "";

            std::ostringstream ss;
            ss << file.rdbuf();
            file.close();
            std::remove(filePath.c_str());
            return ss.str();
        }
    }

    bool require_admin_and_do(const std::string& choice) {
        if (is_admin()) {
            return true; // 已有管理员权限
        }

        WCHAR path[MAX_PATH];
        if (GetModuleFileNameW(NULL, path, MAX_PATH) == 0) {
            std::cerr << "[-] Failed to get executable path." << std::endl;
            return false;
        }
        std::string tempFile = GetTempFileNameForOutput();
        //std::cout << tempFile << std::endl;
        std::wstring params = utf8_to_utf16("\"service\" \"" + choice + "\"");
        params += L" \"--pipe=" + utf8_to_utf16(tempFile) + L"\"";
        SHELLEXECUTEINFOW sei = {};
        sei.cbSize = sizeof(sei);
        sei.fMask = SEE_MASK_NOASYNC | SEE_MASK_NOCLOSEPROCESS;
        sei.lpVerb = L"runas";              // 请求管理员权限
        sei.lpFile = path;                  // 当前程序路径
        sei.nShow = SW_NORMAL;
        sei.lpParameters = params.c_str();

        std::cout << "[+] 申请管理员权限(UAC自主提权)..." << std::endl;
        if (!ShellExecuteExW(&sei)) {
            std::cerr << "[-] Failed to request admin privileges." << std::endl;
            return false;
        } else {
            WaitForSingleObject(sei.hProcess, INFINITE); // 等待子进程结束
            CloseHandle(sei.hProcess);
            std::cout << "[+] 管理员权限操作, 继续:" << std::endl;
            std::cout << ReadFileContent(tempFile) << std::endl;
            std::cout << "[+] 管理员权限操作, 结束." << std::endl;
        }

        return false; // 当前进程退出，由新管理员进程接管
    }

    // 安装服务
    void install() {
        // 自动提权逻辑
        if (!require_admin_and_do("install")) {
            return; // 当前进程退出，由提权后的新进程继续运行
        }

        // 正常流程
        std::cout << "[+] Running with administrator privileges." << std::endl;
        spdlog::error("[+] Running with administrator privileges.");
        SC_HANDLE schSCManager = OpenSCManagerW(nullptr, nullptr, SC_MANAGER_CREATE_SERVICE);
        if (!schSCManager) {
            std::cerr << "[-] Failed to open SCManager (" << get_last_error_string() << ")" << std::endl;
            spdlog::warn("[-] Failed to open SCManager ({})", get_last_error_string());
            return;
        }

        std::wstring serviceNameW = utf8_to_utf16(g_api_service_config.service_name);
        std::wstring displayNameW = utf8_to_utf16(g_api_service_config.display_name);
        CHAR path[MAX_PATH];
        GetModuleFileName(nullptr, path, MAX_PATH);
        std::string exec_path = path;
        exec_path.append(" service run");
        std::wstring execute = utf8_to_utf16(exec_path);
        SC_HANDLE schService = CreateServiceW(
            schSCManager,
            serviceNameW.c_str(),
            displayNameW.c_str(),
            SERVICE_ALL_ACCESS,
            SERVICE_WIN32_OWN_PROCESS,
            SERVICE_AUTO_START,
            SERVICE_ERROR_NORMAL,
            execute.c_str(),
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            nullptr
        );

        if (!schService) {
            DWORD err = GetLastError();
            if (err == ERROR_SERVICE_EXISTS) {
                std::cerr << "[!] Service already exists." << std::endl;
                spdlog::warn("[!] Service already exists.");
            } else {
                std::cerr << "[-] CreateService failed (" << err << ")" << std::endl;
                spdlog::warn("[-] CreateService failed ({})", err);
            }
            CloseServiceHandle(schSCManager);
            return;
        }

        // 设置服务描述
        if (!g_api_service_config.description.empty()) {
            std::wstring descW = utf8_to_utf16(g_api_service_config.description);
            SERVICE_DESCRIPTIONW sd;
            sd.lpDescription = const_cast<LPWSTR>(descW.c_str());
            ChangeServiceConfig2W(schService, SERVICE_CONFIG_DESCRIPTION, &sd);
        }

        std::cout << "[+] Service installed successfully: " << g_api_service_config.service_name << std::endl;
        spdlog::warn("[+] Service installed successfully: {}", g_api_service_config.service_name);
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
    }

    // 卸载服务
    void uninstall() {
        // 自动提权逻辑
        if (!require_admin_and_do("uninstall")) {
            return; // 当前进程退出，由提权后的新进程继续运行
        }

        // 正常流程
        std::cout << "[+] Running with administrator privileges." << std::endl;
        SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
        if (!schSCManager) {
            std::cerr << "[-] Failed to open SCManager (" << get_last_error_string() << ")" << std::endl;
            return;
        }

        std::wstring serviceNameW = utf8_to_utf16(g_api_service_config.service_name);
        SC_HANDLE schService = OpenServiceW(schSCManager, serviceNameW.c_str(), DELETE);
        if (!schService) {
            std::cerr << "[-] Failed to open service (" << get_last_error_string() << ")" << std::endl;
            CloseServiceHandle(schSCManager);
            return;
        }

        if (!DeleteService(schService)) {
            std::cerr << "[-] DeleteService failed (" << get_last_error_string() << ")" << std::endl;
        } else {
            std::cout << "[+] Service uninstalled: " << g_api_service_config.service_name << std::endl;
        }

        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
    }

    // 启动服务
    void start() {
        // 自动提权逻辑
        if (!require_admin_and_do("start")) {
            return; // 当前进程退出，由提权后的新进程继续运行
        }

        SC_HANDLE schSCManager = OpenSCManagerW(nullptr, nullptr, SC_MANAGER_CONNECT);
        if (!schSCManager) {
            std::cerr << "[-] Failed to open SCManager (" << get_last_error_string() << ")" << std::endl;
            return;
        }

        std::wstring serviceNameW = utf8_to_utf16(g_api_service_config.service_name);
        SC_HANDLE schService = OpenServiceW(schSCManager, serviceNameW.c_str(), SERVICE_START);
        if (!schService) {
            std::cerr << "[-] Failed to open service (" << get_last_error_string() << ")" << std::endl;
            CloseServiceHandle(schSCManager);
            return;
        }

        if (!::StartService(schService, 0, nullptr)) {
            std::cerr << "[-] StartService failed (" << get_last_error_string() << ")" << std::endl;
        } else {
            std::cout << "[+] Service started: " << g_api_service_config.service_name << std::endl;
        }

        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
    }

    // 停止服务
    void stop() {
        // 自动提权逻辑
        if (!require_admin_and_do("stop")) {
            return; // 当前进程退出，由提权后的新进程继续运行
        }

        SC_HANDLE schSCManager = OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT);
        if (!schSCManager) {
            std::cerr << "[-] Failed to open SCManager (" << get_last_error_string() << ")" << std::endl;
            return;
        }

        std::wstring serviceNameW = utf8_to_utf16(g_api_service_config.service_name);
        SC_HANDLE schService = OpenServiceW(schSCManager, serviceNameW.c_str(), SERVICE_STOP);
        if (!schService) {
            std::cerr << "[-] Failed to open service (" << get_last_error_string() << ")" << std::endl;
            CloseServiceHandle(schSCManager);
            return;
        }

        SERVICE_STATUS status;
        if (!ControlService(schService, SERVICE_CONTROL_STOP, &status)) {
            std::cerr << "[-] StopService failed (" << get_last_error_string() << ")" << std::endl;
        } else {
            std::cout << "[+] Service stopped: " << g_api_service_config.service_name << std::endl;
        }

        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
    }

    // 查询状态
    void query_status() {
        SC_HANDLE schSCManager = OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT);
        if (!schSCManager) {
            std::cerr << "[-] Failed to open SCManager (" << get_last_error_string() << ")" << std::endl;
            return;
        }

        std::wstring serviceNameW = utf8_to_utf16(g_api_service_config.service_name);
        SC_HANDLE schService = OpenServiceW(schSCManager, serviceNameW.c_str(), SERVICE_QUERY_STATUS);
        if (!schService) {
            std::cerr << "[-] Failed to open service (" << get_last_error_string() << ")" << std::endl;
            CloseServiceHandle(schSCManager);
            return;
        }

        SERVICE_STATUS status;
        if (!QueryServiceStatus(schService, &status)) {
            std::cerr << "[-] QueryServiceStatus failed (" << get_last_error_string() << ")" << std::endl;
        } else {
            std::cout << "[+] Service status: ";
            switch (status.dwCurrentState) {
                case SERVICE_RUNNING:
                    std::cout << "Running";
                    break;
                case SERVICE_STOPPED:
                    std::cout << "Stopped";
                    break;
                case SERVICE_START_PENDING:
                    std::cout << "Starting";
                    break;
                case SERVICE_STOP_PENDING:
                    std::cout << "Stopping";
                    break;
                default:
                    std::cout << "Unknown";
                    break;
            }
            std::cout << std::endl;
        }

        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
    }

    // 全局变量
    SERVICE_STATUS          g_ServiceStatus = {};
    SERVICE_STATUS_HANDLE   g_StatusHandle = nullptr;
    HANDLE                  g_ServiceStopEvent = nullptr;

    // 服务控制处理函数
    VOID WINAPI ServiceCtrlHandler(DWORD CtrlCode) {
        switch (CtrlCode) {
            case SERVICE_CONTROL_STOP:
                // 收到停止请求
                if (g_ServiceStatus.dwCurrentState != SERVICE_RUNNING) {
                    break;
                }
                // 更新状态为停止中
                g_ServiceStatus.dwControlsAccepted = 0;
                g_ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
                g_ServiceStatus.dwWin32ExitCode = 0;
                g_ServiceStatus.dwCheckPoint = 4;

                if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE){
                    spdlog::error("{}: ServiceCtrlHandler: SetServiceStatus returned error", g_api_service_config.service_name);
                }

                // 设置停止事件
                SetEvent(g_ServiceStopEvent);
                break;

            default:
                break;
        }
    }

    // 服务主函数
    VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv)
    {
        (void)argc;
        (void)argv;
        std::wstring serviceNameW = utf8_to_utf16(g_api_service_config.service_name);
        // 注册服务控制处理函数
        g_StatusHandle = RegisterServiceCtrlHandlerW(serviceNameW.c_str(), ServiceCtrlHandler);

        if (g_StatusHandle == nullptr)
        {
            spdlog::error("{}: RegisterServiceCtrlHandler returned error", g_api_service_config.service_name);
            return;
        }

        // 初始状态为启动中
        ZeroMemory(&g_ServiceStatus, sizeof(g_ServiceStatus));
        g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
        g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
        g_ServiceStatus.dwControlsAccepted = 0;
        g_ServiceStatus.dwWin32ExitCode = 0;
        g_ServiceStatus.dwServiceSpecificExitCode = 0;
        g_ServiceStatus.dwCheckPoint = 0;

        if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
        {
            spdlog::error("{}: ServiceMain: SetServiceStatus returned error", g_api_service_config.service_name);
        }

        // 创建停止事件
        g_ServiceStopEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
        if (g_ServiceStopEvent == nullptr)
        {
            // 设置错误状态
            g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
            g_ServiceStatus.dwWin32ExitCode = GetLastError();
            g_ServiceStatus.dwCheckPoint = 1;

            if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
            {
                spdlog::error("{}: ServiceMain: SetServiceStatus returned error", g_api_service_config.service_name);
            }
            return;
        }

        // 更新状态为继续启动
        g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
        g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
        g_ServiceStatus.dwCheckPoint = 1;

        if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
        {
            spdlog::error("{}: ServiceMain: SetServiceStatus returned error", g_api_service_config.service_name);
        }

        // 模拟初始化工作（实际应用中替换为你的初始化代码）
        Sleep(1000); // 模拟耗时操作

        // 更新状态为运行中
        g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
        g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
        g_ServiceStatus.dwCheckPoint = 0;

        if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
        {
            spdlog::error("{}: ServiceMain: SetServiceStatus returned error", g_api_service_config.service_name);
        }

        // 主服务循环
        while (true)
        {
            // 等待停止事件或超时（实际应用中替换为你的服务逻辑）
            WaitForSingleObject(g_ServiceStopEvent, 3000);

            // 检查是否收到停止事件
            if (WaitForSingleObject(g_ServiceStopEvent, 0) == WAIT_OBJECT_0)
            {
                runtime::set_quit_flag(true);
                spdlog::warn("{}: ServiceMain: Received stop signal", g_api_service_config.service_name);
                break;
            }

            // 这里可以放置定期执行的服务任务
            spdlog::warn("{}: ServiceMain: Doing work...", g_api_service_config.service_name);
        }

        // 清理工作
        CloseHandle(g_ServiceStopEvent);

        // 更新状态为已停止
        g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        g_ServiceStatus.dwControlsAccepted = 0;
        g_ServiceStatus.dwWin32ExitCode = 0;
        g_ServiceStatus.dwCheckPoint = 3;

        if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
        {
            spdlog::warn("{}: ServiceMain: SetServiceStatus returned error", g_api_service_config.service_name);
        }
    }

    void run_daemon() {
        spdlog::info("[*] 守护进程已启动");
        //runtime::logger_set(false, config::is_debug());
        // 服务分派表
        SERVICE_TABLE_ENTRY DispatchTable[] =
            {
                { (LPTSTR)g_api_service_config.service_name.c_str(), (LPSERVICE_MAIN_FUNCTION)ServiceMain },
                { nullptr, nullptr }
            };

        if (!StartServiceCtrlDispatcher(DispatchTable)) {
            spdlog::error("StartServiceCtrlDispatcher failed ({})", GetLastError());
        }
        runtime::wait_for_exit();
    }
}