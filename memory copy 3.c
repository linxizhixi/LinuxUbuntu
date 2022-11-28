#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#define N 5          /* 空闲区说明表中的记录数 */
struct freePartition /* 空闲区说明表 */
{
    int startAddress; /* 空闲区起始地址*/
    int size;         /* 空闲区大小*/
    int state;        /*空闲区状态：0为空表目，1为可用空闲块*/
};
struct freePartition freeBlock[N] = {{20, 20, 1}, {80, 50, 1}, {150, 100, 1}, {300, 30, 0}, {600, 100, 1}};

/* ----------------------------------------------------------------------------------------------------------------------
 *                alloc()函数
 *	函数功能：为作业分配主存空间
 *	函数参数：param1：sizeRequest——作业所申请的空闲区大小
 *	函数返回值：如果找到符合要求的空闲区，则返回为作业分配的空闲区的起始地址
 *              否则，返回-1
 * -------------------------------------------------------------------------------------------------------------------*/
int alloc(int sizeRequest)
{
    int i;
    /* flagFind为是否找到符合要求的空闲块的标记：找到为1，未找到为0  */
    int flagFind = 0;
    int returnAddress;
    for (i = 0; i < N; i++)
    {
        if (freeBlock[i].state == 1 && freeBlock[i].size >= sizeRequest)
        {
            /* 返回为作业分配的主存地址  */
            returnAddress = freeBlock[i].startAddress;
            flagFind = 1;
            if (freeBlock[i].size > sizeRequest)
            {
                freeBlock[i].startAddress = freeBlock[i].startAddress + sizeRequest;
                freeBlock[i].size = freeBlock[i].size - sizeRequest;
                goto END;
            }
            if (freeBlock[i].size == sizeRequest)
            {
                freeBlock[i].state = 0;
                goto END;
            }
        }
    }
    if (flagFind == 0)
    {
        returnAddress = -1;
    }
END:
    return returnAddress;
}

/* --------------------------------------------------------------------------
 *                setFree()函数
 *	  函数功能：主存回收函数
 *	  函数参数：param1:startAddress 回收区的起始地址
 *              param2:size         回收区的大小
 *	  函数返回值：无返回值
 * -------------------------------------------------------------------------- */
void setFree(int startAddress, int size)
{
    int i, indexLinkNext, indexLinkPre;
    /* tagLinkNext用来标记回收区的高地址是否邻接一个空闲区, 邻接为1，否则为0; */
    /* tagLinkPre用来标记回收区的地地址是否邻接一个空闲区，邻接为1，否则为0; */
    int tagLinkPre = 0, tagLinkNext = 0;
    for (i = 0; i < N; i++)
    {
        /* 检查当前空闲区说明表中的表目是否是回收区的F2邻接区 */
        if (freeBlock[i].startAddress == startAddress + size && freeBlock[i].state == 1)
        {
            tagLinkNext = 1;
            indexLinkNext = i;
        }
        /* 检查当前空闲区说明表中的表目是否是回收区的F1邻接区 */
        if (freeBlock[i].startAddress + freeBlock[i].size == startAddress && freeBlock[i].state == 1)
        {
            tagLinkPre = 1;
            indexLinkPre = i;
        }
        /* 如果回收区的F1邻接区和F2邻接区都已找到，则跳出循环  */
        if (tagLinkPre && tagLinkNext)
        {
            break;
        }
    }
    if (tagLinkPre && tagLinkNext) /* 如果回收区与F1和F2都邻接 */
    {
        freeBlock[indexLinkPre].size += freeBlock[indexLinkNext].size + size;
        freeBlock[indexLinkNext].state = 0;
    }
    else if (tagLinkNext) /* 如果回收区与F2相邻接  */
    {
        freeBlock[indexLinkNext].startAddress = startAddress;
        freeBlock[indexLinkNext].size += size;
    }
    else if (tagLinkPre) /* 如果回收区与F1相邻接   */
    {
        freeBlock[indexLinkPre].size += size;
    }
    else /* 如果回收区与F1、F2都不邻接 */
    {
        for (i = 0; i < N; i++)
        {
            if (freeBlock[i].state == 0)
            {
                freeBlock[i].startAddress = startAddress;
                freeBlock[i].size = size;
                freeBlock[i].state = 1;
                break;
            }
        }
    }
}

/* --------------------------------------------------------------------------
 *                adjustFreeTable()函数
 *	  函数功能：对空闲区说明表中的空闲区进行调整
 *              使空闲区按起始地址从小到大排列，空表目放在后面
 *	  函数参数：无参数
 *	  函数返回值：无返回值
 * -------------------------------------------------------------------------- */
