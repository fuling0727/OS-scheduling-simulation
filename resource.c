#include "resource.h"
#include "task.h"
#include <ucontext.h>
int resource_owner[RESOURCES_COUNT];
int nowtask;
struct ucontext ctx[TASKS_COUNT],ctx_main;
int priority[TASKS_COUNT];
int resource_owner[RESOURCES_COUNT];
status_type get_resource(resource_type id)
{
    status_type result;
    if(resource_owner[id] == 0)
    {
        result = STATUS_OK;
        resource_owner[id] = nowtask;
        priority[nowtask] = resources_priority[id];

    }
    else
    {
        result = STATUS_ERROR;
    }
    return result;
}

status_type release_resource(resource_type id)
{
    status_type result;
    if(resource_owner[id] == nowtask)
    {
        priority[nowtask] = task_const[nowtask].static_priority;
        resource_owner[id] = 0;
        swapcontext(&ctx[nowtask],&ctx_main);
        result = STATUS_OK;
        return result;
    }
    else
    {
        result = STATUS_ERROR;
        return result;
    }
}
