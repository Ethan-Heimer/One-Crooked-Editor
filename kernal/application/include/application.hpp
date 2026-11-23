#pragma once

#include <memory>
#include <string_view>
#include <sys/socket.h>
#include <type_traits>
#include <vector>

#include "requestbus.hpp"
#include "subprocess.hpp"

#include "utils/threadconstructor.hpp"
#include "utils/subprocessconstructor.hpp"
#include "utils/applicationtypes.hpp"
#include "utils/subprocessinfo.hpp"
#include "utils/memoryusageinfo.hpp"

namespace CrookedEditor::Application{
    class Application final{
        public:
            Application(Events::RequestBus& requestBus);
            ~Application();
            Application(const Application&) = delete;
            Application(Application&&) = delete;

            Application& operator=(const Application&) = delete;
            Application& operator=(Application&&) = delete;

            void CleanUp();

            ThreadHandle SpawnThread(ThreadConstructor threadConstructor) const;
            SubprocessInfo SpawnSubprocess(SubprocessConstructor process) const;

            Process::FileDescripter GetInputFileDescriptor(SubprocessHandle handle) const;
            Process::FileDescripter GetOutputFileDescriptor(SubprocessHandle handle) const;

            void Kill(SubprocessHandle handle);
            void KillThread(ThreadHandle handle);

            bool DoesThreadExist(ThreadHandle handle);

            void PrintLog(std::string_view message);

            MemoryUsageData MemoryUsage();

            int GetThreadCount();
            std::vector<int> GetThreadIds();

            void Quit();

            template<typename Func, typename... Args>
            ThreadHandle SpawnThread(Func func, Args... args) const {
                return SpawnThread({func, args...});
            }
            
            template<typename... Args>
            requires(std::is_same_v<const char*, Args> && ...)
            SubprocessInfo SpawnSubprocess(std::string_view process, Args... args) const {
                return SpawnSubprocess(SubprocessConstructor{process, args...});
            } 
        private:
            struct Impl;
            std::unique_ptr<Impl> pImpl;
    };
}
