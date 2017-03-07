#include "jobHandler.class.hpp"

int					jobHandler::must_leave = 0;
uint32_t			jobHandler::q_top = 0;
t_job 				jobHandler::queue[MAXJOB];
std::mutex 			jobHandler::mut;

std::thread			jobHandler::slaves[NUMTHREAD];

void jobHandler::init()
{
	for (uint32_t i = 0; i < NUMTHREAD; i++)
	{
		jobHandler::slaves[i] = std::thread(jobHandler::job_worker, queue);
	}
}

void 	jobHandler::shutdown()
{
	must_leave = 1;
	for (uint8_t i = 0; i < NUMTHREAD; i++)
		slaves[i].join();

}

void 	jobHandler::push_job(t_job job)
{
	mut.lock();
	queue[q_top] = job;
	q_top++;
	mut.unlock();
}

t_job 	jobHandler::grab_job()
{
	t_job job;
	mut.lock();
	if (q_top > 0)
		job = (queue[q_top-- - 1]);
	mut.unlock();
	return (job);
}

void 	jobHandler::job_worker(t_job queue[MAXJOB])
{
	t_job job;

	uint32_t tq_top;

	tq_top = q_top;
	while (!must_leave)
	{
		if (q_top < 1)
		{
			usleep(1000);
			continue ;
		}
		job = grab_job();
		job.fptr(job.data);
	}
}