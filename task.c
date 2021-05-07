#include "task.h"
#include <stdio.h>
#include "resource.h"
#include <ucontext.h>
//int state[TASKS_COUNT];
int readyQ_num = 0;
int ready_queue[TASKS_COUNT];
int ready_queue[TASKS_COUNT];
int state[TASKS_COUNT];
int priority[TASKS_COUNT];
int is_finish[TASKS_COUNT];
int nowtask;
//char str[TASKS_COUNT][8192];
int resource_owner[RESOURCES_COUNT];
int had_run[TASKS_COUNT];
int find_best_priority();
void initial_ucontext(int i);
void initial_priority();
struct ucontext ctx[TASKS_COUNT],ctx_main;
int is_auto;
status_type activate_task(task_type id)
{
    if(state[id] != 0)
    {
        status_type result = STATUS_ERROR;
        return result;
    }
    else if(is_auto == 1)
    {

        state[id] = is_ready(id);
        push_in_readyQ(id);

        status_type result = STATUS_OK;
        return result;
    }
    else
    {

        state[id] = is_ready(id);

        push_in_readyQ(id);
        if(priority[id] > priority[nowtask])
        {
            state[nowtask] = is_ready(nowtask);//put origin task back to ready queue
            swapcontext(&ctx[nowtask],&ctx_main);
        }
        status_type result = STATUS_OK;
        return result;

    }
}

status_type terminate_task(void)
{
    status_type result;
    int is_own_resource = 0;
    for(int i=0; i<RESOURCES_COUNT; i++)
    {
        if(resource_owner[i] == nowtask)
            is_own_resource = 1;
    }
    if(is_own_resource == 1)
    {
        result = STATUS_ERROR;
        return result;
    }
    else
    {
        state[nowtask] = is_suspended(nowtask);


        popout_readyQ(nowtask);
        is_finish[nowtask] = 1;

        swapcontext(&ctx[nowtask],&ctx_main);
        result = STATUS_OK;
        return result;
    }
}

void initial_priority()
{
    for(int i=0; i<TASKS_COUNT; i++)
    {
        priority[i] = task_const[i].static_priority;
    }
}
void push_in_readyQ(int id)
{
    ready_queue[readyQ_num] = id;
    readyQ_num++;
}
task_state_type is_suspended(task_type id)
{
    task_state_type result = SUSPENDED;

    return result;
}
task_state_type is_ready(task_type id)
{
    task_state_type result = READY;

    return result;
}
task_state_type is_run(task_type id)
{
    task_state_type result = RUNNING;
    had_run[id] = 1;

    return result;
}
void popout_readyQ(task_type id)
{
    int changeID;

    for(int i=0; i<readyQ_num; i++)
    {
        if(ready_queue[i] == id)
        {
            changeID = i;
            break;
        }
    }

    for(int i=changeID; i<readyQ_num; i++)
    {
        ready_queue[i] = ready_queue[i+1];
    }
    readyQ_num--;

}

