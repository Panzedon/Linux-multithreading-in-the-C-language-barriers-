#include <pthread.h>
#include <stdio.h>
#include <math.h>

float f1_result;
float f2_result;
float f3_result;
pthread_barrier_t barrier1;
pthread_barrier_t barrier2;

struct values{
    float a;
    float b;
    short n;
    float h;
};

void *func1(void *arg){
    float x;
    struct values thread_arg = *((struct values*)arg);
    for(short i = 0; i < thread_arg.n; i++){
        x = thread_arg.a + thread_arg.h * i;
        f1_result = sin(x) * sin(x) * cos(x);
        pthread_barrier_wait(&barrier1);
        pthread_barrier_wait(&barrier2);
    }
}

void *func2(void *arg){
    float x;
    struct values thread_arg = *((struct values*)arg);
    for(short i = 0; i < thread_arg.n; i++){
        x = thread_arg.a + thread_arg.h * i;
        f2_result = sin(x) - cos(x) * cos(x);
        pthread_barrier_wait(&barrier1);
        pthread_barrier_wait(&barrier2);
    }
}

void *func3(void *arg){
    float x;
    struct values thread_arg = *((struct values*)arg);
    for(short i = 0; i < thread_arg.n; i++){
        x = thread_arg.a + thread_arg.h * i;
        f3_result = sin(x) * (1 - cos(x) * cos(x));
        pthread_barrier_wait(&barrier1);
        pthread_barrier_wait(&barrier2);
    }
}

int main ()
{
	pthread_t thread_f1;
	pthread_t thread_f2;
	pthread_t thread_f3;

	struct values arguments;
	arguments.a = -2 * M_PI;
	arguments.b = 0;
	arguments.n = 12;
	arguments.h = (2*M_PI)/12;
	char sep[] = "_______________________";
	char probil[] = " ";


    pthread_barrier_init(&barrier1, NULL, 4);
    pthread_barrier_init(&barrier2, NULL, 4);
	pthread_create(&thread_f1, NULL, &func1, &arguments);
    pthread_create(&thread_f2, NULL, &func2, &arguments);
    pthread_create(&thread_f3, NULL, &func3, &arguments);

    printf("_%23s_%23s_%23s_%23s_\n", sep, sep, sep, sep);
    printf("|%23s|%23s|  %23s|  %23s|\n", "x", "sin^2(x)*cos(x)", "sin(x) – cos^2(x)",
    "sin(x)*(1 – cos^2(x))");
    printf("|%23s|%23s|%23s|%23s|\n", sep, sep, sep, sep);


    float x;
    for(int i = 0; i < arguments.n; i++){
        pthread_barrier_wait(&barrier1);
        x = arguments.a + arguments.h * i;
        printf("|%23.4f|", x);
        printf("%23.4f|", f1_result);
        printf("%23.4f|", f2_result);
        printf("%23.4f|", f3_result);
        printf("\n");
        printf("|%23s|%23s|%23s|%23s|\n", sep, sep, sep, sep);
        pthread_barrier_wait(&barrier2);
    }
    printf("|%23s_%23s_%23s_%23s|\n", sep, sep, sep, sep);

    pthread_join(thread_f1, NULL);
    pthread_join(thread_f1, NULL);
    pthread_join(thread_f1, NULL);
    pthread_barrier_destroy(&barrier1);
    pthread_barrier_destroy(&barrier2);

	return 0;
}