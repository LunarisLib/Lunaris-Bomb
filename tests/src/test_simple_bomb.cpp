#include <iostream>

#include <Lunaris/bomb.h>

using namespace Lunaris::Bomb;

int main() {
    std::printf("Testing bomb in its usual way...\n");

    {
        std::atomic<bool> defused_by_func{false};
        {
            std::printf("Planting a bomb...\n");
            Bomb bomb_that_defuses_aint_it_fun([&defused_by_func]{
                defused_by_func = true;
            });
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
            Bomb bomb_that_defuses_aint_it_fun([&defused_by_func]{
                defused_by_func = true;
            });
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

    std::printf("PASSED!\n");
    return 0;
}