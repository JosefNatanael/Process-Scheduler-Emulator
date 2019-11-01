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

ProcessQueue* ProcessQueue::perform_aging(unsigned int time, const unsigned int aging_threshold) {
	return nullptr; // TODO
}

void ProcessQueue::merge_back(ProcessQueue* process_queue) {
	// TODO
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
