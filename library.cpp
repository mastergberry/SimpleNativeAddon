#include <nan.h>
#include <iostream>
#include <chrono>
#include <thread>

#ifdef _WIN32
#include <processthreadsapi.h>
#else
#include <unistd.h>
#endif

using Nan::New;
using Nan::Set;
using Nan::GetFunction;
using v8::FunctionTemplate;
using v8::String;
using v8::Integer;

using namespace std;

NAN_METHOD(ExampleMethod)
{
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

    info.GetReturnValue().Set(Nan::New<Integer>(static_cast<int>(randomNumber)));
}

NAN_MODULE_INIT(Init)
{
    Set(target, New<String>("exampleMethod").ToLocalChecked(), GetFunction(New<FunctionTemplate>(ExampleMethod)).ToLocalChecked());
}

// Register native add on
#if NODE_MODULE_VERSION >= 80
NAN_MODULE_WORKER_ENABLED(SimpleNativeAddon, Init)
#else
NODE_MODULE(SimpleNativeAddon, Init)
#endif