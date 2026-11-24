#pragma once

#include <memory>
#include <string_view>
#include <sys/socket.h>
#include <type_traits>
#include <vector>

#include "requestbus.hpp"
#include "subprocess.hpp"

#include "applicationtypes.hpp"

#include "threadconstructor.hpp"
#include "subprocessconstructor.hpp"
#include "subprocessinfo.hpp"
#include "memoryusageinfo.hpp"

namespace CrookedEditor::Runtime{
    class RuntimeResourceManager final{
        public:
            RuntimeResourceManager(Events::RequestBus& requestBus);
            ~RuntimeResourceManager();
            RuntimeResourceManager(const RuntimeResourceManager&) = delete;
            RuntimeResourceManager(RuntimeResourceManager&&) = delete;

            RuntimeResourceManager& operator=(const RuntimeResourceManager&) = delete;
            RuntimeResourceManager& operator=(RuntimeResourceManager&&) = delete;

            void CleanUp();
            void Quit();

            API::ThreadHandle SpawnThread(ThreadConstructor threadConstructor) const;
            API::SubprocessInfo SpawnSubprocess(SubprocessConstructor process) const;

            Process::FileDescripter GetInputFileDescriptor(API::SubprocessHandle handle) const;
            Process::FileDescripter GetOutputFileDescriptor(API::SubprocessHandle handle) const;

            void Kill(API::SubprocessHandle handle);
            void KillThread(API::ThreadHandle handle);

            bool DoesThreadExist(API::ThreadHandle handle);

            API::MemoryUsageData MemoryUsage();

            int GetThreadCount();
            std::vector<int> GetThreadIds();

            template<typename Func, typename... Args>
            API::ThreadHandle SpawnThread(Func func, Args... args) const {
                return SpawnThread({func, args...});
            }
            
            template<typename... Args>
            requires(std::is_same_v<const char*, Args> && ...)
            API::SubprocessInfo SpawnSubprocess(std::string_view process, Args... args) const {
                return SpawnSubprocess(SubprocessConstructor{process, args...});
            } 
        private:
            struct Impl;
            std::unique_ptr<Impl> pImpl;
    };
}
