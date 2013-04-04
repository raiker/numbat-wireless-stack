#include "PollScheduler.h"

Register_Class(PollScheduler)
;

PollScheduler::PollScheduler() :
		cScheduler(), n_fds(0) {
	for (size_t i = 0; i < sizeof(poll_fd) / sizeof(pollfd); i++) {
		poll_fd[i].fd = -1;
		poll_fd[i].events = POLLIN;
		poll_fd[i].revents = 0;
		module[i] = NULL;
	}
}

PollScheduler::~PollScheduler() {
}

void PollScheduler::startRun() {
	gettimeofday(&start, NULL);
}

void PollScheduler::endRun() {
	for (int i = 0; i < n_fds; i++)
		delete message[i];
}

void PollScheduler::executionResumed() {
	gettimeofday(&start, NULL);
	start = timeval_substract(start, simTime().dbl());
}

void PollScheduler::registerFD(cModule *module, int fd) {
	this->module[n_fds] = module;
	message[n_fds] = new cMessage("ready to read");
	poll_fd[n_fds++].fd = fd;
}

cMessage *PollScheduler::getNextEvent() {
	cMessage *msg = sim->msgQueue.peekFirst();

	if (msg and msg->getArrivalTime() == simTime())
		return msg;

	timeval now;
	gettimeofday(&now, NULL);
	int timeout = msg ? msg->getArrivalTime().dbl() * 1000 - timeval_msec(now - start) : -1;

	if (msg and timeout < 0)
		throw cRuntimeError(this, "Too slow! Current time is %.3f; next event at %.3f.",
				timeval_msec(now - start) * 1e-3, msg->getArrivalTime().dbl());

	if (n_fds > 0)
		poll(poll_fd, n_fds, timeout);
	else if (msg)
		usleep(timeout * 1000);
	else
		throw cTerminationException(eENDEDOK);

	for (int i = 0; i < n_fds; i++)
		if (poll_fd[i].revents & POLLIN) {
			poll_fd[i].revents = 0;
			gettimeofday(&now, NULL);
			now = now - start;
			message[i]->setArrival(module[i], -1, now.tv_sec + now.tv_usec * 1e-6);
			sim->msgQueue.insert(message[i]);
			msg = sim->msgQueue.peekFirst();
		}

	return msg;
}
