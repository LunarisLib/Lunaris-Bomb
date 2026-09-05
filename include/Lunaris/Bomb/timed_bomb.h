#pragma once

#include <condition_variable>
#include <thread>
#include <mutex>

#include <Lunaris/Bomb/bomb.h>

namespace Lunaris {
namespace Bomb {

    /**
     * @brief A TimedBomb should feel like a bomb, but it has a timer trigger too! If timed out or destroyed, BOOM.
     * 
     */
    class TimedBomb {
    public:
        /**
         * @brief Construct a new bomb object
         * 
         * @param `callback` function to call on explosion
         * @param `seconds` how many seconds to timeout and explode anyway?
         */
        TimedBomb(std::function<void()> callback, const double seconds);

        /**
         * @brief Move bomb object to this
         * 
         * @param `oth` moving bomb
         */
        TimedBomb(TimedBomb&& oth);
        
        ~TimedBomb();

        /**
         * @brief Assign this bomb another bomb.
         * 
         * CAUTIOUS: if this bomb here was not defused, assigning another one to it explodes this one!
         * 
         * @param `oth` the moving bomb
         */
        void operator=(TimedBomb&& oth);

        /**
         * @brief Removes the bomb from it (returns the function and defuses itself)
         * 
         * @return `std::function<void()>` the function that'd be run on destruction or timeout
         */
        std::function<void()> defuse();

        /**
         * @brief Has it been defused?
         * 
         * @return `bool` true if defused
         */
        bool is_defused() const;
    private:
        struct _bomb {
            Bomb bomb;
            std::condition_variable cond;
            std::thread thr;
            std::mutex mtx;
            std::atomic<bool> cancel_timer{false};

            _bomb(std::function<void()>, const double);
            ~_bomb();
        };

        std::unique_ptr<_bomb> m_bomb;
    };
    
} // namespace Bomb
} // namespace Lunaris