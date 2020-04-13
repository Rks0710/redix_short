#include <stdio.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <sys/mman.h>

void radixSort(float *, int);


int main(int argc, char *argv[]) {
    char *FILEPATH = argv[1];

    
    struct stat buffer;
    int status;
    int FILESIZE = 0;

    status = stat(FILEPATH, &buffer);
    if(status == 0) {
        FILESIZE = buffer.st_size;
    }

   
    int fd = open(FILEPATH, O_RDWR);
    float *map = mmap(0, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  
    float *floats = (float *)malloc(FILESIZE);
    memcpy(floats, map, FILESIZE);

    int n = FILESIZE / sizeof(float);
    radixSort(floats, n);


    memcpy(map, floats, FILESIZE);
    free(floats);
    close(fd);
    return 0;
}

void radixSort(float *a, int n) {
    float *t = (float *)malloc(sizeof(float)*n);
    int bitLength = sizeof(float) * 8;
    int count[2];
    int pref[2];
    int mask = 1;
    int negatives = 0, positives = 0;
    unsigned int b;
    union ufi {
     float f;
     int i;
    } u; 

    for (int shift = 0; shift < bitLength; shift++) {
    
        for (int i = 0; i < 2; i++)
            count[i] = 0;

        
        for (int i = 0; i < n; i++) {
         
            u.f = a[i];
            b = (u.i >> shift) & mask; 

            count[b]++;


            if (shift == 0 && a[i] < 0)
                negatives++;
        }
        if (shift == 0) positives = n - negatives;

        
        pref[0] = 0;
        for (int i = 1; i < 2; i++)
            pref[i] = pref[i - 1] + count[i - 1];

       
        for (int i = 0; i < n; i++) {
            
            u.f = a[i];
            b = (u.i >> shift) & mask;
         
            int index = pref[b]++;

            
            if (shift == bitLength - 1)
            {
                if (a[i] < 0)
                    index = positives - (index - negatives) - 1;
                else
                    index += negatives;
            }
            t[index] = a[i];
        }
        memcpy( a, t, sizeof(float) * n );
    }
    free(t);
    return 0;
}