#include <chrono>

#include <Lunaris/Bomb/timed_bomb.h>

namespace Lunaris {
namespace Bomb {
    
    TimedBomb::TimedBomb(std::function<void()> callback, const double seconds) {
        m_bomb = std::make_unique<_bomb>(std::move(callback), seconds);
    }

    TimedBomb::TimedBomb(TimedBomb&& oth) 
        : m_bomb(std::move(oth.m_bomb))
    {}

    TimedBomb::~TimedBomb() {
        m_bomb.reset();
    }

    void TimedBomb::operator=(TimedBomb&& oth) {
        m_bomb = std::move(oth.m_bomb);
    }

    std::function<void()> TimedBomb::defuse() {
        auto fcn = m_bomb->bomb.defuse();
        m_bomb.reset();
        return fcn;
    }

    bool TimedBomb::is_defused() const {
        return m_bomb ? m_bomb->bomb.is_defused() : true;
    }

    TimedBomb::_bomb::_bomb(std::function<void()> fcn, const double seconds)
        : bomb(std::move(fcn))
    {
        thr = std::thread(
            [this, timeout_sec = std::chrono::duration<double>(seconds)] {
                std::unique_lock<std::mutex> l(this->mtx);

                const bool cancelled = this->cond.wait_for(l,
                    timeout_sec,
                    [this]{ return this->bomb.is_defused() || this->cancel_timer; });

                l.unlock();

                if (cancelled || this->cancel_timer) // this->cancel_timer may happen late here instead of there
                    return;
                
                if (const auto callback = this->bomb.defuse(); callback)
                    callback();
            }
        );
    }

    TimedBomb::_bomb::~_bomb() {
        cancel_timer = true;
        cond.notify_one();
        thr.join();
    }

} // namespace Bomb
} // namespace Lunaris