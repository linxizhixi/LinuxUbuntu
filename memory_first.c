#include <conio.h>
#include <stdlib.h>

#include "iostream.h"
#include "stdio.h"
#define getjcb(type) (type *)malloc(sizeof(type))
#define getsub(type) (type *)malloc(sizeof(type))
#define NULL 0
int num, num2;     // 要调度的作业数和要回收的区域数
int m = 0;         // 已分配作业数
int flag;          // 分配成功标志
int isup, isdown;  // 回收区域存在上邻和下邻的标志
int is = 0;

struct jcb
{
    char name[10];
    char state;
    int ntime;  // 所需时间
    int size;   // 所需空间大小
    int addr;   // 所分配分区的首地址
    struct jcb *link;
} *ready = NULL, *p, *q, *as = NULL;
// 作业队列ready,已分配作业队列as
typedef struct jcb JCB;

struct subarea
{  // 分区块
    char name[10];
    int addr;  // 分区首地址
    int size;  // 分区大小
    char state;
    struct subarea *link;
} *sub = NULL, *r, *s, *cur;
// 空闲分区队列sub,当前分区指针cur
typedef struct subarea SUB;

void sort_sub() /*对空闲分区按从小到大排序*/
{
    SUB *first, *second;
    int insert = 0;
    if ((sub == NULL) || ((s->size) < (sub->size))) /*插在队列之首*/
    {
        s->link = sub;
        sub = s;
    }
    else
    {
        first = sub; /*寻找适当的位置插入*/
        second = first->link;
        while (second != NULL)
        {
            if ((s->size) < (second->size))
            {
                s->link = second;
                first->link = s;
                second = NULL;
                insert = 1;
            }
            else
            {
                first = first->link;
                second = second->link;
            }
        }
        if (insert == 0) first->link = s; /*插在队尾*/
    }
}

void sort_sub() /*对空闲分区按从小到大排序*/ sort * /
{
    JCB *first;
    if (ready == NULL)
        ready = p;
    else
    {
        first = ready;
        while (first->link != NULL)
            first = first->link;
        first->link = p;
        p->link = NULL;
    }
}

void lastsort() /*建立对已分配作业队列的排列函数,直接插在队列之尾sort3*/
{
    JCB *fir;
    if (as == NULL)
        as = q;
    else
    {
        fir = as;
        while (fir->link != NULL)
            fir = fir->link;
        fir->link = q;
        q->link = NULL;
    }
    m++;
}

void input() /* 建立作业控制块函数*/
{
    int i;
    printf("\n请输入要调度的总作业数:");
    scanf("%d", &num);
    for (i = 0; i < num; i++)
    {
        printf("\n作业号No.%d:\n", i);
        p = getjcb(JCB);
        printf("\n输入作业名:");
        scanf("%s", &p->name);
        printf("\n输入作业的大小:");
        scanf("%d", &p->size);
        printf("\n输入作业所需运行时间:");
        scanf("%d", &p->ntime);
        p->state = 'w';
        p->link = NULL;
        firstsort(); /* 调用sort函数*/
    }
    printf("\n 按任一键继续......\n");
    getch();
}

void input2() /*建立要回收区域的函数*/
{
    JCB *k;
    int has;
    q = getjcb(JCB);
    printf("\n输入区域名(作业名):");
    scanf("%s", &q->name);
    p = as;
    while (p != NULL)
    {
        if (strcmp(p->name, q->name) == 0) /*在已分配作业队列中寻找*/
        {
            q->addr = p->addr;
            q->size = p->size;
            has = 1; /*输入作业名存在标志位*/
            if (p == as)
                as = p->link; /*在已分配作业队列中删除该作业*/
            else
            {
                k = as;
                while (k->link != p) k = k->link;
                k->link = k->link->link; /*删除*/
            }
            printf("输出该作业首地址:%d\n", q->addr);
            printf("输出该作业大小:%d\n\n", q->size);
            q->link = NULL;
            break;
        }
        else
        {
            p = p->link;
            has = 0;
        } /*输入作业名不存在标志*/
    }
    if (has == 0)
    {
        printf("\n输入作业名错误!请重新输入!\n");
        input2();
    }
}

