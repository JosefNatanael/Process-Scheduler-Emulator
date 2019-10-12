/*
 * Complete the Process definitions marked by TODO
 * print() has already been defined for your debugging convenience.
 *
 * You may add your own helper classes, structs, and functions here.
 */

#include "Process.h"

 // TODO, You may also use initializer list syntax " : ".
 // Constructor for Process, initializes pid, execute_time, and priority
Process::Process(const unsigned int pid, unsigned int execute_time, unsigned int priority)
	: pid(pid)
	, execute_time(execute_time)
	, priority(priority)
	, aging_counter(0)
{}

const unsigned int Process::get_pid() const {
	return pid; // TODO
}

unsigned int Process::get_execute_time() const {
	return execute_time; // TODO
}

unsigned int Process::get_priority() const {
	return priority; // TODO
}

unsigned int Process::get_aging_counter() const {
	return aging_counter; // TODO
}

// Prints a summary of pid, execute_time, priority, aging_counter
void Process::print() const {
	cout << "Process ID: " << pid << "\n"
		<< "Execution Time Remaining: " << execute_time << "\n"
		<< "Priority: " << priority << "\n"
		<< "Aging Counter: " << aging_counter << endl;
}

// Simulate execution of process
void Process::execute(unsigned int time) {
	// TODO
	execute_time < time ? execute_time -= time : execute_time = 0;
	aging_counter = 0;
}

// Simulate waiting of process in the queue
void Process::wait(unsigned int time) {
	// TODO
	aging_counter += time;
}

void Process::promote_priority() {
	// TODO
	++priority;
}
