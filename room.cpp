#include "room.h"

std::random_device Room::rd;
std::mt19937 Room::rng(Room::rd());


Room::Room(int difficulty)
{
    rng.seed(rd());

    std::uniform_int_distribution<int> typeDist(0, 4);
    std::uniform_int_distribution<int> pathDist(0, 3);
    std::uniform_int_distribution<int> chestDist(0, 1);
    std::uniform_int_distribution<int> optionalDist(0, 5);
    std::uniform_int_distribution<int> monsterDist(0, difficulty);

    type = typeDist(rng);
    path_l = pathDist(rng);
    path_m = pathDist(rng);
    path_r = pathDist(rng);

    if (chestDist(rng))
        chest = 1;
    if (optionalDist(rng) == 1)
        corpse = pathDist(rng);
    if (optionalDist(rng) == 1)
        engraving = pathDist(rng);
    if (optionalDist(rng) == 1)
        trap = pathDist(rng);
    if (optionalDist(rng) == 1)
        monster = monsterDist(rng);
}

std::string Room::to_string() const
{
    std::string roomInfo = "Room:\n\n";
    roomInfo += "Type: " + std::to_string(type) + "\n\n";
    roomInfo += "Paths: " + std::to_string(path_l) + ", " +
                std::to_string(path_m) + ", " +
                std::to_string(path_r) + "\n\n";
    if (chest)
        roomInfo += "Chest: " + std::to_string(*chest) + "\n\n";
    if (corpse)
        roomInfo += "Corpse: " + std::to_string(*corpse) + "\n\n";
    if (engraving)
        roomInfo += "Engraving: " + std::to_string(*engraving) + "\n\n";
    if (trap)
        roomInfo += "Trap: " + std::to_string(*trap) + "\n\n";
    if (monster)
        roomInfo += "Monster: " + std::to_string(*monster) + "\n\n";

    return roomInfo;
}
