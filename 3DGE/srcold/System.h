#ifndef _SYSTEM_H_
#define _SYSTEM_H_

namespace GE {

    /**
     * A system manages the application of a component or components onto the
     * entity that it is associated with.
     */
    class System {
    public:
        System(class SystemBlackboard *blackboard);
        ~System();

        virtual void run() = 0;

    protected:
        SystemBlackboard *m_blackboard;
    };
}
