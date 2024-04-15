#ifndef usage_h
#define usage_h

#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <unistd.h>

float cpu_usage() {
    FILE* file = fopen("/proc/stat", "r");
    if (!file) {
        fprintf(stderr, "Erreur: Impossible d'ouvrir /proc/stat\n");
        return -1.0;
    }

    char line[128];
    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    float total_prev = 0.0, total_curr = 0.0;
    float idle_prev = 0.0, idle_curr = 0.0;
    float usage;

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "cpu ", 4) == 0) {
            sscanf(line + 5, "%llu %llu %llu %llu %llu %llu %llu %llu %llu %llu", &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal, &guest, &guest_nice);
            total_curr = user + nice + system + idle + iowait + irq + softirq + steal;
            idle_curr = idle;
            break;
        }
    }

    fclose(file);

    if (total_curr - total_prev != 0) {
        usage = ((total_curr - total_prev) - (idle_curr - idle_prev)) / (total_curr - total_prev) * 100;
    } else {
        usage = 0.0;
    }

    total_prev = total_curr;
    idle_prev = idle_curr;

    return usage;
}

float ram_usage() {
    FILE* file = fopen("/proc/meminfo", "r");
    if (!file) {
        fprintf(stderr, "Erreur: Impossible d'ouvrir /proc/meminfo\n");
        return -1.0;
    }

    char line[128];
    unsigned long long total_mem, free_mem, buffers, cached;
    float usage;

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "MemTotal:", 9) == 0) {
            sscanf(line + 9, "%llu kB", &total_mem);
        } else if (strncmp(line, "MemFree:", 8) == 0) {
            sscanf(line + 8, "%llu kB", &free_mem);
        } else if (strncmp(line, "Buffers:", 8) == 0) {
            sscanf(line + 8, "%llu kB", &buffers);
        } else if (strncmp(line, "Cached:", 7) == 0) {
            sscanf(line + 7, "%llu kB", &cached);
        }
    }

    fclose(file);

    usage = 100.0 * (1.0 - ((float)free_mem / (float)(total_mem - buffers - cached)));

    return usage;
}


#endif