void init_sub() /*初始化空闲分区表*/
{
    r = getsub(SUB);
    strcpy(r->name, "one");
    r->addr = 5;
    r->size = 10;
    r->state = 'n';
    sub = r;
    s = getsub(SUB);
    strcpy(s->name, "two");
    s->addr = 20;
    s->size = 120;
    s->state = 'n';
    sub->link = s;
    r = s;
    s = getsub(SUB);
    strcpy(s->name, "three");
    s->addr = 160;
    s->size = 40;
    s->state = 'n';
    r->link = s;
    r = s;
    s = getsub(SUB);
    strcpy(s->name, "four");
    s->addr = 220;
    s->size = 10;
    s->state = 'n';
    r->link = s;
    r = s;
    s = getsub(SUB);
    strcpy(s->name, "five");
    s->addr = 250;
    s->size = 20;
    s->state = 'n';
    r->link = s;
}

void disp() /*空闲分区表的显示函数*/
{
    printf("\t\t 分区          首地址           长度          状态 \n");
    r = sub;
    while (r != NULL)
    {
        printf("\t\t %s\t\t%d\t\t%d\t\t%c\n", r->name, r->addr, r->size, r->state);
        r = r->link;
    }
    printf("\n");
}

void disp2() /*显示已分配内存的作业表函数*/
{
    printf("\t\t 作业名         首地址          长度          状态 \n");
    p = as;
    while (p != NULL)
    {
        printf("\t\t %s\t\t%d\t\t%d\t\t%c\n", p->name, p->addr, p->size, p->state);
        p = p->link;
    }
    printf("\n\n");
}

void perfit(JCB *pr) /*最佳适应作业分区assign*/
{
    SUB *k;
    r = sub;
    while (r != NULL)
    {
        if (((r->size) > (pr->size)) && (r->state == 'n')) /*有空闲分区大于作业大小的情况*/
        {
            pr->addr = r->addr;
            r->size -= pr->size;
            r->addr += pr->size;
            if (r == sub)
                sub = r->link; /*删除空闲分区*/
            else
            {
                s = sub;
                while (s->link != r) s = s->link;
                s->link = s->link->link; /*删除空闲分区*/
            }
            flag = 1; /*分配成功标志位置1*/
            q = pr;
            lastsort(); /*插入已分配作业队列*/
                        // 重新插入剩余空闲分区,插在合适位置

            if (r->size < sub->size) /*插入队首*/
            {
                r->link = sub;
                sub = r;
            }
            else /*插在适当的位置*/
            {
                s = sub;
                while ((s->size) <= (r->size)) s = s->link;
                k = sub;
                if (k == s)
                {
                    r->link = sub->link;
                    sub = r;
                }    /*插在队首的后一个位置*/
                else /*第二个以后的位置*/
                {
                    while (k->link != s) k = k->link;
                    r->link = s;
                    k->link = r;
                }
            }
            printf("作业%s的分区为[%s],首地址为%d.\n", p->name, r->name, pr->addr);
            break;
        }
        else if (((r->size) == (pr->size)) && (r->state == 'n')) /*有空闲分区等于作业大小的情况*/
        {
            pr->addr = r->addr;
            flag = 1; /*分配成功标志位置1*/
            q = pr;
            lastsort(); /*插入已分配作业队列*/
            s = sub;    /*空闲分区已完成分配,应删除*/
            while (s->link != r) s = s->link;
            s->link = s->link->link; /*删除空闲分区*/
            printf("作业%s的分区为[%s],首地址为%d.\n", p->name, r->name, pr->addr);
            break;
        }
        else
        {
            r = r->link;
            flag = 0;
        }
    }
    if (flag == 0) /*作业过大的情况*/
    {
        printf("作业%s长度过大，内存不足，分区分配出错!\n", p->name);
        is = 1;
    }
}

