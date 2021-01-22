//////#include<stdio.h>
//////#include<stdlib.h>
//////#include<conio.h>
//////#include<string.h>
//////#include <windows.h>
//////#include "time.h"
//////
//////#define SIZELIMIT 15 //每个变量限制在15字符
//////#define MAXNUM 500 //设定最大的车次数量
//////typedef struct  //定义一个班次信息的结构体CardBase，用于存储每列火车的具体信息
//////{
//////	char C_ID[SIZELIMIT];//班次
//////	char C_TIME[SIZELIMIT];//发车的时间
//////	char C_ANAME[SIZELIMIT];//发车站
//////	char C_BNAME[SIZELIMIT];//终点
//////	char C_USETIME[SIZELIMIT];//行车的时间
//////	char C_MAXNUM[SIZELIMIT];//额定的载量
//////	char C_REMAINNUM[SIZELIMIT];//余票的数量
//////}CardBase;
//////int cbNum = 0;//记录有的班次数
//////CardBase cBList[MAXNUM];//train list
//////
////////读取班次信息
//////void readcardbasefile()
//////{
//////	FILE* fp;
//////	//打开文件失败则创建文件
//////	if ((fp = fopen("./number.dat", "r")) == NULL)
//////	{
//////		//初次运行创建文件
//////		if ((fp = fopen("./number.dat", "w")) == NULL)
//////		{
//////			exit(0); //返回
//////		}
//////		else
//////		{
//////			fclose(fp);
//////		}
//////		return;
//////	}
//////	/*文件位置指针移动到文件末尾*/
//////	fseek(fp, 0, 2);//重定位文件内部位置指针
//////	/* int fseek( FILE *stream, long offset, int origin );
//////	第一个参数stream为文件指针
//////	第二个参数offset为偏移量，正数表示正向偏移，负数表示负向偏移
//////	第三个参数origin设定从文件的哪里开始偏移,可能取值为：SEEK_CUR、 SEEK_END 或 SEEK_SET
//////	SEEK_SET： 文件开头
//////	SEEK_CUR： 当前位置
//////	SEEK_END： 文件结尾
//////	其中SEEK_SET,SEEK_CUR和SEEK_END依次为0，1和2.
//////	简言之：
//////	fseek(fp,100L,0);把stream指针移动到离文件开头100字节处；
//////	fseek(fp,100L,1);把stream指针移动到离文件当前位置100字节处；
//////	fseek(fp,-100L,2);把stream指针退回到离文件结尾100字节处。 */
//////	if (ftell(fp) > 0)//文件不为空
//////	/*ftell函数用于得到文件位置指针当前位置相对于文件首的偏移字节数
//////	也就是得到文件所包含的字节数,如果大于0，则代表文件非空*/
//////	{
//////		//文件位置指针移动到文件开始
//////		rewind(fp);
//////		char buff[10] = { 0 };
//////		for (cbNum = 0; !feof(fp) && fread(&cBList[cbNum], sizeof(CardBase), 1, fp); cbNum++)
//////			/*对于feof函数，如果遇到文件结束，函数feof（fp）的值为非零值，否则为0。
//////			即如果文件结束，!feof(fp)为0，跳出循环
//////			对于fread函数，是从一个文件流中读数据，如果调用成功返回实际读取到的项个数，
//////			如果不成功或读到文件末尾返回 0*/
//////			fgets(buff, 10, fp);
//////		/*char *fgets(char *buf, int bufsize, FILE *stream);
//////		从文件结构体指针stream中读取数据，每次读取一行。读取的数据保存在buf指向的字符数组中，
//////		每次最多读取bufsize-1个字符（第bufsize个字符赋'\0'），如果文件中的该行，不足bufsize个字符，
//////		则读完该行就结束。如若该行（包括最后一个换行符）的字符数超过bufsize-1，则fgets只返回一个不完整的行，
//////		但是，缓冲区总是以NULL字符结尾，对fgets的下一次调用会继续读该行。函数成功将返回buf，失败或读到文件结尾返回NULL。
//////		因此我们不能直接通过fgets的返回值来判断函数是否是出错而终止的，应该借助feof函数或者ferror函数来判断。
//////		*/
//////		fclose(fp);
//////	}
//////	else
//////	{
//////		fclose(fp);
//////	}
//////	return;
//////}
////////保存班次信息
//////void writecardbasefile()
//////{
//////	int i;
//////	FILE* fp;
//////	if ((fp = fopen("./number.dat", "w")) == NULL)
//////	{
//////		printf("系统错误");
//////	}
//////	char buff[10] = { 0 };
//////	strcpy(buff, "\r\n");
//////	for (i = 0; i < cbNum; i++)
//////	{
//////		if (fwrite(&cBList[i], sizeof(CardBase), 1, fp) != 1)
//////		{
//////			printf("系统错误");
//////		}
//////		if (fwrite(buff, 2, 1, fp) != 1)
//////		{
//////			printf("系统错误");
//////		}
//////	}
//////	fclose(fp);
//////}
////////保存排序信息
//////void writesortfile()
//////{
//////	int i;
//////	FILE* fp;
//////	if ((fp = fopen("./sort.dat", "w")) == NULL)
//////	{
//////		printf("系统错误");
//////	}
//////	char buff[10] = { 0 };
//////	strcpy(buff, "\r\n");
//////	for (i = 0; i < cbNum; i++)
//////	{
//////		if (fwrite(&cBList[i], sizeof(CardBase), 1, fp) != 1)
//////		{
//////			printf("系统错误");
//////		}
//////		if (fwrite(buff, 2, 1, fp) != 1)
//////		{
//////			printf("系统错误");
//////		}
//////	}
//////	fclose(fp);
//////}
////////打印并输入后返回
//////void printReturn(char* info)
//////{
//////	printf("\n\n\t %s", info);
//////	fflush(stdin);//清空输入缓冲区，通常是为了确保不影响后面的数据读取.
//////	getchar();
//////}
////////输入信息
//////void setInfo(char pinfo[1024], char desinfo[])
//////{
//////	printf("\n\t%s:", pinfo);
//////	fflush(stdin);
//////	scanf("%s", desinfo);
//////}
////////系统初始化
//////void initsystem()
//////{
//////	readcardbasefile();
//////}
////////录入班次
//////void infoinput()
//////{
//////	setInfo("班次", cBList[cbNum].C_ID);
//////	setInfo("发车时间(24小时制)", cBList[cbNum].C_TIME);
//////	setInfo("起点站", cBList[cbNum].C_ANAME);
//////	setInfo("终点站", cBList[cbNum].C_BNAME);
//////	setInfo("行车时间", cBList[cbNum].C_USETIME);
//////	setInfo("额定载量", cBList[cbNum].C_MAXNUM);
//////	setInfo("余票数量", cBList[cbNum].C_REMAINNUM);
//////	cbNum++;
//////	writecardbasefile();
//////	printReturn("\n\t录入成功，回车键返回");
//////}
////////时间比较
//////int timecmp(char A[10])
//////{
//////	//将火车时间转成整数
//////	char tempa[10] = { 0 };
//////	int ta = 0;
//////	int i;
//////	for (i = 0; i < strlen(A); i++)
//////		if (A[i] != ':' && A[i] != '：') //冒号的中文输入和英文输入
//////		{
//////			tempa[ta] = A[i];
//////			ta++;
//////		}
//////	int atime = atoi(tempa);//把字符串变成整型
//////	//获取系统时间
//////	char tempb[10] = { 0 };
//////	time_t t = time(0);
//////	strftime(tempb, 10, "%H%M", localtime(&t));
//////	int btime = atoi(tempb);
//////	//比较
//////	if (atime <= btime) //已经发车
//////		return 1;
//////	if (((atime - btime <= 30) && (atime - btime > 5) && (atime / 100 == btime / 100)) || (((atime % 100 + (60 - btime % 100)) <= 30) && (atime % 100 + (60 - btime % 100)) > 5 && (atime / 100 - btime / 100 == 1))) //距发车半小时以内,停止退票,%表示取余
//////		return 2;
//////	if (((atime - btime <= 5) && (atime / 100 == btime / 100)) || ((atime % 100 + (60 - btime % 100) && (atime / 100 - btime / 100 == 1)) <= 5)) //距发车前五分钟内停止检票
//////		return 3;
//////	return 0; //可以办理购退票
//////}
////////浏览所有班次
//////void queryallinfo()
//////{
//////	printf("班次信息\n");
//////	printf("班次    发车时间    起点站    终点站    行车时间  额定载量  余票数量   状态\n");
//////	int i;
//////	for (i = 0; i < cbNum; i++)
//////	{
//////		char temp[20] = { 0 };
//////		strcpy(temp, "未发车");
//////		if (1 == timecmp(cBList[i].C_TIME))
//////			strcpy(temp, "已发车");
//////		if (2 == timecmp(cBList[i].C_TIME))
//////			strcpy(temp, "停止退票");
//////		if (3 == timecmp(cBList[i].C_TIME))
//////			strcpy(temp, "停止检票");
//////		printf("%-010s%-010s%-010s%-010s%-010s%-010s%-010s%s\n",
//////			cBList[i].C_ID, cBList[i].C_TIME,
//////			cBList[i].C_ANAME, cBList[i].C_BNAME, cBList[i].C_USETIME,
//////			cBList[i].C_MAXNUM, cBList[i].C_REMAINNUM, temp);
//////	}
//////	Sleep(5000);
//////	printReturn("\n\t回车键返回");
//////}
////////通过班次号查路线
//////void queryinfobyID()
//////{
//////	char ID[20] = { 0 };
//////	setInfo("输入班次号", ID);
//////	int i;
//////	for (i = 0; i < cbNum; i++)
//////	{
//////		if (strcmp(cBList[i].C_ID, ID) == 0)
//////		{
//////			printf("班次信息\n");
//////			printf("班次 发车时间 起点站 终点站 行车时间 额定载量 余票数量\n");
//////			printf("%-010s%-010s%-010s%-010s%-010s%-010s%-010s\n",
//////				cBList[i].C_ID, cBList[i].C_TIME,
//////				cBList[i].C_ANAME, cBList[i].C_BNAME, cBList[i].C_USETIME,
//////				cBList[i].C_MAXNUM, cBList[i].C_REMAINNUM);
//////			Sleep(5000);
//////			printReturn("\n\t回车键返回");
//////			return;
//////		}
//////	}
//////	printReturn("\n\t指定信息不存在，回车键返回");
//////}
////////通过终点站查路线
//////void queryinfobyBNAME()
//////{
//////	char Name[20] = { 0 };
//////	setInfo("输入终点站", Name);
//////	int i;
//////	for (i = 0; i < cbNum; i++)
//////	{
//////		if (strcmp(cBList[i].C_BNAME, Name) == 0)
//////		{
//////			printf("班次信息\n");
//////			printf("班次 发车时间 起点站 终点站 行车时间 额定载量 余票数量\n");
//////			printf("%-010s%-010s%-010s%-010s%-010s%-010s%-010s\n",
//////				cBList[i].C_ID, cBList[i].C_TIME,
//////				cBList[i].C_ANAME, cBList[i].C_BNAME, cBList[i].C_USETIME,
//////				cBList[i].C_MAXNUM, cBList[i].C_REMAINNUM);
//////			Sleep(5000);
//////			printReturn("\n\t回车键返回");
//////			return;
//////		}
//////	}
//////	printReturn("\n\t指定信息不存在，回车键返回");
//////}
////////排序按余票数量排序保存
//////void sortSave()
//////{
//////	//冒泡排序
//////	int i, j;
//////	for (i = 0; i < cbNum; i++)
//////		for (j = 0; j < cbNum - i - 1; j++)
//////		{
//////			if (atoi(cBList[j].C_REMAINNUM) < atoi(cBList[j + 1].C_REMAINNUM))
//////			{
//////				CardBase temp = cBList[j];
//////				cBList[j] = cBList[j + 1];
//////				cBList[j + 1] = temp;
//////			}
//////		}
//////	//打印排序结果
//////	queryallinfo();
//////	//保存排序结果
//////	writesortfile();
//////}
////////售票
//////void sale()
//////{
//////	char ID[20] = { 0 };
//////	setInfo("输入班次号", ID);
//////	int i;
//////	for (i = 0; i < cbNum; i++)
//////	{
//////		if (strcmp(cBList[i].C_ID, ID) == 0)
//////		{
//////			if (cBList[i].C_REMAINNUM == 0)
//////			{
//////				printReturn("\n\t余票不足，回车返回");
//////				return;
//////			}
//////			//余票减少
//////			int temp = atoi(cBList[i].C_REMAINNUM) - 1;
//////			if (temp < 0) temp = 0;//确保余票不为负数
//////			_itoa(temp, cBList[i].C_REMAINNUM, 10);
//////			//保存到文件
//////			writecardbasefile();
//////			printf("班次信息\n");
//////			printf("班次 发车时间 起点站 终点站 行车时间 额定载量 余票数量\n");
//////			printf("%-010s%-010s%-010s%-010s%-010s%-010s%-010s\n",
//////				cBList[i].C_ID, cBList[i].C_TIME,
//////				cBList[i].C_ANAME, cBList[i].C_BNAME, cBList[i].C_USETIME,
//////				cBList[i].C_MAXNUM, cBList[i].C_REMAINNUM);
//////			printReturn("\n\t售票成功，回车返回");
//////			return;
//////		}
//////	}
//////	printReturn("\n\t指定班次不存在，回车键返回");
//////}
////////退票
//////void back()
//////{
//////	char ID[20] = { 0 };
//////	setInfo("输入班次号", ID);
//////	int i;
//////	for (i = 0; i < cbNum; i++)
//////	{
//////		if (strcmp(cBList[i].C_ID, ID) == 0)
//////		{
//////			//余票增加
//////			int temp = atoi(cBList[i].C_REMAINNUM) + 1;
//////			_itoa(temp, cBList[i].C_REMAINNUM, 10);
//////			//保存到文件
//////			writecardbasefile();
//////			printf("班次信息\n");
//////			printf("班次 发车时间 起点站 终点站 行车时间 额定载量 余票数量\n");
//////			printf("%-010s%-010s%-010s%-010s%-010s%-010s%-010s\n",
//////				cBList[i].C_ID, cBList[i].C_TIME,
//////				cBList[i].C_ANAME, cBList[i].C_BNAME, cBList[i].C_USETIME,
//////				cBList[i].C_MAXNUM, cBList[i].C_REMAINNUM);
//////			printReturn("\n\t\t退票成功，回车返回");
//////			return;
//////		}
//////	}
//////	printReturn("\n\t指定班次不存在，回车键返回");
//////}
////////更新车次信息
//////void update()
//////{
//////	int flag;
//////	char ID[20] = { 0 };
//////	setInfo("请输入要删除或修改的车次（请确认车次输入正确）", ID);
//////	do
//////	{
//////		printf("\n\t按下列提示更新车次信息（退出修改请按0）:");
//////		printf("\n\t1：删除车次;2：修改发车时间;3：修改起点站;4：修改终点站;");
//////		printf("\n\t5：修改行车时间;6：修改额定载量;7：修改余票数量;0:退出修改;");
//////		printf("\n\t请选择：");
//////		scanf("%d", &flag);
//////		int i;
//////		CardBase temp;
//////		for (i = 0; i < cbNum; i++)
//////		{
//////			if (strcmp(cBList[i].C_ID, ID) == 0)
//////			{
//////				if (flag == 1) //删除车次信息
//////				{
//////					int j;
//////					for (j = i; j < cbNum; j++)
//////						cBList[j] = cBList[j + 1];//后面的数据覆盖前面的数据
//////					cbNum--;//车次数减一
//////					//保存到文件
//////					writecardbasefile();
//////				}
//////				if (flag == 2) //修改发车时间
//////				{
//////					setInfo("请输入新的发车时间", cBList[i].C_TIME);
//////					temp = cBList[i];
//////					//保存到文件
//////					writecardbasefile();
//////				}
//////				if (flag == 3) //修改起点站
//////				{
//////					setInfo("请输入新的起点站", cBList[i].C_ANAME);
//////					temp = cBList[i];
//////					//保存到文件
//////					writecardbasefile();
//////				}
//////				if (flag == 4) //修改终点站
//////				{
//////					setInfo("请输入新的终点站", cBList[i].C_BNAME);
//////					temp = cBList[i];
//////					//保存到文件
//////					writecardbasefile();
//////				}
//////				if (flag == 5) //修改行车时间
//////				{
//////					setInfo("请输入新的行车时间", cBList[i].C_USETIME);
//////					temp = cBList[i];
//////					//保存到文件
//////					writecardbasefile();
//////				}
//////				if (flag == 6) //修改额定载量
//////				{
//////					setInfo("请输入新的额定载量", cBList[i].C_MAXNUM);
//////					temp = cBList[i];
//////					//保存到文件
//////					writecardbasefile();
//////				}
//////				if (flag == 7) //修改余票数量
//////				{
//////					setInfo("请输入新的余票数量", cBList[i].C_REMAINNUM);
//////					temp = cBList[i];
//////					//保存到文件
//////					writecardbasefile();
//////				}
//////			}
//////		}
//////		//更改完成后显示更新后的班次信息,如果是删除车次则不显示,即flag不等于0的时候显示更新信息
//////		if (flag != 1 && flag != 0)
//////		{
//////			printf("更新后的班次信息\n");
//////			printf("班次       发车时间        起点站        终点站        行车时间        额定载量        余票数量\n");
//////			printf("%-010s%-010s%-010s%-010s%-010s%-010s%-010s\n",
//////				temp.C_ID, temp.C_TIME,
//////				temp.C_ANAME, temp.C_BNAME, temp.C_USETIME,
//////				temp.C_MAXNUM, temp.C_REMAINNUM);
//////		}
//////	} while (flag);
//////	printReturn("\n\t完成车次信息更新，回车键返回");
//////}
//////void mainmenu()
//////{
//////	while (1)
//////	{
//////		char select;
//////		do {
//////			system("cls");//清屏
//////			printf("\n\t════■□■□=============═══╮");
//////			printf("\n\t│ 简易火车售票系统 │");
//////			printf("\n\t╰═══■□■□══════════╯");
//////			printf("\n\t ┌────────────────┐");
//////			printf("\n\t │1.录入班次 │");
//////			printf("\n\t │2.浏览所有班次 │");
//////			printf("\n\t │3.通过班次号查路线 │");
//////			printf("\n\t │4.通过终点站查路线 │");
//////			printf("\n\t │5.排序保存 │");
//////			printf("\n\t │6.售票 │");
//////			printf("\n\t │7.退票 │");
//////			printf("\n\t │8.更新车次信息 │");
//////			printf("\n\t │0.exit │");
//////			printf("\n\t └────────────────┘");
//////			printf("\n\t 请选择：");
//////			fflush(stdin);//清空输入缓冲区，通常是为了确保不影响后面的数据读取.
//////			select = getchar();//等待用户输入数据
//////		} while (select < '0' || select>'8');
//////		switch (select)
//////		{
//////		case '0':exit(0); break;
//////		case '1':infoinput(); break;
//////		case '2':queryallinfo(); break;
//////		case '3':queryinfobyID(); break;
//////		case '4':queryinfobyBNAME(); break;
//////		case '5':sortSave(); break;
//////		case '6':sale(); break;
//////		case '7':back(); break;
//////		case '8':update(); break;
//////		}
//////	}
//////}
////////主函数
//////int main()
//////{
//////	initsystem(); //系统初始化
//////	while (1)
//////	{
//////		mainmenu();
//////	}
//////}
//////////
//////////
//////////
//////////
//////////
////////
////////
////////#include "stdio.h"    
////////#include "string.h"
////////#include "ctype.h"      
////////#include "stdlib.h"   
////////#include "io.h"  
////////#include "math.h"  
////////#include "time.h"
////////
////////#define OK 1
////////#define ERROR 0
////////#define TRUE 1
////////#define FALSE 0
////////
////////#define MAXSIZE 20 /* 存储空间初始分配量 */
////////
////////typedef int Status;/* Status是函数的类型,其值是函数结果状态代码，如OK等 */
////////typedef int ElemType;/* ElemType类型根据实际情况而定，这里假设为int */
////////
////////
////////Status visit(ElemType c)
////////{
////////    printf("%d ", c);
////////    return OK;
////////}
////////
////////typedef struct Node
////////{
////////    ElemType data;
////////    struct Node* next;
////////}Node;
////////typedef struct Node* LinkList; /* 定义LinkList *///地址 == 地址
////////
/////////* 初始化顺序线性表 */
////////Status InitList(Node** L)
////////{
////////    L = (Node*)malloc(sizeof(Node)); /* 产生头结点,并使L指向此头结点 */
////////    if (!(L)) /* 存储分配失败 */
////////        return ERROR;
////////    (L)->next = NULL; /* 指针域为空 */
////////
////////    return OK;
////////}
////////
/////////* 初始条件：顺序线性表L已存在。操作结果：若L为空表，则返回TRUE，否则返回FALSE */
////////Status ListEmpty(LinkList L)
////////{
////////    if (L->next)
////////        return FALSE;
////////    else
////////        return TRUE;
////////}
////////
/////////* 初始条件：顺序线性表L已存在。操作结果：将L重置为空表 */
////////Status ClearList(LinkList* L)
////////{
////////    LinkList p, q;
////////    p = (*L)->next;           /*  p指向第一个结点 */
////////    while (p)                /*  没到表尾 */
////////    {
////////        q = p->next;
////////        free(p);
////////        p = q;
////////    }
////////    (*L)->next = NULL;        /* 头结点指针域为空 */
////////    return OK;
////////}
////////
/////////* 初始条件：顺序线性表L已存在。操作结果：返回L中数据元素个数 */
////////int ListLength(LinkList L)
////////{
////////    int i = 0;
////////    LinkList p = L->next; /* p指向第一个结点 */
////////    while (p)
////////    {
////////        i++;
////////        p = p->next;
////////    }
////////    return i;
////////}
////////
/////////* 初始条件：顺序线性表L已存在，1≤i≤ListLength(L) */
/////////* 操作结果：用e返回L中第i个数据元素的值 */
////////Status GetElem(LinkList L, int i, ElemType* e)
////////{
////////    int j;
////////    LinkList p;		/* 声明一结点p */
////////    p = L->next;		/* 让p指向链表L的第一个结点 */
////////    j = 1;		/*  j为计数器 */
////////    while (p && j < i)  /* p不为空或者计数器j还没有等于i时，循环继续 */
////////    {
////////        p = p->next;  /* 让p指向下一个结点 */
////////        ++j;
////////    }
////////    if (!p || j > i)
////////        return ERROR;  /*  第i个元素不存在 */
////////    *e = p->data;   /*  取第i个元素的数据 */
////////    return OK;
////////}
////////
/////////* 初始条件：顺序线性表L已存在 */
/////////* 操作结果：返回L中第1个与e满足关系的数据元素的位序。 */
/////////* 若这样的数据元素不存在，则返回值为0 */
////////int LocateElem(LinkList L, ElemType e)
////////{
////////    int i = 0;
////////    LinkList p = L->next;
////////    while (p)
////////    {
////////        i++;
////////        if (p->data == e) /* 找到这样的数据元素 */
////////            return i;
////////        p = p->next;
////////    }
////////
////////    return 0;
////////}
////////
////////
/////////* 初始条件：顺序线性表L已存在,1≤i≤ListLength(L)， */
/////////* 操作结果：在L中第i个位置之前插入新的数据元素e，L的长度加1 */
////////Status ListInsert(LinkList* L, int i, ElemType e)
////////{
////////    int j;
////////    LinkList p, s;
////////    p = *L;
////////    j = 1;
////////    while (p && j < i)     /* 寻找第i个结点 */
////////    {
////////        p = p->next;
////////        ++j;
////////    }
////////    if (!p || j > i)
////////        return ERROR;   /* 第i个元素不存在 */
////////    s = (LinkList)malloc(sizeof(Node));  /*  生成新结点(C语言标准函数) */
////////    s->data = e;
////////    s->next = p->next;      /* 将p的后继结点赋值给s的后继  */
////////    p->next = s;          /* 将s赋值给p的后继 */
////////    return OK;
////////}
////////
/////////* 初始条件：顺序线性表L已存在，1≤i≤ListLength(L) */
/////////* 操作结果：删除L的第i个数据元素，并用e返回其值，L的长度减1 */
////////Status ListDelete(LinkList* L, int i, ElemType* e)
////////{
////////    int j;
////////    LinkList p, q;
////////    p = *L;
////////    j = 1;
////////    while (p->next && j < i)	/* 遍历寻找第i个元素 */
////////    {
////////        p = p->next;
////////        ++j;
////////    }
////////    if (!(p->next) || j > i)
////////        return ERROR;           /* 第i个元素不存在 */
////////    q = p->next;
////////    p->next = q->next;			/* 将q的后继赋值给p的后继 */
////////    *e = q->data;               /* 将q结点中的数据给e */
////////    free(q);                    /* 让系统回收此结点，释放内存 */
////////    return OK;
////////}
////////
/////////* 初始条件：顺序线性表L已存在 */
/////////* 操作结果：依次对L的每个数据元素输出 */
////////Status ListTraverse(LinkList L)
////////{
////////    LinkList p = L->next;
////////    while (p)
////////    {
////////        visit(p->data);
////////        p = p->next;
////////    }
////////    printf("\n");
////////    return OK;
////////}
////////
/////////*  随机产生n个元素的值，建立带表头结点的单链线性表L（头插法） */
////////void CreateListHead(LinkList* L, int n)
////////{
////////    LinkList p;
////////    int i;
////////    srand(time(0));                         /* 初始化随机数种子 */
////////    *L = (LinkList)malloc(sizeof(Node));
////////    (*L)->next = NULL;                      /*  先建立一个带头结点的单链表 */
////////    for (i = 0; i < n; i++)
////////    {
////////        p = (LinkList)malloc(sizeof(Node)); /*  生成新结点 */
////////        p->data = rand() % 100 + 1;             /*  随机生成100以内的数字 */
////////        p->next = (*L)->next;
////////        (*L)->next = p;						/*  插入到表头 */
////////    }
////////}
////////
/////////*  随机产生n个元素的值，建立带表头结点的单链线性表L（尾插法） */
////////void CreateListTail(LinkList* L, int n)
////////{
////////    LinkList p, r;
////////    int i;
////////    srand(time(0));                      /* 初始化随机数种子 */
////////    *L = (LinkList)malloc(sizeof(Node)); /* L为整个线性表 */
////////    r = *L;                                /* r为指向尾部的结点 */
////////    for (i = 0; i < n; i++)
////////    {
////////        p = (Node*)malloc(sizeof(Node)); /*  生成新结点 */
////////        p->data = rand() % 100 + 1;           /*  随机生成100以内的数字 */
////////        r->next = p;                        /* 将表尾终端结点的指针指向新结点 */
////////        r = p;                            /* 将当前的新结点定义为表尾终端结点 */
////////    }
////////    r->next = NULL;                       /* 表示当前链表结束 */
////////}
////////
////////int main()
////////{
////////    LinkList L;
////////    Node* L;
////////    ElemType e;
////////    Status i;
////////    int j, k;
////////    i = InitList(&L);
////////    printf("初始化L后：ListLength(L)=%d\n", ListLength(L));
////////    for (j = 1; j <= 5; j++)
////////        i = ListInsert(&L, 1, j);
////////    printf("在L的表头依次插入1～5后：L.data=");
////////    ListTraverse(L);
////////
////////    printf("ListLength(L)=%d \n", ListLength(L));
////////    i = ListEmpty(L);
////////    printf("L是否空：i=%d(1:是 0:否)\n", i);
////////
////////    i = ClearList(&L);
////////    printf("清空L后：ListLength(L)=%d\n", ListLength(L));
////////    i = ListEmpty(L);
////////    printf("L是否空：i=%d(1:是 0:否)\n", i);
////////
////////    for (j = 1; j <= 10; j++)
////////        ListInsert(&L, j, j);
////////    printf("在L的表尾依次插入1～10后：L.data=");
////////    ListTraverse(L);
////////
////////    printf("ListLength(L)=%d \n", ListLength(L));
////////
////////    ListInsert(&L, 1, 0);
////////    printf("在L的表头插入0后：L.data=");
////////    ListTraverse(L);
////////    printf("ListLength(L)=%d \n", ListLength(L));
////////
////////    GetElem(L, 5, &e);
////////    printf("第5个元素的值为：%d\n", e);
////////    for (j = 3; j <= 4; j++)
////////    {
////////        k = LocateElem(L, j);
////////        if (k)
////////            printf("第%d个元素的值为%d\n", k, j);
////////        else
////////            printf("没有值为%d的元素\n", j);
////////    }
////////
////////
////////    k = ListLength(L); /* k为表长 */
////////    for (j = k + 1; j >= k; j--)
////////    {
////////        i = ListDelete(&L, j, &e); /* 删除第j个数据 */
////////        if (i == ERROR)
////////            printf("删除第%d个数据失败\n", j);
////////        else
////////            printf("删除第%d个的元素值为：%d\n", j, e);
////////    }
////////    printf("依次输出L的元素：");
////////    ListTraverse(L);
////////
////////    j = 5;
////////    ListDelete(&L, j, &e); /* 删除第5个数据 */
////////    printf("删除第%d个的元素值为：%d\n", j, e);
////////
////////    printf("依次输出L的元素：");
////////    ListTraverse(L);
////////
////////    i = ClearList(&L);
////////    printf("\n清空L后：ListLength(L)=%d\n", ListLength(L));
////////    CreateListHead(&L, 20);
////////    printf("整体创建L的元素(头插法)：");
////////    ListTraverse(L);
////////
////////    i = ClearList(&L);
////////    printf("\n删除L后：ListLength(L)=%d\n", ListLength(L));
////////    CreateListTail(&L, 20);
////////    printf("整体创建L的元素(尾插法)：");
////////    ListTraverse(L);
////////
////////
////////    return 0;
////////}
////////
////////
//////#include<stdio.h>
//////int abc(int** p)
//////{
//////	**p = 5;
//////		return 0;
//////}
////#include <stdio.h>
////#include <string.h>
////#pragma warning(disable:4996) 
////struct Books
////{
////	char  title[50];
////	char  author[50];
////	char  subject[100];
////	int   book_id;
////};
////int BinaryRec(int arr[], int left, int right, int key)
////{
////
////	int mid = -1;
////
////	if (left <= right)
////	{
////		mid = (left + right) / 2;
////
////		if (arr[mid] == key)
////			return mid;
////		else if (arr[mid] < key)
////			return BinaryRec(arr, mid + 1, right, key);
////		else
////			return BinaryRec(arr, left, mid - 1, key);
////	}
////
////	return mid;
////}
////
////int BinFind(int a[], int size,int target) {
////	int left = 0;
////	int right = size - 1;
////	while (left <= right) {
////		int mid = (left + right) / 2;
////		if (a[mid] == target) {
////			return mid;
////		}
////		else if (a[mid] < target) {
////			left = mid + 1;
////		}
////		else {
////			right = mid - 1;
////		}
////	}
////	return -1;
////}
////
////char* str() {
////	char s[10];
////	strcpy(s, "held\n");
////	char* ss = s;
////	return ss;
////}
////int bin(int a[], int left, int right, int tar) {
////	int mid = -1;
////	if (left <= right) {
////		mid = (left + right) / 2;
////		if (a[mid] == tar) {
////			return mid;
////		}
////		else if (a[mid] > tar) {
////			right = mid - 1;
////			return bin(a, left, right, tar);
////		}
////		else {
////			left = mid + 1;
////			return bin(a, left, right, tar);
////		}
////	}
////	return mid;
////
////
////
////}
////void Swap(int* a, int* b) {
////	int temp;
////	temp = *a;
////	*a = *b;
////	*b = temp;
////}
//////冒泡排序
////void BubbleSort(int a[], int n) {//两个循环，从小到大
////	int i, j;
////	for (i = 0; i < n; i++) {//i 代表末尾已经排好序值得个数
////		int flag = 0;
////		for (j = 0; j < n- i-1; j++) {
////			if (a[j + 1] < a[j]) {
////				Swap(&a[j + 1], &a[j]);
////				flag = 1;
////			}
////		}
////		if (flag == 0)
////			return;
////	}
////}
////
////
////void Insertsort(int a[], int n) {
////	int i,temp,j;
////	for (i = 1; i < n; i++) {//待排序
////		if (a[i] < a[i - 1]) {//和已排序最大值进行比较
////			temp = a[i];
////			for (j = i - 1; j >= 0 && a[j] > temp; j--) {//和剩余的比较
////				a[j + 1] = a[j];
////			}
////			a[j + 1] = temp;
////		}
////	}
////}
////
/////* 函数声明 */
////
////void InsertSort2(int a[], int n)
////{
////	for (int i = 1; i < n; i++)
////		for (int j = i - 1; j >= 0 && a[j] > a[j + 1]; j--)
////			Swap(&a[j], &a[j + 1]);
////}
/////*
////没有移植的教程和资料，wireshark 的Makefile中没有指定Gcc,需要一些Python和Perl的环境
////
////*/
////
////
////void QuickSort(int a[], int left, int right) {
////	if (left < right) {//condition
////		int i = left, j = right;
////		int base = a[left];             //基准,left empty
////		while (i < j) {
////			while (i < j && a[j] >= base)    //从右往左找小于base的元素
////				j--;
////			if (i < j)
////			{
////				a[i] == a[j];
////				//i++;
////			}
////			while (i < j && a[i] < base) {
////				i++;
////			}
////			if (i < j)
////			{
////				a[j] = a[i];
////				//j--;
////			}
////		}
////		a[i] = base;
////		QuickSort(a, left, i - 1);      //递归调用，分治
////		QuickSort(a, i + 1, right);
////	}
////}
////
////void printBook(struct Books* book);
////
////
////char* reverseLeftWords(char* s, int n) {
////	if (s == NULL || n == 0) {
////		return "";
////	}
////	int len = strlen(s);
////	int len2 = len;
////	char* res = (char*)malloc(sizeof(char) * len + 1 );
////	int i, j = 0;
////
////	for (i = n - 1; i >= 0; i--) {
////		res[len - 1] = s[i];
////		len--;
////	}
////	for (i = n; i < len2; i++) {
////		res[j] = s[i];
////		j++;
////	}
////	res[len2] = '\0';
////	return res;
////}
////
////void getMac(char* str,int i) {
////	int j = 0,start = 0,end =0,position = 0;
////	char s1[20] = { 0 };
////	char s2[20] = { 0 };
////	//int flag = 0;
////	for (j = i; j >= 0; j--) {
////		if (str[i] == ' ')
////		{
////			break;
////		}
////	}
////	start = j;
////	end = i;
////	for (start; start <= end; start++) {
////		s1[position] = str[start];
////		position++;
////	}
////
////
////	for (j = i;; j++) {
////		if (str[i] == ' ')
////		{
////			break;
////		}
////	}
////	start = i;
////	end = j;
////	for (start; start <= end; start++) {
////		s2[position] = str[start];
////		position++;
////	}
////	printf("s1 : %s  s2: %s\n", s1, s2);
//// }
////
////int main()
////{
////
////	char snwin[10] = "abcdefghi";
////	char* res = reverseLeftWords(snwin, 5);
////	printf("res : %s\n", res);
////	int arr[30] = { 1,2,3,4,5,6,0 };
////	QuickSort(arr, 0,6);
////	//BubbleSort(arr, 7);
////	printf("arr : %d\n", arr[6]);
////	int sn = bin(arr, 0, 5, 6);
////	int sn2 = BinFind(arr, 6, 6);
////	printf("sn %d  %d\n", sn,sn2);
////	char str[][10] = { "hell","world" };
////
////	struct Books Book1;        /* 声明 Book1，类型为 Books */
////	struct Books Book2;        /* 声明 Book2，类型为 Books */
////	struct Books* Book3[2];
////	Book3[0] = &Book1;
////	/* Book1 详述 */
////	strcpy(Book1.title, "C Programming");
////	strcpy(Book1.author, "Nuha Ali");
////	strcpy(Book1.subject, "C Programming Tutorial");
////	Book1.book_id = 6495407;
////
////	/* Book2 详述 */
////	//strcpy(Book2.title, "Telecom Billing");
////	//strcpy(Book2.author, "Zara Ali");
////	//strcpy(Book2.subject, "Telecom Billing Tutorial");
////	//Book2.book_id = 6495700;
////	Book2 = Book1;
////	/* 通过传 Book1 的地址来输出 Book1 信息 */
////	printBook(&Book1);
////
////	/* 通过传 Book2 的地址来输出 Book2 信息 */
////	printBook(&Book2);
////	printf("Book book_id : %d\n", (*Book3[0]).book_id);
////	//char* s = str();
////	//printf("s %s\n", s);
////	return 0;
////}
////void printBook(struct Books* book)
////{
////	printf("Book title : %s\n", book->title);
////	printf("Book author : %s\n", book->author);
////	printf("Book subject : %s\n", book->subject);
////	printf("Book book_id : %d\n", book->book_id);
////}
////
////
//#include <stdio.h>
//#include <string.h>
//
//char temp_str[30];    // 临时子串
//
//void ReadStrUnit(char* str, char* temp_str, int idx, int len)  // 从母串中获取与子串长度相等的临时子串
//{
//    int index = 0;
//    for (index; index < len; index++)
//    {
//        temp_str[index] = str[idx + index];
//    }
//    temp_str[index] = '\0';
//}
//
//int GetSubStrPos(char* str1, char* str2)
//{
//    int idx = 0;
//    int len1 = strlen(str1);
//    int len2 = strlen(str2);
//
//    if (len1 < len2)
//    {
//        printf("error 1 \n"); // 子串比母串长
//        return -1;
//    }
//
//    while (1)
//    {
//        ReadStrUnit(str1, temp_str, idx, len2);    // 不断获取的从 母串的 idx 位置处更新临时子串
//        if (strcmp(str2, temp_str) == 0)break;      // 若临时子串和子串一致，结束循环
//        idx++;                                  // 改变从母串中取临时子串的位置
//        if (idx >= len1)return -1;                 // 若 idx 已经超出母串长度，说明母串不包含该子串
//    }
//
//    return idx;    // 返回子串第一个字符在母串中的位置
//}
//void getMac(char* str, int i) {
//    int j = 0, start = 0, end = 0, position = 0;
//    char s1[20] = { 0 };
//    char s2[20] = { 0 };
//    //int flag = 0;
//    for (j = i - 1; j >= 0; j--) {
//        if (str[j] == ' ')
//        {
//            break;
//        }
//    }
//    start = j;
//    end = i;
//    for (start; start <= end; start++) {
//        s1[position] = str[start];
//        position++;
//    }
//
//
//    for (j = i + 5; j < 100; j++) {
//        if (str[j] == ' ')
//        {
//            break;
//        }
//    }
//    start = i + 5;
//    end = j;
//    position = 0;
//    for (start; start <= end; start++) {
//        s2[position] = str[start];
//        position++;
//    }
//    printf("s1 : %s  s2: %s\n", s1, s2);
//}
//
//int main()
//{
//
//    char* str1 = "58 109.999592027 172.31.237.11 → 192.168.153.145 TCP 60 5001 → 54860 [ACK] Seq=16 Ack=793 Win=64240 Len=0";
//    char* str2 = " → ";
//    int i = 0, j = 0;
//    i = GetSubStrPos(str1, str2);
//    if (i < 0)
//    {
//        printf("not found\n");
//    }
//    else
//    {
//        printf("i = %d\n", i);
//    }
//    getMac(str1, i);
//    return 0;
//}
//
//
//
//
//
//
//
//
//
//
//
//
//
//#include<stdio.h>
//#include<stdlib.h>
//#include<pthread.h>
//#include<string.h>
//FILE *fp = NULL;
//char data[300] = {'0'};
//void *packetcapture(void)
//{
//	fp = popen("tcpdump -i eth5", "r");
//	if (fp == NULL)
//	{
//		printf("popen error!\n");
//		return NULL;
//	}
//}
//
//void *printmessage(void)
//{
//	while (fgets(data, sizeof(data), fp) != NULL)
//	{
//		int len = strlen(data);
//		printf("len---->%d  ",len);
//		printf("get---->%s  ",data);
//		pclose(fp);
//	}
//}
//int main()
//{
//	
//    fp = popen("./tcpdump -i eth5", "r");
//    if (fp == NULL)
//    {
//        printf("popen error!\n");
//        return NULL;
//    }
//    while (fgets(data, sizeof(data), fp) != NULL)
//    {
//        int len = strlen(data);
//        printf("len---->%d  ", len);
//        printf("get---->%s  ", data);
//
//    }
//	pclose(fp);
//	return 0;
//}
//

