#pragma once

class mode
{
public:
    virtual void executer(int iterations) = 0; // Méthode pure virtuelle
    virtual ~mode() {}
};
