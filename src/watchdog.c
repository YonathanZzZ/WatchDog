#define _POSIX_C_SOURCE 200112L
#define __USE_POSIX    /* struct sigaction */
#include <stdatomic.h> /* atomic_int */
#include <sys/types.h> /* fork */
#include <unistd.h>    /* fork */
#include <pthread.h>   /* pthread_create */
#include <signal.h>    /* SIGUSR1, SIGUSR2 */
#include <assert.h>    /* assert */
#include <stdio.h>     /* perror */
#include <errno.h>     /* perror */
#include <stdlib.h>    /* getenv, unsetenv, atoi */
#include <semaphore.h> /* sem_open */
#include <fcntl.h>     /* sem_open O constants */
#include <sys/stat.h>  /* semaphore modes*/

#include "watchdog.h"
#include "scheduler.h"

#define SEM_NAME ("kausdk")

atomic_int sig_counter = 0;

pid_t wd_pid = 0;
pid_t pid = 0;

pthread_t sched_thread = 0;
sem_t *sem = NULL;
int stop_flag = 0;
sigset_t signal_set = {0};

scheduler_t *sched = NULL;

static void SIGUSR1Handler(int sig_num)
{
    (void)sig_num;
    ++sig_counter;
}

static void SIGUSR2Handler(int sig_num)
{
    (void)sig_num;
    stop_flag = 1;
}

static void *SchedThreadFunc(void *arg)
{
    (void)arg;

    SchedulerRun(sched);

    return (NULL);
}

static int SendSIGUSR1Task(void *pid)
{
    if (stop_flag)
    {
        return (-1);
    }

    if (WD_SUCCESS != kill(*(pid_t *)pid, SIGUSR1))
    {
        perror("Failed to send signal");

        return (-1);
    }

    return (0);
}

static int ReviveProcess(char *path)
{
    char env_str[10] = {'\0'};

    assert(path);

    /* check if this process is the watchdog */
    if (NULL == getenv("WD_PID"))
    {
        /* watchdog process died */
        wd_pid = fork();
        if (-1 == wd_pid)
        {
            perror("Failed to create child process");
            return (-1);
        }

        if (0 == wd_pid)
        {
            /* watchdog process */

            if (-1 == execl("./watchdog_op.out", path, (char *)NULL))
            {
                perror("Failed to run watchdog executable in revive.");
                return (-1);
            }
        }
        else
        {
            /* parent process */
            sem_wait(sem);

            sprintf(env_str, "%d", wd_pid);
            setenv("WD_PID", env_str, 1);
        }
    }
    else if (atoi(getenv("WD_PID")) == getpid())
    {
        /* user process died */
        pid = fork();
        if (-1 == pid)
        {
            perror("Failed to create child process");
            return (-1);
        }

        if (0 == pid)
        {
            /* user process */
            if (-1 == execl(path, "./watchdog_op.out", (char *)NULL))
            {
                perror("Failed to load user executable");
                return (-1);
            }
        }
        else
        {
            /* watchdog process */
            sem_wait(sem);
        }
    }

    return (0);
}

static int CheckCounterTask(void *data)
{
    assert(data);

    /* check signal counter */

    if (0 != sig_counter)
    {
        sig_counter = 0;
    }
    else
    {
        /* the other process was terminated. recreate it. */
        ReviveProcess(data);
    }

    return (0);
}

static int CheckStopFlagTask(void *data)
{
    (void)data;

    if (stop_flag)
    {
        SchedulerStop(sched);

        sem_destroy(sem);

        /* send SIGUSR2 to other process */
        kill(pid, SIGUSR2);
    }

    return (0);
}

