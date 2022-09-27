#ifndef LABS_PA_22_FIRST_TASK_H
#define LABS_PA_22_FIRST_TASK_H

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "matrix/matrix.h"
#include "constant/fork_pid.h"
#include "constant/filenames.h"

void process(const std::function<void()>& childFunction);

void printPidWithText(const std::string& text);

void inputMatrices();

void sumMatrices();

void printMatrices();

void* attachToSharedMemory(int shared_memory_id);

int createOrGetSharedMemory();

#endif //LABS_PA_22_FIRST_TASK_H