void adjustFreeTable()
{
    struct freePartition tmp;
    int numEmptyRecord = 0;
    int emptyRecord[N];
    int i, j;
    for (i = 0; i < N; i++)
    {
        emptyRecord[i] = -1;
    }
    /*统计空闲分区说明表中的空表目数，并在emptyRecord中记录空表目的编号*/
    for (i = 0, j = 0; i < N; i++)
    {
        if (freeBlock[i].state == 0)
        {
            numEmptyRecord++;
            emptyRecord[j++] = i;
        }
    }

    /* 逆序搜索空闲分区说明表中的空闲块登记项，并将其与空白表项交换 */
    for (i = N - 1, j = 0; i >= N - numEmptyRecord; i--)
    {
        if (freeBlock[i].state == 1)
        {
            // 把原本散落在空闲表中的空白项先换至空闲表尾部
            tmp.state = freeBlock[i].state;
            tmp.startAddress = freeBlock[i].startAddress;
            tmp.size = freeBlock[i].size;

            freeBlock[i].state = freeBlock[emptyRecord[j]].state;
            freeBlock[i].startAddress = freeBlock[emptyRecord[j]].startAddress;
            freeBlock[i].size = freeBlock[emptyRecord[j]].size;

            freeBlock[emptyRecord[j]].state = tmp.state;
            freeBlock[emptyRecord[j]].startAddress = tmp.startAddress;
            freeBlock[emptyRecord[j]].size = tmp.size;

            j++;
        }
    }

    /* 再把前N-numEmptyRecord个记录用冒泡排序法按地址从小到大的顺序排列 */
    for (i = 0; i < N - numEmptyRecord; i++)
    {
        for (j = 0; j < N - numEmptyRecord - 1 - i; j++)
        {
            if (freeBlock[j].startAddress > freeBlock[j + 1].startAddress)
            {
                tmp.startAddress = freeBlock[j].startAddress;
                tmp.size = freeBlock[j].size;
                tmp.state = freeBlock[j].state;

                freeBlock[j].startAddress = freeBlock[j + 1].startAddress;
                freeBlock[j].size = freeBlock[j + 1].size;
                freeBlock[j].state = freeBlock[j + 1].state;

                freeBlock[j + 1].startAddress = tmp.startAddress;
                freeBlock[j + 1].size = tmp.size;
                freeBlock[j + 1].state = tmp.state;
            }
        }
    }
}

/* --------------------------------------------------------------------------
 *                printFreeTable()函数
 *	  函数功能：打印空闲区说明表
 *	  函数参数：无参数
 *	  函数返回值：无返回值
 * -------------------------------------------------------------------------- */
void printFreeTable()
{
    int i;
    printf("\t\t| --------------------------------------------\t|\n");
    printf("\t\t| start\t\tsize\t\tstate\t\t|\n");
    printf("\t\t| --------------------------------------------\t|\n");

    for (i = 0; i < N; i++)
    {
        printf("\t\t| %3d\t\t%3d\t\t%3d\t\t|\n", freeBlock[i].startAddress, freeBlock[i].size, freeBlock[i].state);
        printf("\t\t| --------------------------------------------\t|\n");
    }
}

int main()
{
    char requestOrNot, ch;
    int i;
    int requestSize;
    int startAddr;
    int ret = 0;
    int releaseStartAddr, releaseSize;

    while (1)
    {
        printf("\nIs there any job request memory? y or n:");
        scanf("%c", &requestOrNot);
        ch = getchar();
        if (requestOrNot == 'y')
        {
            printf("Initially the free memory is as below:\n");
            adjustFreeTable();
            printFreeTable();
            printf("input memory size requested:");
            scanf("%d", &requestSize);
            getchar(); /* 吸收回车符 */

            startAddr = alloc(requestSize);
            if (startAddr == -1)
            {
                printf("memory allocation failed.Please wait!\n");
                ret = -1;
                continue;
            }
            else
            {
                printf("allocation successs!\n");
                printf("job's memory start address is:%d, memory size is %d\n", startAddr, requestSize);
                adjustFreeTable();
                printf("after allocation, the free memory is as below:\n");
                printFreeTable();
                printf("job is running.\n");
                printf("job is terminated.\n");
                for (i = 0; i < 1000; i++)
                    ;  // 延时
                printf("Please input the start address of the area to be released:\n");
                scanf("%d", &releaseStartAddr);
                if (releaseStartAddr < 0 || releaseStartAddr > 600)
                {
                    printf("Please check your releaseStartAddr!\n");
                }
                getchar();
                printf("Please input the size of the area to be released:\n");
                scanf("%d", &releaseSize);
                if (releaseSize > 100)
                {
                    printf("Please check your releaseStartAddr,which need < 100!\n");
                }
                getchar();
                setFree(releaseStartAddr, releaseSize);
                adjustFreeTable();
                printf("after free the memory, the free memory is as below:\n");
                printFreeTable();
            }
        }
        else
            break;
    }
END:
    return ret;
}
