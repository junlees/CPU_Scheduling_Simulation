#include <stdio.h>
#include <stdlib.h>

#define QUANTUMTIME 20
#define MAXPRIORITY 31
#define INCREASEPRIORITY 10
#define FILENAME "input.txt"


typedef struct _inputData {
	int type;
	int process_id;
	int priority; 
	int computing_time;
}inputData;


typedef struct _process {
	int type;
	int process_id;
	int queue_id;
	int priority;
	int computing_time;
	int turnaround_time;
}process;


typedef struct _qNode {
	process data;
	struct _qNode* next;
	struct _qNode* prev;
}qNode;


typedef qNode* queue;



FILE* fp;

queue createNodeByInput(inputData);
queue createNodeByProcess(queue);
queue insertNode4Priority(queue, inputData);
queue sortNode4Priority(queue);
queue insertNode4ComputingTime(queue, inputData);
queue sortNode4ComputingTime(queue);
queue defineQeuID(queue);
queue findTail(queue);
queue findHead(queue);
queue priorityQueueScheduling(queue, inputData[], int*, int*, int);
queue priorityRTQueueScheduling(queue, inputData[], int*, int*, int);
queue priorityLastQueueScheduling(queue, queue, inputData[], int*, int*, int);
queue priorityQueueToLastQueue(queue, queue);
queue deleteMaxPriority(queue);
queue addToQueue(queue, queue);
//queue getLastNode(queue);
void resultProcessed(queue, int, int);
void queueHeadToTailPrint(queue);
void queueTailToHeadPrint(queue);


