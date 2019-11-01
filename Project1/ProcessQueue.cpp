/*
 * Complete the ProcessQueue definitions marked by TODO
 * Default Constructor has already been defined, as failing to initialize sentinel to point to itself is a common yet frustrating pitfall.
 * print() has already been defined for your debugging convenience.
 *
 * You may add your own helper classes, structs, and functions here.
 */

#include "ProcessQueue.h"

 /*
  * These will fail as sentinel doesn't exist yet to point to itself.
  * Initializer List : sentinel{new ProcessNode{nullptr, sentinel, sentinel}}
  * sentinel = new ProcessNode{nullptr, sentinel, sentinel};
  */
ProcessQueue::ProcessQueue() {
	sentinel = new ProcessNode{};
	sentinel->prev = sentinel->next = sentinel; // Need to do this on a separate line, otherwise sentinel doesn't exist yet to point to itself.
}

// Deallocate all the nodes, including the sentinel, and the processes in it.
ProcessQueue::~ProcessQueue() {
	// TODO
	if (sentinel == nullptr) {
		return;
	}
	if (sentinel->next == sentinel) {
		delete sentinel;
		sentinel = nullptr;
		return;
	}
	while (sentinel->next != sentinel) {
		ProcessNode* afterSentinelNext = sentinel->next->next;
		delete sentinel->next->process;
		sentinel->next->process = nullptr;
		delete sentinel->next;

		// Hook up the sentinel with the node after the removed node.
		sentinel->next = afterSentinelNext;
		afterSentinelNext->prev = sentinel;
	}
	delete sentinel;
	sentinel = nullptr;
}

void ProcessQueue::print() const {
	cout << "================================================================================" << endl;
	if (is_empty()) {
		cout << "This ProcessQueue is empty." << endl;
	}
	else {
		for (ProcessNode* current_node{ sentinel->next }; current_node != sentinel->prev; current_node = current_node->next) {
			current_node->process->print();
			cout << "--------------------------------------------------------------------------------" << endl;
		}
		sentinel->prev->process->print();
	}
	cout << "================================================================================" << endl;
}

// Performs Aging on all Processes in ProcessQueue. Extracts and returns a ProcessQueue of Processes with incremented priority (can be empty).
// Assumptions: can perform aging, i.e. not in max priority queue, and it's higher process/priority queue can perform aging as well.
ProcessQueue* ProcessQueue::perform_aging(unsigned int time, const unsigned int aging_threshold) {
	// TODO
	ProcessQueue* higherPriorityQueue = new ProcessQueue{};
	if (aging_threshold == 0) {
		return higherPriorityQueue;
	}
	ProcessNode* temp = sentinel->next;
	while (temp != sentinel) {
		if (temp->process->get_aging_counter() + time >= aging_threshold) {
			unsigned int remainingTime = aging_threshold - time - temp->process->get_aging_counter();
			temp->process->reset_aging_counter();
			temp->process->wait(remainingTime);
			temp->process->promote_priority();
			higherPriorityQueue->enqueue(remove(temp));
		}
		else {
			temp->process->wait(time);
		}
		temp = temp->next;
	}
	return higherPriorityQueue;
}

// Simply merge, nothing special added
void ProcessQueue::merge_back(ProcessQueue* process_queue) {
	// TODO
	// If nothing to merge, no need to do anything
	if (process_queue->sentinel->next == process_queue->sentinel) {
		return;
	}

	// Bridge last element to the start of process_queue's first element
	sentinel->prev->next = process_queue->sentinel->next;
	process_queue->sentinel->next->prev = sentinel->prev;

	// Bridge sentinel and process_queue's last element
	process_queue->sentinel->prev->next = sentinel;
	sentinel->prev = process_queue->sentinel->prev;

	// Cut process_queue's ties
	process_queue->sentinel->next = process_queue->sentinel;
	process_queue->sentinel->prev = process_queue->sentinel;

	// Call destructor of process_queue
	delete process_queue;
}

// Checks if the process is a nullptr, do nothing.
void ProcessQueue::enqueue(Process* process) {
	// TODO
	if (process == nullptr) {
		return;
	}
	ProcessNode* newNode = new ProcessNode(process, sentinel, sentinel->prev);
	sentinel->prev->next = newNode;
	sentinel->prev = newNode;
}

/* 
  If ProcessQueue is empty, return nullptr
  Otherwise return the Process in ProcessNode, deallocating the ProcessNode.
*/
Process* ProcessQueue::dequeue() {
	// TODO
	if (sentinel->next == sentinel) {
		return nullptr;
	}
	Process* toReturn = sentinel->next->process;
	ProcessNode* afterSentinelNext = sentinel->next->next;
	delete sentinel->next;

	// Hook up the sentinel with the node after the removed node.
	sentinel->next = afterSentinelNext;
	afterSentinelNext->prev = sentinel;

	return toReturn;
}

bool ProcessQueue::is_empty() const {
	// TODO
	return (sentinel->next == sentinel);
}

/*
  Be very careful when deleting the ProcessNode, don't accidentally delete the Process that we actually want to extract and return.
  Do nothing and return nullptr if process_node is empty or queue is empty
*/
Process* ProcessQueue::remove(ProcessNode* process_node) {
	// TODO
	// If invalid process_node or the queue is empty return nullptr
	if (process_node == nullptr || sentinel->next == sentinel) {
		return nullptr;
	}
	ProcessNode* temp = sentinel->next;
	while (temp != sentinel) {
		if (temp == process_node) {
			break;
		}
		temp = temp->next;
	}
	// If not found, return nullptr
	if (temp == sentinel) {
		return nullptr;
	}
	Process* toReturn = temp->process;
	ProcessNode* nextTemp = temp->next;
	ProcessNode* prevTemp = temp->prev;
	delete temp;
	temp = nullptr;

	// Hook up the nodes next to the removed node.
	nextTemp->prev = prevTemp;
	prevTemp->next = nextTemp;

	return toReturn;
}
