#include "func.h"

queue createNodeByInput(inputData data) {
	queue newNode = (queue)malloc(sizeof(qNode));
	newNode->data.computing_time = data.computing_time;
	newNode->data.priority = data.priority;
	newNode->data.process_id = data.process_id;
	newNode->data.type = data.type;
	newNode->data.turnaround_time = 0;
	newNode->data.queue_id = NULL;

	newNode->next = NULL;
	newNode->prev = NULL;

	return newNode;
}

queue createNodeByProcess(queue data) {
	queue newNode = (queue)malloc(sizeof(qNode));
	newNode->data.computing_time = data->data.computing_time;
	newNode->data.priority = data->data.priority;
	newNode->data.process_id = data->data.process_id;
	newNode->data.type = data->data.type;
	newNode->data.turnaround_time = data->data.turnaround_time;
	newNode->data.queue_id = data->data.queue_id;

	newNode->next = NULL;
	newNode->prev = NULL;

	return newNode;
}
queue insertNode4Priority(queue head, inputData data) {
	if (head == NULL) {
		queue new = createNodeByInput(data);
		new = defineQeuID(new);
		return new;
	}
	queue target = NULL;
	queue temp = head;

	while (temp != NULL) {
		if (temp->data.priority <= data.priority) {
			target = temp;
		}
		temp = temp->next;
	}
	if (target == NULL) {
		queue new = createNodeByInput(data);
		new = defineQeuID(new);
		
		new->next = head;
		head->prev = new;
		return new;
	}
	else {
		queue new = createNodeByInput(data);
		new = defineQeuID(new);

		if (target->next == NULL) {
			new->next = NULL;
		}
		else {
			new->next = target->next;
		}
		new->prev = target;
		if (target->next != NULL) {
			target->next->prev = new;
		}
		target->next = new;
		return head;
	}
}

queue sortNode4Priority(queue head) {
	queue target = NULL;
	queue temp = head;

	head = defineQeuID(head);

	while (temp != NULL) {
		if (temp->data.priority <= head->data.priority) {
			target = temp;
		}
		temp = temp->next;
	}
	if (target == NULL) {
		return head;
	}
	else {
		queue new = createNodeByProcess(head);
		if (target->next == NULL) {
			new->next = NULL;
		}
		else {
			new->next = target->next;
		}
		new->prev = target;
		if (target->next != NULL) {
			target->next->prev = new;
		}
		
		target->next = new;
		temp = head;
		head = head->next;
		head->prev = NULL;
		free(temp);
		return head;
	}
}

queue insertNode4ComputingTime(queue head, inputData data) {
	if (head == NULL) {
		queue new = createNodeByInput(data);
		return new;
	}
	queue target = NULL;
	queue temp = head;
	while (temp != NULL) {
		if (temp->data.computing_time <= data.computing_time) {
			target = temp;
		}
		temp = temp->next;
	}
	if (target == NULL) {
		queue new = createNodeByInput(data);
		new->next = head;
		head->prev = new;
		return new;
	}
	else {
		queue new = createNodeByInput(data);
		if (target->next == NULL) {
			new->next = NULL;
		}
		else {
			new->next = target->next;
		}
		new->prev = target;
		if (target->next != NULL) {
			target->next->prev = new;
		}
		target->next = new;
		return head;
	}
}

queue sortNode4ComputingTime(queue head) {
	queue target = NULL;
	queue temp = head;
	while (temp != NULL) {
		if (temp->data.computing_time <= head->data.computing_time) {
			target = temp;
		}
		temp = temp->next;
	}
	if (target == NULL) {
		return head;
	}
	else {
		queue new = createNodeByProcess(head);
		if (target->next == NULL) {
			new->next = NULL;
		}
		else {
			new->next = target->next;
		}
		new->prev = target;
		if (target->next != NULL) {
			target->next->prev = new;
		}
		target->next = new;
		temp = head;
		head = head->next;
		head->prev = NULL;
		free(temp);
		return head;
	}
}
queue defineQeuID(queue target) {
	if (target->data.priority < 0) {
		target->data.queue_id = 0;
	}
	else if (target->data.priority == MAXPRIORITY) {
		target->data.queue_id = 2;
	}
	else {
		target->data.queue_id = 1;
	}
	return target;
}
queue findTail(queue head) {
	queue temp = head;
	while (temp->next != NULL) {
		temp = temp->next;
	}
	return temp;
}