#include<stdio.h>
#include<math.h>
#include<string.h>

void checkout_ssid(char* data);
int hexstring_to_int(char* s);

char temp_str[30] = { '0' };
void ReadStrUnit(char* str, char* temp_str, int idx, int len)  // 从母串中获取与子串长度相等的临时子串
{
	int index = 0;
	for (index; index < len; index++)
	{
		temp_str[index] = str[idx + index];
	}
	temp_str[index] = '\0';
}

int GetSubStrPos(char* str1, char* str2)
{
	int idx = 0;
	int len1 = strlen(str1);
	int len2 = strlen(str2);

	if (len1 < len2)
	{
		printf("error 1 \n"); // 子串比母串长
		return -1;
	}

	while (1)
	{
		ReadStrUnit(str1, temp_str, idx, len2);    // 不断获取的从 母串的 idx 位置处更新临时子串
		if (strcmp(str2, temp_str) == 0)break;      // 若临时子串和子串一致，结束循环
		idx++;                                  // 改变从母串中取临时子串的位置
		if (idx >= len1)return -1;                 // 若 idx 已经超出母串长度，说明母串不包含该子串
	}

	return idx;    // 返回子串第一个字符在母串中的位置
}

int get_channel(char* data)
{
	int index_start = -1;
	int index_stop = -1;
	int i = 0;
	int count = 0;
	char* data2 = data;
	char num[5] = "";
	index_start = GetSubStrPos(data, "tsft");
	index_start = index_start + 5;

	index_stop = GetSubStrPos(data2, "MHz");
	index_stop = index_stop - 1;
	printf("Channel start : %d, stop : %d\n", index_start, index_stop);
	for (i = index_start; i < index_stop; i++) {
		num[count] = data[i];
		count++;

	}
	i = atoi(num);
	i = (i - 5170) / 10 * 2 + 34;
	printf("Current packeture Channel : %d\n", i);
	return i;
}


