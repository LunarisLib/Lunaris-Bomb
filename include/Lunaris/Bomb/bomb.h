#pragma once

#include <functional>
#include <atomic>

namespace Lunaris {
namespace Bomb {
    
    /**
     * @brief Bomb is a really good tool when you need something to run then everything is destroyed (when things goes out of scope).
     * 
     * You create the bomb with a function. The function is run when the object is destroyed. Easy, right?
     * 
     * You can defuse the bomb if you don't want the function running at the end, maybe you've set the bomb for exception recovery and if things goes right you defuse it, I don't know.
     */
    class Bomb {
    public:
        /**
         * @brief Construct a new bomb object
         * 
         * @param `callback` function to call on explosion
         */
        Bomb(std::function<void()> callback);

        /**
         * @brief Move bomb object to this
         * 
         * @param `oth` moving bomb
         */
        Bomb(Bomb&& oth) noexcept;
        
        ~Bomb();

        /**
         * @brief Assign this bomb another bomb.
         * 
         * CAUTIOUS: if this bomb here was not defused, assigning another one to it explodes this one!
         * 
         * @param `oth` the moving bomb
         */
        void operator=(Bomb&& oth);

        /**
         * @brief Removes the bomb from it (returns the function and defuses itself)
         * 
         * @return `std::function<void()>` the function that'd be run on destruction
         */
        std::function<void()> defuse();

        /**
         * @brief Has it been defused?
         * 
         * @return `bool` true if defused
         */
        bool is_defused() const;
    private:
        void explode_if_not_defused();

        std::function<void()> m_callback;
        std::atomic<bool> m_defused{false};
    };

} // namespace Bomb
} // namespace Lunaris