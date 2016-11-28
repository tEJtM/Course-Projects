#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <pthread.h>

const int debug = 1;
static int BOUND = 1000007;

typedef long long UI;

void DEBUG(char *s) {
	if (!debug) {
		return ;
	}
	puts(s);
	fflush(stdout);
}

typedef int(*MUTEX_LOCK)(pthread_mutex_t *);
MUTEX_LOCK old_mutex_lock = NULL;

typedef int(*MUTEX_UNLOCK)(pthread_mutex_t *);
MUTEX_UNLOCK old_mutex_unlock = NULL;

pthread_mutex_t hook_mutex = PTHREAD_MUTEX_INITIALIZER;

struct HashTable {
	int head[1000007], next[1000007], tot;
	UI value[1000007];
};

struct HashTable table;
int go[1000007], mutex_set[1000007];

int hash_code(UI x) {
	return (x % BOUND + BOUND) % BOUND;
}

void hash_clear(struct HashTable *table) {
	int i;
	for (i = 0; i < BOUND; i++) {
		table->head[i] = table->next[i] = -1;
		go[i] = mutex_set[i] = -1;
	}
	table->tot = 0;
}

int table_find(struct HashTable *table, UI a) {
DEBUG("fuck");
	int ta = hash_code(a), ptr;
	for (ptr = table->head[ta]; ptr != -1; ptr = table->next[ptr]) {
		if (table->value[ptr] == a) {
			return ptr;
		}
	}

	int tot = table->tot;
	table->next[tot] = table->head[ta];
	table->head[ta] = tot;
	table->value[tot] = a;
	table->tot++;
	if (debug) {
		printf("new tot = %d\n", tot);
		fflush(stdout);
	}
	return tot;
}

int get_mutex_id(pthread_mutex_t *x) {
	return table_find(&table, (UI)x + 5678901234ULL);
}

int get_thread_id(UI x) {
	return table_find(&table, x);
}

void detector_delete(pthread_mutex_t *mutex) {
	int id = get_mutex_id(mutex);

	mutex_set[id] = -1;
	go[id] = -1;
}

void detector_detect(int x) {
	int y = go[x];
	while (y != -1 && y != x) {
		y = go[y];
	}
	if (y == x) {
		FILE* fout = fopen("report.txt", "w");
		//puts("BOOOOM");
		//fllush(stdout);
		fputs("DeadLock Detected", fout);
		fclose(fout);
		exit(0);
	}
}

void detector_wait(UI thread, pthread_mutex_t *mutex) {
	int tid = get_thread_id(thread), mid = get_mutex_id(mutex);
	go[tid] = mid;
	detector_detect(tid);
}

void detector_hold(UI thread, pthread_mutex_t *mutex) {
	UI tid = get_thread_id(thread), mid = get_mutex_id(mutex);
	mutex_set[mid] = 1;
	go[mid] = tid;
}

int detector_mutex_free(pthread_mutex_t *mutex) {
	int mid = get_mutex_id(mutex);
	return mutex_set[mid] == -1;
}

void detector_init() {
	static int flag = 0;

	old_mutex_lock(&hook_mutex);

	if (flag) {
		return ;
	}
	flag = 1;

	hash_clear(&table);
	old_mutex_unlock(&hook_mutex);
}


int pthread_mutex_lock(pthread_mutex_t *mutex) {
DEBUG("gao ye god");
	static void *handle = NULL;

	if (!handle) {
		handle = dlopen("libc.so.6", RTLD_LAZY);
		old_mutex_lock = (MUTEX_LOCK)dlsym(handle, "pthread_mutex_lock");
		old_mutex_unlock = (MUTEX_LOCK)dlsym(handle, "pthread_mutex_unlock");

		detector_init();
	}
DEBUG("enter1");
	old_mutex_lock(&hook_mutex);
DEBUG("outer1");
	if (!detector_mutex_free(mutex)) {
		detector_wait(pthread_self(), mutex);
DEBUG("wait");
		old_mutex_unlock(&hook_mutex);
		return old_mutex_lock(mutex);
	} else {
		/*
		int ret = old_mutex_lock(mutex);
		if (ret == 0) {
			detector_hold(pthread_self(), mutex);
		}
DEBUG("get lock\n");
		old_mutex_unlock(&hook_mutex);
		return ret;
		*/

		detector_hold(pthread_self(), mutex);
DEBUG("get lock\n");
		old_mutex_unlock(&hook_mutex);
		return old_mutex_lock(mutex);
	}
}

int pthread_mutex_unlock(pthread_mutex_t *mutex) {

	old_mutex_lock(&hook_mutex);

	detector_delete(mutex);

	old_mutex_unlock(&hook_mutex);
	return old_mutex_unlock(mutex);
}