void firstfit(JCB *pr) /*首次适应作业分区*/
{
    SUB *k;
    r = sub; /*从空闲表头开始寻找*/
    while (r != NULL)
    {
        if (((r->size) > (pr->size)) && (r->state == 'n')) /*有空闲分区大于作业大小的情况*/
        {
            pr->addr = r->addr;
            r->size -= pr->size;
            r->addr += pr->size;
            flag = 1; /*分配成功标志位置1*/
            q = pr;
            q->state = 'r';
            lastsort(); /*插入已分配作业队列*/
            printf("作业%s的分区为[%s],首地址为%d.\n", p->name, r->name, pr->addr);
            break;
        }
        else if (((r->size) == (pr->size)) && (r->state == 'n')) /*有空闲分区等于作业大小的情况*/
        {
            pr->addr = r->addr;
            flag = 1; /*分配成功标志位置1*/
            q = pr;
            lastsort(); /*插入已分配作业队列*/
            s = sub;    /*空闲分区已完成分配,应删除*/
            while (s->link != r) s = s->link;
            s->link = s->link->link; /*删除空闲分区*/
            printf("作业%s的分区为[%s],首地址为%d.\n", p->name, r->name, pr->addr);
            break;
        }
        else
        {
            r = r->link;
            flag = 0;
        }
    }
    if (flag == 0) /*作业过大的情况*/
    {
        printf("作业%s长度过大，内存不足，分区分配出错!\n", p->name);
        is = 1;
    }
}

void cyclefit(JCB *pr) /*循环首次适应作业分区*/
{
    SUB *k;
    r = cur; /*从当前指针开始寻找*/
    while (r != NULL)
    {
        if (((r->size) > (pr->size)) && (r->state == 'n')) /*有空闲分区大于作业大小的情况*/
        {
            pr->addr = r->addr;
            r->size -= pr->size;
            r->addr += pr->size;
            flag = 1; /*分配成功标志位置1*/
            cur = r;  /*更新当前指针*/
            q = pr;
            q->state = 'r';
            lastsort(); /*插入已分配作业队列*/
            printf("作业%s的分区为[%s],首地址为%d.\n", p->name, r->name, pr->addr);
            break;
        }
        else if (((r->size) == (pr->size)) && (r->state == 'n')) /*有空闲分区等于作业大小的情况*/
        {
            pr->addr = r->addr;
            flag = 1; /*分配成功标志位置1*/
            cur = r;
            q = pr;
            lastsort(); /*插入已分配作业队列*/
            s = sub;    /*空闲分区已完成分配,应删除*/
            while (s->link != r) s = s->link;
            s->link = s->link->link; /*删除空闲分区*/
            printf("作业%s的分区为[%s],首地址为%d.\n", p->name, r->name, pr->addr);
            break;
        }
        else
        {
            r = r->link;
            if (r == NULL) /*当前指针为空时,重新由空闲区队列之首寻找*/
            {
                k = cur; /*作保存当前指针用*/
                cur = sub;
                r = cur;
            }
            if (k == r) /*又回到开始的指针时,确定为没有空间满足要求*/
            {
                cur = k;
                break;
            }
            flag = 0; /*分配不成功标志*/
        }
    }
    if (flag == 0) /*作业过大的情况*/
    {
        printf("作业%s长度过大，内存不足，分区分配出错!\n", p->name);
        is = 1;
    }
}

void less_to_more() /*把分区按大小从小到大排序*/
{
    r = sub;
    sub = NULL;
    while (r != NULL)
    {
        s = r;
        r = s->link;
        s->link = NULL;
        sort_sub(); /*调用排序函数*/
    }
}