queue findHead(queue tail) {
	queue temp = tail;
	while (temp->prev != NULL) {
		temp = temp->prev;
	}
	return temp;
}

queue priorityQueueScheduling(queue head, inputData orgDat[], int* quantumTime, int* taTime, int size) {
	if (head != NULL) {
		if (head->data.computing_time > *quantumTime) {
			head->data.computing_time -= *quantumTime;
			if (head->data.priority < (MAXPRIORITY-INCREASEPRIORITY)) {
				head->data.priority += INCREASEPRIORITY;
			}
			else {
				head->data.priority = MAXPRIORITY;
				head = defineQeuID(head);
			}
			*taTime += *quantumTime;
			*quantumTime = 0;
			head = sortNode4Priority(head);
			//printf("head->data.process_id: %d, priority: %d\n", head->data.process_id, head->data.priority);
			return head;
		}
		else {
			//queue temp = head;
			*taTime += head->data.computing_time;
			*quantumTime -= head->data.computing_time;
			head->data.turnaround_time += head->data.computing_time;

			//printf("head->data.process_id: %d\n", head->data.process_id);
			for (int i = 0; i < size; i++) {
				//printf("orgDat[%d].process_id: %d\n", i, orgDat[i].process_id);
				if (orgDat[i].process_id == head->data.process_id) {
					printf("(Queue) ");
					resultProcessed(head, orgDat[i].computing_time, *taTime);
				}
			}
			
			queue temp = head;
			if (head->next == NULL) {
				head = NULL;
				return head;
			}
			head = head->next;
			head->prev = NULL;
			free(temp);
			return head;
		}
	}
	else {
		return NULL;
	}
}

queue priorityRTQueueScheduling(queue head, inputData input[], int* quantumTime, int* taTime, int size) {
	if (head != NULL) {
		if (head->data.computing_time > *quantumTime) {
			head->data.computing_time -= *quantumTime;

			*taTime += *quantumTime;
			*quantumTime = 0;
			//printf("head->data.process_id: %d, priority: %d\n", head->data.process_id, head->data.priority);
			return head;
		}
		else {
			//queue temp = head;
			*taTime += head->data.computing_time;
			*quantumTime -= head->data.computing_time;
			head->data.turnaround_time += head->data.computing_time;
			//printf("head->data.process_id: %d\n", head->data.process_id);
			for (int i = 0; i < size; i++) {
				//printf("orgDat[%d].process_id: %d\n", i, orgDat[i].process_id);
				if (input[i].process_id == head->data.process_id) {
					printf("(RTQueue) ");
					resultProcessed(head, input[i].computing_time, *taTime);
					break;
				}
			}
			queue temp = head;
			if (head->next == NULL) {
				head = NULL;
				return head;
			}
			head = head->next;
			head->prev = NULL;
			free(temp);
			return head;
		}
	}
	else {

		return NULL;
	}
}

