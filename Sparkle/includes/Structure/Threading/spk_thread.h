#pragma once

#include <string>
#include <functional>
#include <thread>
#include <future>

#include "Structure/Utils/spk_iostream.h"

namespace spk
{
    class Thread {
        friend class WorkerPool;

    public:
        enum class LaunchMethod { Delayed, Immediate };

    private:
        std::string _threadName;
        std::function<void()> _funct;
        std::thread _thread;
        std::promise<bool> _starterSignal;

    public:
        template <typename Funct, typename... Args>
        Thread(std::string p_threadName, Funct&& p_func, Args&&... p_args)
            : _threadName(std::move(p_threadName)),
            _funct(std::bind(std::forward<Funct>(p_func), std::forward<Args>(p_args)...)),
            _starterSignal() {
            auto wrapper = [this]() {
                spk::cout.setPrefix(_threadName);
                spk::cerr.setPrefix(_threadName);
                _starterSignal.get_future().wait();
                _funct();
            };
            _thread = std::thread(wrapper);
        }

        ~Thread() {
            join();
        }

        template <typename Funct, typename... Args>
        Thread(LaunchMethod p_launchMethod, std::string p_threadName, Funct&& p_func, Args&&... p_args) :
                Thread(p_threadName, p_func, p_args...)
            {
            if (p_launchMethod == LaunchMethod::Immediate) {
                start();
            }
        }

        std::thread::id getId() const
        {
            return (_thread.get_id());
        }

        void join()
        {
            if (_thread.joinable())
                _thread.join();
        }

        void start() {
            _starterSignal.set_value(true);
        }
    };
}