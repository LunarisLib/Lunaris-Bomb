#include <Lunaris/Bomb/bomb.h>

namespace Lunaris {
namespace Bomb {
    
    Bomb::Bomb(std::function<void()> callback)
        : m_callback(std::move(callback))
    {}

    Bomb::Bomb(Bomb&& oth) noexcept
        : m_callback(std::move(oth.m_callback))
    {
        oth.m_defused = true;
    }

    Bomb::~Bomb() {
        explode_if_not_defused();
    }

    void Bomb::operator=(Bomb&& oth) {
        explode_if_not_defused();
        m_callback = std::move(oth.m_callback);
        oth.m_defused = true;
    }

    std::function<void()> Bomb::defuse() {
        m_defused = true;
        return std::move(m_callback);
    }

    bool Bomb::is_defused() const {
        return m_defused;
    }

    void Bomb::explode_if_not_defused() {
        if (!m_defused) {
            m_defused = true; // avoid double bomb
            if (m_callback)
                m_callback();
        }
    }

} // namespace Bomb
} // namespace Lunaris