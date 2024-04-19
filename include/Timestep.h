#ifndef TIMESTEP_H
#define TIMESTEP_H

namespace Enxus
{
    class Timestep
    {
    public:
        Timestep(float time) : m_Time(time) {}
        ~Timestep(){};

        operator float() { return m_Time; }

        float GetSeconds() const { return m_Time; }
        float GetMiliseconds() const { return m_Time * 1000; }

    private:
        float m_Time;
    };

}

#endif