void reclperfit(JCB *pr) /*最佳适应回收区域,按分区大小排列*/
{
    SUB *k;
    r = sub;
    while (r != NULL)
    {
        if (r->addr == ((pr->addr) + (pr->size))) /*回收区域有下邻*/
        {
            pr->size += r->size;
            s = sub;
            isdown = 1; /*下邻标志位置1*/
            while (s != NULL)
            {
                if (((s->addr) + (s->size)) == (pr->addr)) /*有下邻又有上邻*/
                {
                    s->size += pr->size;
                    k = sub;
                    while (k->link != r) k = k->link;
                    k->link = k->link->link;
                    isup = 1; /*上邻标志位置1*/
                    break;
                }
                else
                {
                    s = s->link;
                    isup = 0;
                } /*上邻标志位置0*/
            }
            if (isup == 0) /*有下邻无上邻*/
            {
                r->addr = pr->addr;
                r->size = pr->size;
            }
            break;
        }
        else
        {
            r = r->link;
            isdown = 0;
        } /*下邻标志位置0*/
    }
    if (isdown == 0) /*区域无下邻*/
    {
        s = sub;
        while (s != NULL)
        {
            if (((s->addr) + (s->size)) == (pr->addr)) /*无下邻但有上邻*/
            {
                s->size += pr->size;
                isup = 1; /*上邻标志位置1*/
                break;
            }
            else
            {
                s = s->link;
                isup = 0;
            } /*上邻标志位置0*/
        }
        if (isup == 0) /*无下邻且无上邻*/
        {
            k = getsub(SUB); /*重新生成一个新的分区结点*/
            strcpy(k->name, pr->name);
            k->addr = pr->addr;
            k->size = pr->size;
            k->state = 'n';
            r = sub;
            while (r != NULL)
            {
                if ((r->size) > (k->size)) /*按分区大小排列,回收区域插在合适的位置*/
                {
                    if (r == sub) /*第一个空闲分区大于回收区域的情况*/
                    {
                        k->link = r;
                        sub->link = k;
                    }
                    else
                    {
                        s = sub;
                        while (s->link != r) s = s->link;
                        k->link = r;
                        s->link = k;
                    }
                    break;
                }
                else
                    r = r->link;
            }
            if (r == NULL) /*所有空闲分区都大于回收区域的情况*/
            {
                s = sub;
                while (s->link != NULL) s = s->link;
                s->link = k;
                k->link = NULL;
            }
        }
    }
    printf("\n区域%s己回收.", pr->name);
}

void reclfirst(JCB *pr) /*首次适应与循环首次适应区域回收*/
{
    SUB *k;
    r = sub;
    while (r != NULL)
    {
        if (r->addr == ((pr->addr) + (pr->size))) /*回收区域有下邻*/
        {
            pr->size += r->size;
            s = sub;
            isdown = 1; /*下邻标志位置1*/
            while (s != NULL)
            {
                if (((s->addr) + (s->size)) == (pr->addr)) /*有下邻又有上邻*/
                {
                    s->size += pr->size;
                    k = sub;
                    while (k->link != r) k = k->link;
                    k->link = k->link->link;
                    isup = 1; /*上邻标志位置1*/
                    break;
                }
                else
                {
                    s = s->link;
                    isup = 0;
                } /*上邻标志位置0*/
            }
            if (isup == 0) /*有下邻无上邻*/
            {
                r->addr = pr->addr;
                r->size = pr->size;
            }
            break;
        }
        else
        {
            r = r->link;
            isdown = 0;
        } /*下邻标志位置0*/
    }
    if (isdown == 0) /*区域无下邻*/
    {
        s = sub;
        while (s != NULL)
        {
            if (((s->addr) + (s->size)) == (pr->addr)) /*无下邻但有上邻*/
            {
                s->size += pr->size;
                isup = 1; /*上邻标志位置1*/
                break;
            }
            else
            {
                s = s->link;
                isup = 0;
            } /*上邻标志位置0*/
        }
        if (isup == 0) /*无下邻且无上邻*/
        {
            k = getsub(SUB); /*重新生成一个新的分区结点*/
            strcpy(k->name, pr->name);
            k->addr = pr->addr;
            k->size = pr->size;
            k->state = 'n';
            r = sub;
            while (r != NULL)
            {
                if ((r->addr) > (k->addr)) /*按分区首地址排列,回收区域插在合适的位置*/
                {
                    if (r == sub) /*第一个空闲分区首址大于回收区域的情况*/
                    {
                        k->link = r;
                        sub->link = k;
                    }
                    else
                    {
                        s = sub;
                        while (s->link != r) s = s->link;
                        k->link = r;
                        s->link = k;
                    }
                    break;
                }
                else
                    r = r->link;
            }
            if (r == NULL) /*所有空闲分区的首址都大于回收区域首址的情况*/
            {
                s = sub;
                while (s->link != NULL) s = s->link;
                s->link = k;
                k->link = NULL;
            }
        }
    }
    printf("\n区域%s己回收.", pr->name);
}