queue priorityLastQueueScheduling(queue head, queue tail, inputData input[], int* quantumTime, int* taTime, int size) {
	if (head != NULL) {
		if (head->data.computing_time > *quantumTime) {
			head->data.computing_time -= *quantumTime;
			
			*taTime += *quantumTime;
			*quantumTime = 0;
			//head = sortNode4ComputingTime(head);
			//printf("head->data.process_id: %d, priority: %d, computingTime: %d\n", head->data.process_id, head->data.priority, head->data.computing_time);
			queue temp = head->next;
			tail->next = head;
			head->prev = tail;
			head->next = NULL;
			tail = head;
			head = temp;

			return head;
		}
		else {
			//queue temp = head;
			*taTime += head->data.computing_time;
			*quantumTime -= head->data.computing_time;
			head->data.turnaround_time += head->data.computing_time;
			//printf("head->data.process_id: %d\n", head->data.process_id);
			for (int i = 0; i < size; i++) {
				//printf("orgDat[%d].process_id: %d\n", i, orgDat[i].process_id);
				if (input[i].process_id == head->data.process_id) {
					printf("(LQueue) ");
					resultProcessed(head, input[i].computing_time, *taTime);
				}
			}
			queue temp = head;
			if (head->next == NULL) {
				head = NULL;
				return head;
			}
			head = head->next;
			head->prev = NULL;
			free(temp);
			return head;
		}
	}
	else {
		return NULL;
	}
}

queue priorityQueueToLastQueue(queue qHead, queue lqHead) {
	queue temp = qHead;
	while (temp != NULL) {
		if (temp->data.queue_id == 2) {
			queue newNode = createNodeByProcess(temp);
			lqHead = addToQueue(lqHead, newNode);
		}
		temp = temp->next;
	}
	return lqHead;
}

queue addToQueue(queue head, queue newNode) {
	if (head == NULL) {
		head = newNode;
	}
	else {
		//queue lastNode = getLastNode(head);
		//lastNode->next = newNode;
		//newNode->prev = lastNode;
		newNode->next = head;
		head->prev = newNode;
		head = newNode;
		head = sortNode4ComputingTime(head);
	}
	return head;
}

queue getLastNode(queue head) {
	if (head == NULL) {
		return NULL;
	}
	while (head->next != NULL) {
		head = head->next;
	}
	return head;
}

queue deleteMaxPriority(queue head) {
	queue current = head;
	queue prev = NULL;
	while (current != NULL) {
		if (current->data.queue_id == 2) {
			if (prev == NULL) {  
				head = current->next;
				free(current);
				current = head;
			}
			else {
				prev->next = current->next;
				if (current->next != NULL) {
					current->next->prev = prev;
				}
				free(current);
				current = prev->next;
			}
		}
		else {
			prev = current;
			current = current->next;
		}
	}
	return head;
}
//queue findMaxPriority(queue head) {
//	queue temp = head;
//	queue target = NULL;
//	while (temp != NULL) {
//		if (temp->data.priority <= head->data.priority) {
//			target = temp;
//		}
//		temp = temp->next;
//	}
//	return target;
//}

void resultProcessed(queue head, int computing_time, int taTime) {
	if (head == NULL) {
		printf("No process\n");
		return;
	}
	char* queID = NULL;
	switch (head->data.queue_id) {
		case 0:
			queID = "RTQ";
			break;
		case 1:
			queID = "Q";
			break;
		case 2:
			queID = "LQ";
			break;
		default:
			break;
	}
	printf("type: %d, process_id: %d, queue_id: %s, priority: %d, computing_time: %d, turnaround_time: %d\n\n",
			head->data.type, head->data.process_id, queID, head->data.priority, computing_time, taTime);
		
}

void queueHeadToTailPrint(queue head) {
	queue temp = head;
	while (temp != NULL) {
		printf("type: %d, process_id: %d, queue_id: %d, priority: %d, computing_time: %d, turnaround_time: %d\n",
						temp->data.type, temp->data.process_id, temp->data.queue_id, temp->data.priority, temp->data.computing_time, temp->data.turnaround_time);
		temp = temp->next;
	}
}

void queueTailToHeadPrint(queue tail) {
	queue temp = tail;
	while (temp != NULL) {
		printf("type: %d, process_id: %d, queue_id: %d, priority: %d, computing_time: %d, turnaround_time: %d\n",
									temp->data.type, temp->data.process_id, temp->data.queue_id, temp->data.priority, temp->data.computing_time, temp->data.turnaround_time);
		temp = temp->prev;
	}
}