int main() {

	//inputData input[14] = { 0, 1, 25, 80,
	//						0, 2, 24, 40,
	//						0, 3, 8, 30,
	//						1, 0, 0, 0,
	//						0, 4, 12, 10,
	//						0, 5, 22, 30,
	//						0, 6, -3, 50,
	//						1, 0, 0, 0,
	//						0, 7, -5, 20,
	//						0, 8, 3, 40,
	//						0, 9, 13, 60,
	//						1, 0, 0, 0,
	//						0, 10, 24, 45,
	//						-1, 0, 0, 0 };
	//int size = 14;
	
	int n = 0;
	int data = 0;
	fp = fopen(FILENAME, "r");
	if (fp == NULL) {
		printf("file open fail\n");
		exit(0);
	}

	while (fscanf(fp, "%d", &data) != EOF) {
		//printf("%d ", data);
		n++;
	}
	fclose(fp);
	int size = n / 4 + 1;

	fp = fopen(FILENAME, "r");
	inputData* input = (inputData*)malloc(sizeof(inputData) * (size));

	for (int i = 0; i < size; i++) {
		fscanf(fp, "%d", &input[i].type);
		if (input[i].type == -1) {
			break;
		}
		fscanf(fp, "%d", &input[i].process_id);
		fscanf(fp, "%d", &input[i].priority);
		fscanf(fp, "%d", &input[i].computing_time);
	}
	fclose(fp);
	int quantomTime = 0;
	printf("Input Quantom Time : ");
	scanf("%d", &quantomTime);
	printf("Quantom Time : %d\n", quantomTime);


	//RealTime Queue
	queue priorityRTQueueHead = NULL;
	queue priorityRTQueueTail = NULL;
	//Priority Queue
	queue priorityQueueHead = NULL;
	queue priorityQueueTail = NULL;
	//Last Priority Queue
	queue priorityLastQueueHead = NULL;
	queue priorityLastQueueTail = NULL;

	//TurnAround Time
	int taTime = 0;
	

	int count = 0;
	while (input[count].type != -1) {

		//type : 0 -> input data
		if (input[count].type == 0) {
			if (input[count].priority < 0) {
				priorityRTQueueHead = insertNode4Priority(priorityRTQueueHead, input[count]);
				priorityRTQueueTail = findTail(priorityRTQueueHead);
			}
			else if (input[count].priority >= MAXPRIORITY) {
				input[count].priority = MAXPRIORITY;
				priorityLastQueueHead = insertNode4ComputingTime(priorityLastQueueHead, input[count]);
				priorityLastQueueTail = findTail(priorityLastQueueHead);
			}
			else {
				priorityQueueHead = insertNode4Priority(priorityQueueHead, input[count]);
				priorityQueueTail = findTail(priorityQueueHead);
			}
		}

		//type : 1 -> scheduling
		else if (input[count].type == 1) {
			//priorityQueueHead = findHead(priorityQueueHead);
			//print head only
			//printf("Head : %d\n", priorityQueueHead->data.process_id);

			int qTime = quantomTime;

			while (qTime != 0) {
				if (priorityRTQueueHead != NULL) {
					priorityRTQueueHead = priorityRTQueueScheduling(priorityRTQueueHead, input, &qTime, &taTime, size);
					priorityLastQueueHead = priorityQueueToLastQueue(priorityQueueHead, priorityLastQueueHead);
					priorityQueueHead = deleteMaxPriority(priorityQueueHead);
				}
				else if (priorityQueueHead != NULL) {
					priorityQueueHead = priorityQueueScheduling(priorityQueueHead, input, &qTime, &taTime, size);

				}
				else if (priorityLastQueueHead != NULL) {
					priorityLastQueueHead = priorityLastQueueScheduling(priorityLastQueueHead, priorityLastQueueTail, input, &qTime, &taTime, size);
				}

				//printf("taTime : %d\n", taTime);
				
			}
			
		}
		count++;
	}
	
	//debug
	//printf("RTQueue\n");
	//queueHeadToTailPrint(priorityRTQueueHead);
	//printf("Queue\n");
	//queueHeadToTailPrint(priorityQueueHead);
	//printf("LQueue\n");
	//queueHeadToTailPrint(priorityLastQueueHead);
	//printf("Tail To Head Queue\n");
	//queueTailToHeadPrint(priorityQueueTail);

	//printf("taTime : %d\n", taTime);

	
	//scheduling
	int qTime = quantomTime;
	while (1) {
		if (priorityRTQueueHead != NULL) {
			priorityRTQueueHead = priorityRTQueueScheduling(priorityRTQueueHead, input, &qTime, &taTime, size);
			//printf("taTime : %d\n", taTime);
		}
		else if (priorityQueueHead != NULL) {
			priorityQueueHead = priorityQueueScheduling(priorityQueueHead, input, &qTime, &taTime, size);
			priorityLastQueueHead = priorityQueueToLastQueue(priorityQueueHead, priorityLastQueueHead);
			priorityQueueHead = deleteMaxPriority(priorityQueueHead);
			//printf("taTime : %d\n", taTime);
		}
		else if (priorityLastQueueHead != NULL) {
			priorityLastQueueTail = findTail(priorityLastQueueHead);
			priorityLastQueueHead = priorityLastQueueScheduling(priorityLastQueueHead, priorityLastQueueTail, input, &qTime, &taTime, size);
			//printf("taTime : %d\n", taTime);
		}
		//printf("LQueue\n");
		//queueHeadToTailPrint(priorityLastQueueHead);
		if (qTime == 0) {
			qTime = quantomTime;
		}
		if (priorityRTQueueHead == NULL && priorityQueueHead == NULL && priorityLastQueueHead == NULL) {
			break;
		}
	}

	
	
	return 0;
}

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
		new = defineQeuID(new);
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
			if(head->next != NULL) {
				queue temp = head->next;
				tail->next = head;
				head->prev = tail;
				head->next = NULL;
				tail = head;
				head = temp;
			}
	

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

//queue getLastNode(queue head) {
//	if (head == NULL) {
//		return NULL;
//	}
//	while (head->next != NULL) {
//		head = head->next;
//	}
//	return head;
//}

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
	printf(" process_id: %d, queue_id: %s, priority: %d, computing_time: %d, turnaround_time: %d\n\n",
			head->data.process_id, queID, head->data.priority, computing_time, taTime);
		
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

