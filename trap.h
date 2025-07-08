#ifndef TRAP_H
#define TRAP_H

#include <string>

class Trap
{ // Would be a good idea to use config like monster and dungeon if i add more trap types
protected:
    int damage;
    std::string name;

public:
    Trap(int difficulty);
    virtual ~Trap() = default;

    int getDamage() const;
    int getAvoidance() const;
    virtual std::string to_string() const;
};

class WallArrows : public Trap
{
public:
    WallArrows(int difficulty);
};

class SwingingAxe : public Trap
{
public:
    SwingingAxe(int difficulty);
};

class RollingBoulder : public Trap
{
public:
    RollingBoulder(int difficulty);
};
#endif