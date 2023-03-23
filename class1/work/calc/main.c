/*
 * @Author: ZhuYongtao
 * @Date: 2022-12-09 00:16:24
 * @LastEditTime: 2022-12-11 23:59:16
 */

#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "add.h"
#include "sub.h"
#include "mux.h"
#include "div.h"

int a,b;

void getHelp(){
    printf("\n-------------------------------\n");
    printf("1-add\n");
    printf("2-sub\n");
    printf("3-mux\n");
    printf("1-div\n");
    printf("palse input chioce: \n");

}

void getNUm(){
    printf("please enter two numbers:\n");
    scanf("%d%d",&a,&b);
}

int main(int argc, char **argv)
{
    int my_chioce;
    
    while(1){
        getHelp();
        scanf("%d", &my_chioce);
        switch(my_chioce){
            case 1:
				getNUm();
                printf("result:%d", add_me(a,b));
				break;
            case 2:
				getNUm();
                printf("result:%d", sub_me(a,b));
				break;
            case 3:
				getNUm();
                printf("result:%d", mux_me(a,b));
				break;
            case 4:
				getNUm();
                printf("result:%f", div_me(a,b));
				break;
            default:
				printf("<<=====invalid usage=====>>\r\n");
        }
    }
    
}