#include <iostream>

#include <Lunaris/bomb.h>

using namespace Lunaris::Bomb;

int main() {
    std::printf("Testing TimeBomb as Bomb in its usual way (large timeout)...\n");

    {
        std::atomic<bool> defused_by_func{false};
        {
            std::printf("Planting a bomb...\n");
            TimedBomb bomb_that_defuses_aint_it_fun([&defused_by_func]{
                defused_by_func = true;
            }, 999);
            std::printf("Dropping the bomb...\n");
        }

        if (!defused_by_func) {
            std::printf("Bomb did not explode.\n");
            return 1;
        }
        else {
            std::printf("Bomb exploded as expected.\n");
        }
    }

    std::printf("Testing defusing bomb now...\n");

    {
        std::atomic<bool> defused_by_func{false};
        std::function<void()> defused_callback;
        {
            std::printf("Planting a bomb...\n");
            TimedBomb bomb_that_defuses_aint_it_fun([&defused_by_func]{
                defused_by_func = true;
            }, 999);
            std::printf("Defusing the bomb...\n");

            defused_callback = bomb_that_defuses_aint_it_fun.defuse();

            std::printf("Asserting defused bomb...\n");

            if (!bomb_that_defuses_aint_it_fun.is_defused()) {
                std::printf("Bomb did not defuse properly.\n");
                return 1;
            }

            std::printf("Dropping the bomb...\n");
        }

        if (defused_by_func) {
            std::printf("Bomb did not defuse. It exploded!\n");
            return 1;
        }
        else {
            std::printf("Bomb defused as expected.\n");
        }

        std::printf("Testing defused callback...\n");

        if (!defused_callback) {
            std::printf("Bomb defuse callback broken!\n");
            return 1;            
        }

        defused_callback();

        if (!defused_by_func) {
            std::printf("Callback extracted from bomb did not work.\n");
            return 1;
        }
        else {
            std::printf("Callback worked.\n");
        }
    }

    std::printf("Testing timed bomb for real now...\n");

    {
        std::atomic<bool> defused_by_func{false};

        std::printf("Planting a bomb...\n");

        auto now = std::chrono::steady_clock::now();
        TimedBomb bomb_that_defuses_aint_it_fun([&defused_by_func]{
            defused_by_func = true;
        }, 2.0);

        std::printf("Waiting up to 3 seconds for a 2 seconds timed bomb to activate...\n");

        while(std::chrono::steady_clock::now() - now < std::chrono::duration<double>(3.0) && !defused_by_func)
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

        double delta{ std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - now).count() };

        if (!defused_by_func) {
            std::printf("Timed bomb did not work.\n");
            return 1;
        }

        std::printf("Timed bomb exploded in %.5lf seconds\n", delta);

        if (delta > 2.5) {
            std::printf("Timed bomb took too much time compared to requested 2 seconds.\n");
            return 1;
        }
    }
    std::printf("Testing timed bomb, but defused...\n");

    {
        std::atomic<bool> defused_by_func{false};

        std::printf("Planting a bomb for 0.5 sec...\n");

        TimedBomb bomb_that_defuses_aint_it_fun([&defused_by_func]{
            defused_by_func = true;
        }, 0.5);

        std::printf("Defusing it and waiting...\n");

        auto defused_callback = bomb_that_defuses_aint_it_fun.defuse();

        std::this_thread::sleep_for(std::chrono::seconds(1));

        if (defused_by_func) {
            std::printf("Timed bomb defuse did not work.\n");
            return 1;
        }

        std::printf("Timed bomb did not explode! Yay!\n");

        std::printf("Testing defused callback...\n");

        if (!defused_callback) {
            std::printf("Bomb defuse callback broken!\n");
            return 1;            
        }

        defused_callback();

        if (!defused_by_func) {
            std::printf("Callback extracted from bomb did not work.\n");
            return 1;
        }
        else {
            std::printf("Callback worked.\n");
        }
    }


    std::printf("PASSED!\n");
    return 0;
}