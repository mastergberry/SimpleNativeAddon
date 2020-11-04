#include <napi.h>
#include <iostream>
#include <chrono>
#include <thread>

#ifdef _WIN32
#include "windows.h"
#include <processthreadsapi.h>
#else
#include <unistd.h>
#endif

using namespace std;

Napi::Value ExampleMethod(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

#ifdef _WIN32
    DWORD pid = GetProcessId(GetCurrentProcess());
#else
    pid_t pid = getpid();
#endif

    cout << "ExampleMethod is called from pid " << pid << endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    int randomNumber = 1;
    for (int i = 1; i <= 100; i++)
        randomNumber += i;

    Napi::Number num = Napi::Number::New(env, randomNumber);

    return num;
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "exampleMethod"), Napi::Function::New(env, ExampleMethod));

    // Uncomment if you are trying to test on Windows and want to attach a debugger
    //std::this_thread::sleep_for(std::chrono::seconds(10));

    cout << "Init loaded" << endl;

    return exports;
}

// Register native add on
NODE_API_MODULE(addon, Init)