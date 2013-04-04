#ifndef POLLSCHEDULER_H_
#define POLLSCHEDULER_H_

#include <cscheduler.h>
#include <cmessage.h>
#include <cmodule.h>
#include <platdep/timeutil.h>
#include <poll.h>

class PollScheduler: public cScheduler {
private:
    pollfd poll_fd[20];
    nfds_t n_fds;
    cModule *module[20];
    cMessage *message[20];
    timeval start;

public:
    PollScheduler();
    virtual ~PollScheduler();

    virtual void startRun();
    virtual void endRun();
    virtual void executionResumed();
    virtual cMessage *getNextEvent();

    virtual void registerFD(cModule *module,int fd);
};

#endif /* POLLRADIOSCHEDULER_H_ */
