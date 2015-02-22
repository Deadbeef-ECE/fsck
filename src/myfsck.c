#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>

#include "partition.h"
#include "fsck_util.h"

extern const unsigned int sector_size_bytes;

int device;

void print_usage(){
    fprintf(stdout, "Usage: ./myfsck -p <partition number> -i /path/to/disk/image\n");
    return;
}

int do_fsck(partition_t *pt, int fix_pt_num){
    if(fix_pt_num >= 0){
        if(fix_pt_num >0){
            do_fix(fix_pt_num);
        }else if(fix_pt_num == 0){
             int i = 1;
             while(1){
                 if(parse_pt_info(pt, i) == -1)
                     break;
                 if(pt->type == EXT_2)    
                     do_fix(i);
                 i++;
             }
        }
    }else{
        print_usage();
    }
    return -1;
}

int main (int argc, char **argv)
{
    //printf("Hello from myfsck!\n");
    
    char* disk_name = NULL;
    uint32_t pt_num = 0;
    uint32_t fix_pt_num = 0;
    int c;
    int fsck = 0;
    while((c = getopt(argc, argv, ":p:i")) != -1){
        switch(c){
            // case 'f':
            //     fsck = 1;
            //     fix_pt_num = atoi(optarg);
            //     break;
            case 'p':
                pt_num = atoi(optarg);
                break;
            case 'i':
                disk_name = argv[optind];
                break;
            default:
                fprintf(stderr, "ERROR: Invalid input!\n");
                exit(-1);
        }
    }
    if(disk_name == NULL){
        print_usage();
        exit(-1);
    }

    //fprintf(stdout, "get pt_num[%d] and disk_name[%s]\n", pt_num, disk_name);

    if((device = open(disk_name, O_RDWR)) == -1){
        fprintf(stderr, "ERROR: Cannot open the file!\n");
        exit(-1);
    }
    
    partition_t pt;
    if(fsck == 1){
        do_fsck(&pt, fix_pt_num);
        //fsck_info_init(pt_num);
    }else{
        if(pt_num > 0){
            if(parse_pt_info(&pt, pt_num) == -1){
                printf("-1\n");
                close(device);
                exit(-1);
            }
            print_pt_info(&pt);
            do_fsck(&pt, pt_num);
        }
    }

    close(device);
    return 0;
}


