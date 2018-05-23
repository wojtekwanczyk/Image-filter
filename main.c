#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <zconf.h>
#include <sys/times.h>
#include <math.h>

int W, H, C, thread_quantity;
unsigned char **I;
float **K;
unsigned char **J;

void *thread_action(void *args);

static void write_times(clock_t, struct tms *, struct tms *);
void load_pic(const char *name);
void load_filter(const char *name);
void save_pic(const char *name);


//=======================================================================

int main(int argc, char *argv[]) {

    if(argc < 5){
        printf("run with args: ./program thread_qtt pict_name filter_name out_name\n");
        exit(1);
    }
    thread_quantity = atoi(argv[1]);
    if(thread_quantity < 1 || thread_quantity > 8){
        printf("Wrong thread quantity\n");
        exit(1);
    }
    char *pict_name = argv[2];
    char *filter_name = argv[3];
    char *out_name = argv[4];


    load_pic(pict_name);
    load_filter(filter_name);

    struct tms   tms_start, tms_end;
    clock_t      start, end;

    int *nrs = malloc(thread_quantity * sizeof(int));                   //thread numbers to identify them
    pthread_t *threads = malloc(thread_quantity * sizeof(pthread_t));       //threads

    J = malloc(H * sizeof(char*));
    for(int i=0; i<H ; i++){
        J[i] = malloc(W * sizeof(char));
    }

    if ((start = times(&tms_start)) == -1)                   // remember start time
    {printf("Error while getting start time"); exit(-1);}


    for(int i=0; i< thread_quantity; i++){
        nrs[i] = i;
        pthread_create(&threads[i], NULL, thread_action, &nrs[i]);
    }

    for(int i=0; i< thread_quantity; i++){
        pthread_join(threads[i], NULL);
    }

    if ((end = times(&tms_end)) == -1)                       // remember end time
    {printf("Error while getting end time"); exit(-1);}

    save_pic(out_name);

    write_times(end-start, &tms_start, &tms_end);

    return 0;
}

//==============================================================================



void load_pic(const char *name){
    FILE *f = fopen(name, "r");

    int tmp;
    fscanf(f, "P2\n");
    fscanf(f, "%d", &W);
    fscanf(f, "%d", &H);
    fscanf(f, "%d", &tmp);

    I = malloc(H * sizeof(char*));
    for(int i=0; i<H ; i++){
        I[i] = malloc(W * sizeof(char));
    }

    for(int i=0; i<H; i++){
        for(int j=0; j<W; j++){
            fscanf(f, "%hhd", &I[i][j]);
        }
    }
    fclose(f);
}

void load_filter(const char *name){
    FILE *f = fopen(name, "r");
    fscanf(f, "%d", &C);

    K = malloc(C * sizeof(float*));
    for(int i=0; i < C; i++){
        K[i] = malloc(C * sizeof(float));
    }

    for(int i = 0; i < C; i++){
        for(int j=0; j < C; j++){
            fscanf(f, "%f", &K[i][j]);
        }
    }

    fclose(f);
}


void save_pic(const char *name){
    FILE *f = fopen(name, "w");

    fprintf(f, "P2\n%d %d\n255\n", W, H);

    for(int i=0; i<H; i++){
        for(int j=0; j<W; j++){
            if((i*H+j)%10 == 0)
                fprintf(f, "\n");
            fprintf(f, "%d ", J[i][j]);
        }
    }

    fclose(f);
}


int min(int a, int b){
    if(a < b) return a;
    return b;
}

int max(int a, int b){
    if(a > b) return a;
    return b;
}


void *thread_action(void *args){
    int nr = *(int *)args;
    //printf("My id number is %d\n", nr);

    int columns = W/thread_quantity;
    int res = W % thread_quantity;

    int start = nr * columns;
    if(nr == thread_quantity - 1) columns += res;
    int end = start  + columns;

    int s;
    int l,r;

    for(int y=start; y < end; y++){
        for(int x=0; x<H; x++){

            s = 0;
            for(int i=0; i < C; i++){
                for(int j=0; j < C; j++){

                    l = (int)(x+1 - ceil(C/2) + i + 1);
                    r = (int)(y+1 - ceil(C/2) + j + 1);

                    l = max(0, l);
                    l = min(H-1, l);

                    r = max(0, r);
                    r = min(W-1, r);

                    s += I[l][r] * K[i][j];
                }
            }

            J[x][y] = (unsigned char)round(s);
        }
    }

    return NULL;
}



static void write_times(clock_t real_t, struct tms *tms_start, struct tms *tms_end) {
    static long clock_tck = 0;

    if (clock_tck == 0){
        if ((clock_tck = sysconf(_SC_CLK_TCK)) < 0) {
            printf("sysconf error");
            exit(-1);
        }
    }

    double real = real_t / (double)clock_tck;
    double user =(tms_end->tms_utime - tms_start->tms_utime) / (double)clock_tck;
    double system = (tms_end->tms_stime - tms_start->tms_stime) / (double)clock_tck;

    FILE *file = fopen("times.txt", "a");

    fprintf(file, "\nNumber of Threads:\t%d\n", thread_quantity);
    fprintf(file, "Image size:\t\t%d x %d\n", W, H);
    fprintf(file, "Filter size:\t\t%d\n", C);
    fprintf(file, "Time (in s):\n");

    fprintf(file, "\treal:\t%7.2f\n", real);
    fprintf(file, "\tuser:\t%7.2f\n", user);
    fprintf(file, "\tsystem:\t%7.2f\n\n", system);

    fclose(file);

    printf("\nReal time:  %7.2f\n\n", real);
    //printf("  user:  %7.2f\n", user);
    //printf("  sys:   %7.2f\n", system);
}