void print()  // 初始化界面
{
    printf("\n\n\n\n");
    printf("\t\t**************************************\n");
    printf("\t\t\t主存空间的分配与回收演示\n");
    printf("\t\t**************************************\n\n\n");
    printf("\t\t\t\t蓝小花\n");
    printf("\t\t\t\t计算机学院\n");
    printf("\t\t\t\t软件4班\n");
    printf("\t\t\t\t3204007102\n");
    printf("\t\t\t\t2006年12月\n");
    printf("\n\n\n");
    printf("\t\t\t按任意键进入演示");
    getch();
    system("cls");
}

void init()
{
    printf("\n\n\n\n\t\t\t 1. 显示空闲分区                  \n");
    printf("\t\t\t 2. 分配作业                    \n");
    printf("\t\t\t 3. 回收作业                 \n");
    printf("\t\t\t 0. 返回菜单                     \n");
    printf("\t\t\t请选择你要的操作:");
}

firstfitf()  // 首次适应算法
{
    init();
    switch (getchar())
    {
    case '1':
        system("cls");
        disp();
        getch();
        system("cls");
        firstfitf();
        break;
    case '2':
        system("cls");
        system("cls");
        input();
        printf("\n");
        while (num != 0)
        {
            p = ready;
            ready = p->link;
            p->link = NULL;
            firstfit(p);
            num--;
        }
        getch();
        printf("\n\t\t              完成分配后的空闲分区表             \n");
        disp();
        printf("\n\t\t                  已分配作业表                   \n");
        disp2();
        if (is == 0)
            printf("\n 全部作业已经被分配内存.");
        else
            printf("\n 作业没有全部被分配内存.\n");
        getch();
        system("cls");
        firstfitf();
        break;
    case '3':
        system("cls");
        disp2();
        printf("\n\n按任意键进行区域回收.");
        printf("\n");
        while (as != NULL)
        {
            getch();
            input2();
            printf("按任意键继续...");
            getch();
            printf("\n");
            reclfirst(q);
            getch();
            printf("\n\t\t              回收后的空闲分区表             \n");
            disp();
            printf("\n\t\t                已分配作业表                 \n");
            disp2();
            printf("\n继续回收...(Enter)");
        }
        printf("\n所有已分配作业已完成!");
        printf("\nPress any key to return...");
        getch();
        system("cls");
        firstfitf();
        break;
    case '0':
        break;
    default:
        getch();
        system("cls");
        firstfitf();
        break;
    }
}