int main() 
{
	char ssid[10] = "\0";
	ssid[0] = 0x53;
	ssid[1] = 0x5a;
	ssid[2] = 0x55;
	
	printf("ssid : %s\n", ssid);


	char s[5][3] = { "53","5a","55" };

	char res[10] = "";
	res[0] =  hexstring_to_int(s[0]);
	res[1] = hexstring_to_int(s[1]);
	res[2] = hexstring_to_int(s[2]);
 	printf("res : %c  %c  %c\n", res[0],res[1],res[2]);
	printf("res : %s\n", res);



	char *test1 = "-3:-53:-1.993667m signal 0dBm noise antenna 0 6.5 Mb/s MCS 0 20 MHz long GI mixed BCC FEC 0us BSSID:cc:08:fb:a6:6e:06 (oui Unknown) DA:00:90:4c:1b:00:01 (oui Unknown) SA:cc:08:fb:a6:6e:06 (oui Unknown) Probe Response (TP-LINK_6E04) [6.0* 9.0 12.0* 18.0 24.0* 36.0 48.0 54.0 Mbit] CH: 153, PRIVACY";
	char *test2 = "-3:-52:-9CC FEC 0us BSSID:1c:ab:34:72:69:c0 (oui Unknown) DA:Broadcast SA:1c:ab:34:72:69:c0 (oui Unknown) Beacon (SZU_WLAN) [18.0 24.0* 36.0 48.0 54.0 Mbit] ESS";
	char *test3 = "-7:-50:-57.817391 4294958764us tsft 5220 MHz 11n 0dBm signal 0dBm noise antenna 0 6.5 Mb/s MCS 0 20 MHz long GI greenfield BCC FEC 160us RA:34:96:72:3c:fa:56 (oui Unknown) TA:0c:dd:24:3b:a9:0c (oui Unknown) Request-To-Send";
	char* test4 = "-7:-50:-57.863098 4294938660us tsft 5240 MHz 11n 0dBm signal 0dBm noise antenna 0 6.5 Mb/s MCS 0 20 MHz long GI mixed BCC FEC 60us BSSID:34:96:72:3c:fa:56 (oui Unknown) DA:00:90:4c:1b:00:01 (oui Unknown) SA:34:96:72:3c:fa:56 (oui Unknown) Probe Res\
ponse (RF_5G_N301) [6.0* 9.0 12.0* 18.0 24.0* 36.0 48.0 54.0 Mbit] CH: 48";
	checkout_ssid(test1);
	checkout_ssid(test2);
	
	char nn[20] = "";
	char ns[40] = "ss";
	memset(ns, 0, 40);
	if (strcmp("", "") == 0 && strcmp(nn, ns) == 0) {
	
		printf("++++++++\n");
	}
	get_channel(test3);
	get_channel(test4);
	return 0;
}

