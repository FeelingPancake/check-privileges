#include <napi.h>
#include <windows.h>
#include <lm.h>
#include <string>
#include <codecvt>
#include <locale>

#pragma comment(lib, "netapi32.lib")

std::wstring stringToWString(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(str);
}

std::string wstringToString(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

Napi::String CheckPrivileges(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
    }

    std::string username = info[0].As<Napi::String>().Utf8Value();
    std::wstring wUsername = stringToWString(username);

    USER_INFO_1* userInfo = nullptr;
    NET_API_STATUS status = NetUserGetInfo(NULL, wUsername.c_str(), 1, (LPBYTE*)&userInfo);
    if (status == NERR_Success) {
        std::wstring privilege;
        if (userInfo->usri1_priv == USER_PRIV_ADMIN) {
            privilege = L"Administer";
        } else if (userInfo->usri1_priv == USER_PRIV_USER) {
            privilege = L"User";
        } else if (userInfo->usri1_priv == USER_PRIV_GUEST) {
            privilege = L"Guest";
        } else {
            privilege = L"Unknown";
        }
        NetApiBufferFree(userInfo);
        return Napi::String::New(env, wstringToString(privilege));
    } else {
        return Napi::String::New(env, "");
    }
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "checkPrivileges"), Napi::Function::New(env, CheckPrivileges));
    return exports;
}

NODE_API_MODULE(checkPrivileges, Init)