cyclefirstf()  // 循环首次适应算法
{
    init();
    switch (getchar())
    {
    case '1':
        system("cls");
        disp();
        getch();
        system("cls");
        cyclefirstf();
        break;
    case '2':
        system("cls");
        cur = sub;
        input();
        printf("\n");
        while (num != 0)
        {
            p = ready;
            ready = p->link;
            p->link = NULL;
            cyclefit(p);
            num--;
        }
        getch();
        printf("\n\t\t              完成分配后的空闲分区表             \n");
        disp();
        printf("\n\t\t                已分配作业表                 \n");
        disp2();
        if (is == 0)
            printf("\n 全部作业已经被分配内存.");
        else
            printf("\n 作业没有全部被分配内存.\n");
        getch();
        system("cls");
        cyclefirstf();
        break;
    case '3':
        system("cls");
        disp2();
        printf("\n\n按任意键进行区域回收.");
        while (as != NULL)
        {
            getch();
            printf("\n");
            input2();
            printf("按任意键继续...");
            getch();
            printf("\n");
            reclfirst(q);
            getch();
            printf("\n\t\t              回收后的空闲分区表             \n");
            disp();
            printf("\n\t\t                已分配作业表                 \n");
            disp2();
            printf("\n继续回收...(Enter)");
        }
        printf("\n所有已分配作业已完成!");
        getch();
        system("cls");
        cyclefirstf();
        break;
    case '0':
        break;
    default:
        getch();
        system("cls");
        cyclefirstf();
        break;
    }
}

perfitf()  // 最佳适应算法
{
    init();
    switch (getchar())
    {
    case '1':
        system("cls");
        less_to_more();
        disp();
        getch();
        system("cls");
        perfitf();
        break;
    case '2':
        system("cls");
        cur = sub;
        input();
        printf("\n");
        while (num != 0)
        {
            p = ready;
            ready = p->link;
            p->link = NULL;
            less_to_more(); /*重新排列分区,按从小到大排列*/
            perfit(p);
            num--;
        }
        getch();
        printf("\n\n\t\t     完成分配后分区表(分区按由小到达排列)    \n");
        disp();
        printf("\n\t\t                 已分配作业表                  \n");
        disp2();
        if (is == 0)
            printf("\n 全部作业已经被分配内存.");
        else
            printf("\n 作业没有全部被分配内存.\n");
        getch();
        system("cls");
        perfitf();
        break;
    case '3':
        system("cls");
        disp2();
        printf("\n\n按任意键进行区域回收.");
        while (as != NULL)
        {
            getch();
            printf("\n");
            input2();
            printf("按任意键继续...");
            getch();
            printf("\n");
            reclperfit(q);
            getch();
            printf("\n\t\t              回收后的空闲分区表             \n");
            disp();
            printf("\n\t\t                已分配作业表                 \n");
            disp2();
            printf("\n继续回收...(Enter)");
        }
        printf("\n所有已分配作业已完成!");
        getch();
        system("cls");
        perfitf();
        break;
    case '0':
        break;
    default:
        getch();
        system("cls");
        perfitf();
        break;
    }
}

void menu() /*菜单函数*/
{
    int i;
    printf("\n\n\n\t\t**************************************\n");
    printf("\t\t\t主存空间的分配与回收演示\n");
    printf("\t\t**************************************\n");
    ;
    printf("\t\t\t\n\n");
    printf("\t\t\t 1. 首次适应算法                    \n");
    printf("\t\t\t 2. 循环首次适应算法                 \n");
    printf("\t\t\t 3. 最佳适应算法                     \n");
    printf("\t\t\t 0. 退出                            \n");
    printf("\t\t\t请选择你要的操作:");
    switch (getchar())
    {
    case '1':
        system("cls");
        firstfitf();
        getch();
        system("cls");
        menu();
        break;
    case '2':
        system("cls");
        cyclefirstf();
        getch();
        system("cls");
        menu();
        break;
    case '3':
        system("cls");
        perfitf();
        getch();
        system("cls");
        menu();
        break;
    case '0':
        break;
    default:
        getch();
        system("cls");
        menu();
        break;
    }
}
void main() /*主函数*/
{
    init_sub();
    print();
    menu();
}
