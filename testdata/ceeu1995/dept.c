/*
	Problem: 	Department
	Author:		Jan Kotas
	Description:	discrete simulation
	Algorithm:	JIHAD with logging
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define AGENTS ('Z'-'A'+1)

#define FLOOR(x)  ((x)/16)

#define bool int
#define false 0
#define true 1

enum
{
	a_entry,
	a_room_queue, /* room */
	a_room, /* room */
	a_elevator_queue,
	a_elevator,
	a_room_to_room, /* room, room */
	a_room_to_elevator, /* room */
	a_elevator_to_room, /* room */
	a_exit,

	a_elevator_event
};

char *msgs[9] = {
	"Entry",
	"Waiting in front of room %04d",
	"Stay in room %04d",
	"Waiting in elevator queue",
	"Stay in elevator",
	"Transfer from room %04d to room %04d",
	"Transfer from room %04d to elevator",
	"Transfer from elevator ro room %04d",
	"Exit",
};

struct request
{
	struct request *next;
	int room, length;
};

struct request *requests[AGENTS];

struct act
{
	struct act *next;
	int agent;
	int type;
	int from, to;
	int room, room_to;
};

struct act *heads[AGENTS];

struct act * alloc_act(int agent, int type, int from, int len, int room, int room_to)
{	
	struct act *a = (struct act*)malloc(sizeof(struct act));

	a->agent = agent;
	a->type = type;

	a->from = from;
	a->to = from + len;

	a->room = room;
	a->room_to = room_to;

	return a;
}

struct act *the_head;

void schedule(struct act *a)
{
	struct act *p, **pp;

	for(p = *(pp = &the_head); p; p = *(pp = &p->next))
		if((p->to > a->to) || ((p->to == a->to) && (p->agent > a->agent)))
			break;

	(*pp = a)->next = p;		
}

void s_room_to_room(struct act *a)
{
	struct request *r = requests[a->agent];

	schedule(a->next = alloc_act(a->agent, a_room_to_room, a->to, 10, 
		a->room, r->room));
}			

void s_room_to_elevator(struct act *a)
{
	schedule(a->next = alloc_act(a->agent, a_room_to_elevator, a->to, 10, 
		a->room, 0));
}
		
void s_elevator_to_room(struct act *a)
{
	struct request *r = requests[a->agent];

	schedule(a->next = alloc_act(a->agent, a_elevator_to_room, a->to, 10, 
		r->room, 0));
}

void s_exit(struct act *a)
{
	schedule(a->next = alloc_act(a->agent, a_exit, a->to, 30, 0, 0));
}

struct room
{
	struct act * queue;
	bool full;
}
rooms[256];

void s_room(struct act *a)
{
	struct request *r = requests[a->agent];

	assert(!rooms[r->room].full);
	rooms[r->room].full = true;

	schedule(a->next = alloc_act(a->agent, a_room, a->to, r->length, r->room, 0));

	requests[a->agent] = r->next;
	free(r);
}

void w_room(struct act *a)
{
	struct request *r = requests[a->agent];

	if(rooms[r->room].full)
	{
		struct act *p, **pp;

		for(p = *(pp = &rooms[r->room].queue); p; p = *(pp = &p->next))
			if(p->agent > a->agent) break;

		(*pp = (a->next = alloc_act(a->agent, a_room_queue, a->to, 0, r->room, 0)))->next = p;				
	}
	else
	{
		s_room(a);
	}
}

struct act elevator_kicker = { NULL, 256, a_elevator_event, 0, 0, 0, 0 };
bool elevator_scheduled;


struct act * up_queues[16];
int up_times[80];

struct act * down_queues[16];
int down_times[80];

void w_elevator(struct act *a, int floor, int floor_to)
{
	struct act *p, **pp;

	assert(floor != floor_to);

	for(p = *(pp = (floor_to > floor) ? &up_queues[floor] : &down_queues[floor]); 
			p; p = *(pp = &p->next))
		if(p->agent > a->agent) break;

	(*pp = (a->next = alloc_act(a->agent, a_elevator_queue, a->to, 0, floor, floor_to)))->next = p;

	if(!elevator_scheduled)
	{
		elevator_kicker.to = 5 * ((a->to + 4) / 5);
		schedule(&elevator_kicker);
		elevator_scheduled = true;
	}
}

