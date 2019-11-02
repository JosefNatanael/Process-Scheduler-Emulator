/*
 * Complete the ProcessScheduler definitions marked by TODO
 * print() has already been defined for your debugging convenience.
 *
 * You may add your own helper classes, structs, and functions here.
 */

#include "ProcessScheduler.h"

ProcessScheduler::ProcessScheduler(const unsigned int quantum_threshold, const unsigned int max_priority, const unsigned int aging_threshold) 
	: quantum_threshold(quantum_threshold)
	, max_priority(max_priority)
	, aging_threshold(aging_threshold)
{
	// TODO
	priority_queues = new ProcessQueue[max_priority + 1];
}

ProcessScheduler::~ProcessScheduler() {
	// TODO
	delete[] priority_queues;
	priority_queues = nullptr;
	if (current_process != nullptr) {
		delete current_process;
		current_process = nullptr;
	}
}

void ProcessScheduler::print() const {
	cout << "################################################################################" << endl;

	cout << "Scheduler Configuration: \n"
			<< "* Quantum Threshold: " << quantum_threshold << "\n"
			<< "* Max Priority: " << max_priority << "\n"
			<< "* Aging Threshold: " << aging_threshold << "\n"
			<< endl;

	cout << "State: \n"
			<< "* Quantum Counter: " << quantum_counter << "\n"
			<< "* Next Process ID: " << next_pid << "\n"
			<< endl;

	cout << "Current Process: ";
	if (has_current_process()) {
		cout << "\n----------------------------------------\n";
		current_process->print();
		cout << "----------------------------------------" << endl;
	}
	else {
		cout << "N/A" << endl;
	}

	for (unsigned int i{max_priority + 1}; i > 0; --i) {
		cout << endl;
		cout << "Priority Queue " << i - 1 << ": \n";
		priority_queues[i - 1].print();

	}
	cout << "################################################################################" << endl;
}

bool ProcessScheduler::has_current_process() const {
	// TODO
	return current_process != nullptr;
}

// Swap out Current Process and reset the quantum_counter, if no Current Process or the newly added Process has higher priority.
void ProcessScheduler::add_process(unsigned int execute_time, unsigned int priority) {
	// TODO
	Process* newProcess = new Process(next_pid, execute_time, priority);
	++next_pid;
	// If our current process is not empty and our current process has equal or more priority to the new process
	// then the new process goes into the queue instead of taking over the current process
	if (has_current_process()) {
		if (current_process->get_priority() >= priority) {
			priority_queues[priority].enqueue(newProcess);
		}
		else {
			// We pack our current process back to the queue, and let the new process take over as the current process, resetting the quantum counter
			priority_queues[current_process->get_priority()].enqueue(current_process);
			current_process = newProcess;
			quantum_counter = 0;
		}
	}
	// If we currently do not have a process, we swap out the current_process and reset the quantum counter
	else {
		current_process = newProcess;
		quantum_counter = 0;
	}
}

/*
 * Simulate 1ms at a time.
 * First execute Current Process and tick-up Quantum Counter (if Quantum Threshold is zero, ignore Quantum Counter steps),
 * Delete Current Process and reset Quantum Counter if it has finished execution,
 * Then perform Aging (if Aging Threshold is zero, ignore Aging steps),
 * Swap to next highest priority Process if Current Process has finished execution,
 * Otherwise, swap out Current Process and reset Quantum Counter, if have a higher priority Process or reached/exceeded Quantum Threshold.
 * Repeat for every ms.
 * Do nothing if no Process to execute.
 */
void ProcessScheduler::simulate(unsigned int time) {
	// TODO
	// i represents the time elapsed
	unsigned int i = 0;
	while (i < time) {
		// If current process is empty
		if (!has_current_process()) {
			for (int j = max_priority; j >= 0; --j) {
				if (!priority_queues[j].is_empty()) {
					current_process = priority_queues[j].dequeue();
					break;
				}
			}
			// current process is not set yet, then return
			if (!has_current_process()) {
				return;
			}
		}
		// Check whether it has reached the quantum threshold
		if (quantum_counter < quantum_threshold) {
			current_process->execute(1);
			++quantum_counter;
		}
		// If the execute time is zero, delete
		if (current_process->get_execute_time() == 0) {
			delete current_process;
			current_process = nullptr;
			quantum_counter = 0;
		}
		// Perform aging
		for (int j = max_priority - 1; j >= 0; --j) {
			priority_queues[j+1].merge_back(priority_queues[j].perform_aging(1, aging_threshold));
		}
		// Re-find next process if execution of the current process is done (no more process)
		if (!has_current_process()) {
			for (int j = max_priority; j >= 0; --j) {
				if (!priority_queues[j].is_empty()) {
					current_process = priority_queues[j].dequeue();
					break;
				}
			}
			// current process is not set yet, then return
			if (!has_current_process()) {
				return;
			}
		}
		else {	// it has current process
			// Only need to check higher priority queues, if found, return it to the end of it's queue
			if (quantum_threshold == 0 || quantum_counter < quantum_threshold) {
				Process* newProcess = nullptr;
				for (unsigned int j = max_priority; j > current_process->get_priority(); --j) {
					if (!priority_queues[j].is_empty()) {
						newProcess = priority_queues[j].dequeue();
						break;
					}
				}
				// That means it is found, a higher priority than the current_process, then it needs to be returned back to the queue
				if (newProcess != nullptr) {
					priority_queues[current_process->get_priority()].enqueue(current_process);
					current_process = newProcess;
				}
			}
			// Gonna be kicked back to it's queue, find next candidate
			else if (quantum_counter == quantum_threshold) {
				quantum_counter = 0;
				priority_queues[current_process->get_priority()].enqueue(current_process);
				for (int j = max_priority; j >= 0; --j) {
					if (!priority_queues[j].is_empty()) {
						current_process = priority_queues[j].dequeue();
						break;
					}
				}
			}
			
		}

		++i;
	}
}
