#ifndef TRAP_H
#define TRAP_H

#include <string>

class Trap {
protected:
    int damage;
    int avoidance; //between 1 and 24?? idk will see how i end up using it
    std::string name;

public:
    Trap(int difficulty);
    virtual ~Trap() = default;
    
    int getDamage() const;
    int getAvoidance() const;
    virtual std::string to_string() const;
    
    virtual void adjustForDifficulty(int difficulty) = 0;
};

class WallArrows : public Trap {
public:
    WallArrows(int difficulty);
    void adjustForDifficulty(int difficulty) override;
};

class SwingingAxe: public Trap {
public:
    SwingingAxe(int difficulty);
    void adjustForDifficulty(int difficulty) override;
};
#endif