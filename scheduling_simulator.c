#include "resource.h"
#include "task.h"
#include <stdio.h>
#include <ucontext.h>
#include <stdlib.h>

int find_best_priority();
extern int had_run[TASKS_COUNT];
extern int ready_queue[TASKS_COUNT];
extern int resource_owner[RESOURCES_COUNT];
extern int nowtask;
extern int is_auto = 1;
extern int readyQ_num;
extern is_finish[TASKS_COUNT];
void initial_context(int i);
void pushback_readyQ();
char str[TASKS_COUNT][8192];
struct ucontext ctx[TASKS_COUNT],ctx_main;

extern int state[TASKS_COUNT];
extern int priority[TASKS_COUNT];

int main()
{

    for(int i=0; i<TASKS_COUNT; i++)
        initial_context(i);
    for(int i=0; i<TASKS_COUNT; i++)
    {
        had_run[i] = 0;
        state[i] = 0;
        priority[i] = 0;
        is_finish[i] = 0;
    }
    for(int i=0; i<RESOURCES_COUNT; i++)
        resource_owner[i] = 0;
    for(int i=0; i<TASKS_COUNT; i++)
    {

        state[i] = is_suspended(task_const[i].id);
    }
    for(int i=0; i<TASKS_COUNT; i++)
        priority[i] = task_const[i].static_priority;
    for(int i=0; i<AUTO_START_TASKS_COUNT; i++)
    {
        activate_task(i);
    }
    is_auto = 0;

    int i = 0;
    while(1)
    {

        nowtask = find_best_priority();
        state[nowtask] = is_run(nowtask);

        getcontext(&ctx_main);
        swapcontext(&ctx_main,&ctx[nowtask]);
        if(is_finish[nowtask] == 1)
        {
            initial_context(nowtask);
            is_finish[nowtask] = 0;
        }
    }


    return 0;
}
void initial_context(int i)
{
    getcontext(&ctx[i]);
    ctx[i].uc_stack.ss_sp = str[i];
    ctx[i].uc_stack.ss_size = sizeof(str[i]);
    ctx[i].uc_link = NULL;
    ctx[i].uc_stack.ss_flags = 0;
    makecontext(&ctx[i],task_const[i].entry,0);

}
int find_best_priority()
{
    int pmax = 0;
    int id,maxid = 0;
    for(int i=0; i<readyQ_num; i++)
    {
        id = ready_queue[i];
        if(priority[id] > pmax)
        {
            maxid = id;
            pmax = priority[id];
        }
        else if(priority[id] == pmax)
        {
            if(state[id] == 2)
            {
                maxid = id;
                pmax = priority[id];
            }
        }
    }
    return maxid;
}