void e_elevator(struct act *a)
{
	int i, index;
	struct act *b;

	assert(elevator_scheduled);
	elevator_scheduled = false;
	
	for(i = 0; i < 16; i++)
	{
		if(!up_queues[i])
			continue;

		index = 80 - (a->to / 5) % 80;

		if(up_times[index] > a->to)
		{
			elevator_scheduled = true;
			continue;
		}

		if(!!(up_queues[i] = (b = up_queues[i])->next))
			elevator_scheduled = true;

		up_times[index] = a->to + 30 * (b->room_to - b->room);

		if(a->to == b->from)
		{
			assert(b->type == a_elevator_queue);

			b->type = a_elevator;
			b->to = b->from + 30 * (b->room_to - b->room);

			schedule(b);
		}
		else
		{
			b->to = a->to;

			schedule(b->next = alloc_act(b->agent, a_elevator, b->to,
				30 * (b->room_to - b->room), b->room, b->room_to));
		}
	}

	for(i = 0; i < 16; i++)
	{
		if(!down_queues[i])
			continue;

		index = (a->to / 5) % 80;

		if(down_times[index] > a->to)
		{
			elevator_scheduled = true;
			continue;
		}

		if(!!(down_queues[i] = (b = down_queues[i])->next))
			elevator_scheduled = true;

		down_times[index] = a->to + 30 * (b->room - b->room_to);

		if(a->to == b->from)
		{
			assert(b->type == a_elevator_queue);

			b->type = a_elevator;
			b->to = b->from + 30 * (b->room - b->room_to);

			schedule(b);
		}
		else
		{
			b->to = a->to;

			schedule(b->next = alloc_act(b->agent, a_elevator, b->to,
				30 * (b->room_to - b->room), b->room, b->room_to));
		}
	}

	if(elevator_scheduled)
	{
		a->to += 5;
		schedule(a);
	}
}

void print_time(int t)
{
	printf("%02d:%02d:%02d", t / 3600, (t / 60) % 60, t % 60);
}

int main(void)
{
	struct act *a, *b;
	struct request *r, **rr;
	char c[16];
	int i, h, m, s, rm, tm;

	memset(heads, 0, sizeof(heads));
	memset(requests, 0, sizeof(requests));

	memset(rooms, 0, sizeof(rooms));

	the_head = NULL;

	for(;;)
	{
		scanf("%s", c);

		if(c[0] == '.') break;

		scanf("%d:%d:%d", &h, &m, &s);

		i = c[0] - 'A';

		schedule(heads[i] = alloc_act(i, a_entry, 3600 * h + 60 * m + s, 30, 0, 0));

		rr = &requests[i];

		for(;;)
		{
			scanf("%d", &rm);

			if(!rm) break;

			scanf("%d", &tm);

			r = (*rr = (struct request *)malloc(sizeof(struct request)));

			r->room = 16 * (rm / 100) + (rm % 100) - 17;
			r->length = tm;

			rr = &r->next;
		}

		*rr = NULL;
	}

	while(the_head)
	{
		the_head = (a = the_head)->next;

		if(a->agent < AGENTS)
			r = requests[a->agent];
		
		switch(a->type)
		{
		case a_entry:
			if(r)
				if(FLOOR(r->room)) w_elevator(a, 0, FLOOR(r->room));
				else w_room(a); 
			else
				s_exit(a);
			break;

		case a_room:

			assert(rooms[a->room].full);
			rooms[a->room].full = false;

			if(!!(b = rooms[a->room].queue))
			{
				rooms[a->room].queue = b->next;

				b->to = a->to;
				s_room(b);
			}			

			if(r)
				if(FLOOR(a->room) == FLOOR(r->room)) s_room_to_room(a);
				else s_room_to_elevator(a);
			else
				if(FLOOR(a->room)) s_room_to_elevator(a);
				else s_exit(a);

			break;
		
		case a_elevator:

			if(r)
				s_elevator_to_room(a);
			else
				s_exit(a);

			break;

		case a_room_to_room:
			w_room(a);
			break;

		case a_room_to_elevator:
			w_elevator(a, FLOOR(a->room), r ? FLOOR(r->room) : 0);
			break;

		case a_elevator_to_room:
			w_room(a);
			break;

		case a_exit:
			a->next = NULL;
			break;

		case a_elevator_event:
			assert(a == &elevator_kicker);
			e_elevator(a);
			break;

		default:
			assert(false);
		}
	}

	for(i = 0; i < AGENTS; i++)
	{
		if(!heads[i])
			continue;

		printf("%c\n", i + 'A');

		for(a = heads[i]; a; )
		{
			print_time(a->from);
			printf(" ");
			print_time(a->to);
			printf(" ");
			printf(msgs[a->type], 
				100 * (a->room / 16) + (a->room % 16) + 101,
				100 * (a->room_to / 16) + (a->room_to % 16) + 101);
			printf("\n");

			a = (b = a)->next;
			free(b);
		}

		printf("\n");
	}
	return 0;
}