int WDStart(char **path)
{
    struct sigaction sig_act1 = {0};
    struct sigaction sig_act2 = {0};
    char *wd_pid_str = NULL;
    char curr_path[500] = {'\0'};

    assert(path);

    /* define signal handler 1 */
    sigemptyset(&sig_act1.sa_mask);
    sig_act1.sa_flags = 0;
    sig_act1.sa_handler = &SIGUSR1Handler;
    sigaction(SIGUSR1, &sig_act1, NULL);

    /* define signal handler 2 */
    sigemptyset(&sig_act2.sa_mask);
    sig_act2.sa_flags = 0;
    sig_act2.sa_handler = &SIGUSR2Handler;
    sigaction(SIGUSR2, &sig_act2, NULL);

    sem = sem_open(SEM_NAME, O_CREAT, S_IRWXU | S_IRWXO, 0);
    if (NULL == sem)
    {
        perror("sem_open error:\n");
        return -1;
    }

    /* check if this is the user process or the watchdog process */
    wd_pid_str = getenv("WD_PID");
    if (NULL == wd_pid_str)
    {
        /* watchdog process does not exist */
        /* create child process for watchdog */
        wd_pid = fork();

        if (-1 == wd_pid)
        {
            perror("Failed to create child.");
            return (WD_FAILED_TO_CREATE_CHILD_PROCESS);
        }

        if (0 == wd_pid)
        {
            /* child process */
            if (-1 == execl("./watchdog_op.out", *path, (char *)NULL))
            {
                perror("Failed to run watchdog executable.");
                return (-1);
            }
        }
        else
        {
            /* parent process */
            sched = SchedulerCreate();

            SchedulerAddTask(sched, &SendSIGUSR1Task, &wd_pid, 0, 1, NULL, NULL);
            SchedulerAddTask(sched, &CheckCounterTask, *path, 2, 2, NULL, NULL);
            SchedulerAddTask(sched, &CheckStopFlagTask, NULL, 1, 1, NULL, NULL);

            sem_wait(sem);

            pthread_create(&sched_thread, NULL, &SchedThreadFunc, sched);

            /* add SIGUSR1 to signal set, then mask the set */
            sigemptyset(&signal_set);
            sigaddset(&signal_set, SIGUSR1);
            pthread_sigmask(SIG_BLOCK, &signal_set, NULL);
        }
    }
    else if (atoi(wd_pid_str) == getpid())
    {
        /* current process is watchdog */
        pid = getppid();

        sched = SchedulerCreate();

        SchedulerAddTask(sched, &SendSIGUSR1Task, &pid, 0, 1, NULL, NULL);
        SchedulerAddTask(sched, &CheckCounterTask, *path, 2, 2, NULL, NULL);
        SchedulerAddTask(sched, &CheckStopFlagTask, NULL, 1, 1, NULL, NULL);
        sem_post(sem);

        SchedulerRun(sched);

        SchedulerDestroy(sched);
    }
    else
    {
        /* user process has been revived */
        sched = SchedulerCreate();

        pid = atoi(getenv("WD_PID"));

        SchedulerAddTask(sched, &SendSIGUSR1Task, &pid, 0, 1, NULL, NULL);
        SchedulerAddTask(sched, &CheckCounterTask, *path, 2, 2, NULL, NULL);
        SchedulerAddTask(sched, &CheckStopFlagTask, NULL, 1, 1, NULL, NULL);

        sem_post(sem);

        pthread_create(&sched_thread, NULL, &SchedThreadFunc, sched);
    }

    return (0);
}

void WDStop(size_t timeout)
{
    time_t start_time = time(0);

    while (!stop_flag)
    {
        if (difftime(time(0), start_time) >= timeout)
        {
            kill(wd_pid, SIGKILL);
        }
        else
        {
            kill(wd_pid, SIGUSR2);
        }
    }

    unsetenv("WD_PID");

    sem_unlink(SEM_NAME);

    SchedulerStop(sched);

    pthread_join(sched_thread, NULL);

    SchedulerDestroy(sched);

    /* unblock SIGUSR1 */
    pthread_sigmask(SIG_UNBLOCK, &signal_set, NULL);
}