#ifndef TASK_H
#define TASK_H

#include "typedefine.h"
#include "config.h"
#include <ucontext.h>
status_type activate_task(task_type id);
status_type terminate_task(void);
task_state_type is_suspended(task_type id);
task_state_type is_ready(task_type id);
task_state_type is_run(task_type id);

void popout_readyQ(task_type id);
void push_in_readyQ(int id);



typedef struct ucontext
{
    unsigned long int uc_flags;
    struct ucontext *uc_link;
    stack_t uc_stack;
    mcontext_t uc_mcontext;
    __sigset_t uc_sigmask;
    //struct _fpstate __fpregs_mem;
};
//extern struct ucontext ctx[6],ctx_main;
#endif /* TASK_H */
