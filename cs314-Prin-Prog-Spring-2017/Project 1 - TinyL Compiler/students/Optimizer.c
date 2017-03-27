/*
 *********************************************
 *  314 Principles of Programming Languages  *
 *  Spring 2017                              *
 *  Author: Ulrich Kremer                    *
 *  Student Version                          *
 *********************************************
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "InstrUtils.h"
#include "Utils.h"

void search_reg(Instruction *ptr, int reg) {

	// printf("Register you are looking for is %d\n", reg);

	while (ptr != NULL) {
		switch (ptr->opcode) {
			case ADD:
			case SUB:
			case MUL:
			case DIV:
			case LOADAI:
				if (ptr->field3 == reg) {
					ptr->critical = 't';
					return;
				}
				break;
			case LOADI:
				if (ptr->field2 == reg) {
					ptr->critical = 't';
					return;
				}
			default:
				break;
		}
		ptr = ptr->prev;
	}

}

void search_offset(Instruction *ptr, int offset) {

	// printf("Offset you are looking for is: %d\n", offset);

	while (ptr != NULL) {
		switch (ptr->opcode) {
			case STOREAI:
				if (ptr->field3 == offset) {
					ptr->critical = 't';
					return;
				}
				break;
			default:
				break;
		}
		ptr = ptr->prev;
	}
}

int main()
{
	Instruction *head;
	head = ReadInstructionList(stdin);

	if (!head) {
		WARNING("No instructions\n");
		exit(EXIT_FAILURE);
	}

	Instruction *ptr;
	Instruction *tmp;

	head->critical = 't';
	ptr = head;

	while (ptr != NULL) {
		if (ptr->opcode == OUTPUTAI) {
			ptr->critical = 't';
		}
		if (ptr->next == NULL) {
			tmp = ptr;
		}
		ptr = ptr->next;
	}

	ptr = tmp;

	while (ptr != NULL) {

		if (ptr->critical == 't') {

			switch (ptr->opcode) {
				case OUTPUTAI:
				case LOADAI:
					tmp = ptr;
					search_offset(tmp, ptr->field2);
					break;
				case STOREAI:
					tmp = ptr;
					search_reg(tmp, ptr->field1);
					break;
				case ADD:
				case SUB:
				case MUL:
				case DIV:
					tmp = ptr;
					search_reg(tmp, ptr->field1);
					tmp = ptr;
					search_reg(tmp, ptr->field2);
					break;
				default:
					break;
			}
		}
		ptr = ptr->prev;
	}

	if (head)
		PrintInstructionList(stdout, head);

	ptr = head;

	while (ptr != NULL) {
		if (ptr->prev != NULL) {
			free(ptr->prev);
		}
		if (ptr->next == NULL) {
			free(ptr);
			break;
		}
		ptr = ptr->next;
	}

	return EXIT_SUCCESS;
}