int hexstring_to_int(char* s)
{
	int t = strlen(s);
	int sum = 0;
	for (int i = 0; i < t; i++) {
		if (s[i] >= 'a' && s[i] <= 'z')
			s[i] = s[i] - 'a' + 10 + '0';
		sum += ((s[i] - '0') * (pow(16, t - 1 - i)));
	}
	return sum;
}



void checkout_ssid(char* data)
{

	char* ret1 = NULL;
	char* ret2 = NULL;
	char ssid[100] = "";
	char mac[20] = "";
	int i = 0;
	int count = 0;
	int flag = 0;

	int index1 = 0;
	int index2 = 0;
	char tag1[30] = { "Probe Response" };
	char tag2[10] = {"Beacon"};
	ret1 = strstr(data, tag1);
	ret2 = strstr(data, "Beacon");



	if (ret1) {
		flag = 1;
		index1 = GetSubStrPos(ret1, "(");
		index2 = GetSubStrPos(ret1, ")");
		for (i = index1 + 1; i < index2; i++) {
			ssid[count] = ret1[i];
			count++;
		}
		printf("Probe Response : mac __>  %s\n", ssid);
	}

	if (ret2) {
		flag = 1;
		index1 = GetSubStrPos(ret2, "(");
		index2 = GetSubStrPos(ret2, ")");
		for (i = index1 + 1; i < index2; i++) {
			ssid[count] = ret2[i];
			count++;
		}
		printf("Probe Response : ssid __>  %s\n", ssid);
	}

	if (flag) {

		count = 0;
		index1 = GetSubStrPos(data, "D:");
		index2 = index1 + 18;
		index1 = index1 + 2;
		for (index1; index1 <= index2; index1++) {
			mac[count] = data[index1];
			count++;
		}

		printf("Probe Response : mac __>  %s\n", mac);

	}

	//copy to global net   num++

}
