#include <stdio.h>
#include <stdlib.h>

//입력 데이터
typedef struct _inputData {
	int type;
	int process_id;
	int priority; //우선순위
	int computing_time;
}inputData;

//출력 데이터
typedef struct _process {
	int type;
	int process_id;
	int queue_id;
	int priority;
	int computing_time;
	int turnaround_time;
}process;

//큐 노드
typedef struct _qNode {
	process data;
	struct _qNode* next;
	struct _qNode* prev;
}qNode;

//큐
typedef qNode* queue;

#define QUANTUMTIME 20
#define MAXPRIORITY 31

queue createNode(inputData);
queue insertNode(queue, inputData);
queue findTail(queue);
queue findHead(queue);
queue priorityQueueScheduling(queue, queue, int);

int main() {
	inputData input[14] = { 0, 1, 25, 80,
							0, 2, 24, 40,
							0, 3, 8, 30,
							1, 0, 0, 0,
							0, 4, 12, 10,
							0, 5, 22, 30,
							0, 6, 28, 50,
							1, 0, 0, 0,
							0, 7, 5, 20,
							0, 8, 3, 40,
							0, 9, 13, 60,
							1, 0, 0, 0,
							0, 10, 24, 45,
							-1, 0, 0, 0 };
	int size = 14;

	//RealTime Queue
	queue priorityRTQueueHead = NULL;
	queue priorityRTQueueTail = NULL;
	//Priority Queue
	queue priorityQueueHead = NULL;
	queue priorityQueueTail = NULL;
	//Last Priority Queue
	queue priorityLastQueueHead = NULL;
	queue priorityLastQueueTail = NULL;


	int count = 0;
	while (input[count].type != -1) {
		if (input[count].type == 0) {
			if (input[count].priority >= MAXPRIORITY) {
				input[count].priority = MAXPRIORITY;
				priorityLastQueueHead = insertNode(priorityLastQueueHead, input[count]);
				priorityLastQueueTail = findTail(priorityLastQueueHead);
			}
			else {
				priorityQueueHead = insertNode(priorityQueueHead, input[count]);
				priorityQueueTail = findTail(priorityQueueHead);
			}
		}

		else if (input[count].type == 1) {
			
		}
		count++;
	}
	priorityQueueTail = findTail(priorityQueueHead);


	////print
	//queue temp = priorityQueueHead;
	//queue temp2 = priorityQueueTail;
	//printf("head->tail\n");
	//while (temp != NULL) {
	//	printf("%d %d %d %d\n", temp->data.type, temp->data.process_id, temp->data.priority, temp->data.computing_time);
	//	temp = temp->next;
	//}
	//printf("\ntail->head\n");
	//while (temp2 != NULL) {
	//	printf("%d %d %d %d\n", temp2->data.type, temp2->data.process_id, temp2->data.priority, temp2->data.computing_time);
	//	temp2 = temp2->prev;
	//}
	
	return 0;
}

queue createNode(inputData data) {
	queue newNode = (queue)malloc(sizeof(qNode));
	newNode->data.computing_time = data.computing_time;
	newNode->data.priority = data.priority;
	newNode->data.process_id = data.process_id;
	newNode->data.type = data.type;

	newNode->next = NULL;
	newNode->prev = NULL;

	return newNode;
}

queue insertNode(queue head, inputData data) {
	if (head == NULL) {
		queue new = createNode(data);
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
		queue new = createNode(data);
		new->next = head;
		head->prev = new;
		return new;
	}
	else {
		queue new = createNode(data);
		new->next = target->next;
		new->prev = target;
		if (target->next != NULL) {
			target->next->prev = new;
		}
		target->next = new;
		return head;
	}
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

queue priorityQueueScheduling(queue head, queue tail, int quantumTime) {
	if (head != NULL) {
		if (head->data.computing_time > quantumTime) {
			head->data.computing_time -= quantumTime;
		}
		else {
			int remainTime = quantumTime - head->data.computing_time;
			queue temp = head;
			while (remainTime != 0) {
				if (temp->next != NULL) {
					if (temp->next->data.computing_time > remainTime) {
						temp->next->data.computing_time -= remainTime;
						remainTime = 0;
					}
					else {
						remainTime -= temp->next->data.computing_time;
						temp->next->data.computing_time = 0;
						temp = temp->next;
					}
				}
				else {
					remainTime = 0;
				}
			
			}
			
		}
	}
	return head;
}