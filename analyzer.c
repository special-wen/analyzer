/*************************************************************************
	> File Name: read.c
	> Author: zxw
	> Mail: 
	> Created Time: 2017年10月11日 星期三 22时06分19秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>


//标识符(变量名表)
typedef struct id{
    char variable[5];
    int id;
    struct id *next;
}ID;
//常数表
typedef struct num{
    char vinary[255];
    int id;
    struct num *next;
}NUM;

#define MAX 10   //读取到的最长单词的个数;;
#define IDMAX 4 //最长标识符个数;
ID *head_id;
NUM *head_num;
int temp; //保存单词中读到的第几位字符;
char str[MAX];  //文件读取时每次读到的单词;
char strToken[MAX]={""}; //存放提取出的每个字符;
int sum; //存放常数;
char ch;        //每个单词的字符;
char *rwtab[6] = {"begin","if","then","while","do","end"};  //保留字;
int IsLetter(char letter);   //判断字符是否为字母;
int IsDight(char ch);       //判断字符是否为数字;
int Reserve(char ch[],char *rwtab[]);  //判断是否为保留字表，并返回其编码;
int InsertId(char str[],ID *head_id);    //将标识符插入符号表中;
int IsertId(char ch[],ID *head_id);  //判断字符是否已经插入在符号表中，如果已插入返回其编码
int InsertConst(int a, NUM *head_num); //将常数插入符号表中;
int IsertConst(int a,NUM *head_num); 
void PrintChar(FILE *fr,char ch[],char value);   //输出保留字和分隔符;
void PrintConst(NUM *head_num);
void PrintId(ID *head_id);

int main(void){
    head_id = malloc(sizeof(ID));
    head_id->next = NULL;
    head_id->id = 0;

    head_num = malloc(sizeof(NUM));
    head_num->next = NULL;
    head_num->id = 0;

    FILE *fd = fopen("./analyzer.txt","r");
    FILE *fr = fopen("./print.txt","w"); 
    //char str[50];
    int a;
    while(fscanf(fd,"%s",str)!=EOF){
        //puts(str);
        //exit(0);
        //判断str是什么东西
        int i = 0;
        int m = 0;
        while(str[i] != '\0' ){
            //printf("%d\n",i);
            //sleep(1);
            //puts("----------------------");
            ch = str[i];
           // printf("%c\n",ch);
            strcpy(strToken,"\0");
            m = 0;
            //判断是否为保留字和是否为标识符;
            if(IsLetter(ch))
            {
                while(IsDight(ch) || IsLetter(ch)){
                    strToken[m] = ch;
                    m++;
                    ch = str[++i];
                }
                //找到一个可能为保留字的字符
                strToken[m] = '\0';
                //puts("");
                //puts(strToken);
                
                //确定是保留字
                if(Reserve(strToken,rwtab) == 1){
                    PrintChar(fr,strToken,'-');
                    //exit(0);
                }
                //确定是否为标识符
                else{
                    //判断是否符合标识符
                    if(m<=4){
                        //将标识符插入符号表中;
                        int get_id;
                        get_id = IsertId(strToken,head_id);
                        fprintf(fr,"(%d,%d)\n",1,get_id);
                    }else{
                        //错误提示
                        fprintf(fr,"标识符最多为4位!");
                    }
                }
                //后退一位
                i--;
            }
            //判断是否为数字或错误提示;
            else if(IsDight(ch)){
                sum = 0;
                while(IsDight(ch)){
                    sum = sum*10 + ch-'0';
                    ch = str[++i];
                }
               // puts("sdasdasdasds");
                if(IsLetter(ch) == 1){
                    while(IsLetter(ch)||IsDight(ch)){
                        ch = str[++i];
                    }
                    fprintf(fr,"错误的变量名!");
                }else{
                    int get_num; 
                    get_num = IsertConst(sum,head_num);
                    fprintf(fr,"(%d,%d)\n",2,get_num);
                }
                i--;
                
                //printf("``````````digg:%d\n",i);
            }
            //判断界符号
            else{
                m=0;
                strToken[m] = ch;
                //puts(strToken);
                //exit(0);
                switch(ch){
                case '<':
                    m = 0;
                    strToken[m] = ch;
                    ch = str[++i];
                    if(ch == '>'){
                        strToken[++m] = ch;
                        //PrintChar(strToken)
                    }else if(ch == '='){
                        strToken[++m] = ch;
                    }else{
                        //ch = strToken[m];
                        i--;
                        strToken[m] = '\0';
                    }
                    PrintChar(fr,strToken,'-');
                    break;
                case '>':
                    m = 0;
                    strToken[m] = ch;
                    ch = str[++i];
                    if(ch == '='){
                        strToken[++m] = ch;
                        strToken[m+1] = '\0';
                    }else{
                        strToken[m-1] = '\0';
                        i--;
                    }
                    PrintChar(fr,strToken,'-');
                    break;
                case ':':
                    //printf("n``````````um:%d\n",i);
                    m=0;
                    strToken[m] = ch;
                    ch = str[++i];
                    //printf("dadadasdas%c\n",ch);
                    if(ch=='='){
                        strToken[++m] = ch;
                        strToken[m+1] = '\0';
                        //puts("dasdasdasdasd");
                    }else{
                        strToken[m+1] = '\0';
                        i--;
                       // printf("*****%d\n",i);
                    }
                    //printf(":=**********%s\n",strToken);
                    PrintChar(fr,strToken,'-');
                    break;
                case '+':strToken[0] = ch;strToken[1] = '\0'; PrintChar(fr,strToken,'-');break;
                case '-':strToken[0] = ch; PrintChar(fr,strToken,'-');break;
                case '*':strToken[0] = ch;strToken[1] = '\0'; PrintChar(fr,strToken,'-');break;
                case '/':strToken[0] = ch;strToken[1] = '\0'; PrintChar(fr,strToken,'-');break;
                case '(':strToken[0] = ch;strToken[1] = '\0'; PrintChar(fr,strToken,'-');break;
                case ')':strToken[0] = ch;strToken[1] = '\0'; PrintChar(fr,strToken,'-');break;
                case '=':strToken[0] = ch;strToken[1] = '\0'; PrintChar(fr,strToken,'-');break;
                case ',':strToken[0] = ch;strToken[1] = '\0'; PrintChar(fr,strToken,'-');break;
                case '.':strToken[0] = ch;strToken[1] = '\0'; PrintChar(fr,strToken,'-');break;
                case ';':strToken[0] = ch;strToken[1] = '\0'; PrintChar(fr,strToken,'-');break;
                        }
                }
                i++;

            }
        }
        PrintConst(head_num);
        PrintId(head_id);
        fclose(fr);
        fclose(fd);
    }

//判断letter是否为字母,返回值为1时，为字母。
int IsLetter(char letter){
    if(letter>='a'&& letter<='z' || letter>='A'&&letter<='Z'){
        return 1;
    }else{
        return 0;
    }
}

//判断ch是否为数字,返回值为1时,为数字.
int IsDight(char ch){
    if(ch >= '0' && ch <= '9')
        return 1;
    else 
        return 0;
}

//判断strToken是否为保留字，如果为保留字，插入保留字表中
int Reserve(char ch[],char *rwtab[]){
    int i;
    for(i= 0 ;i<6;i++){
        if(strcmp(ch,rwtab[i]) ==0){
            return 1;
        }
    }
}

//插入标识符表中
int InsertId(char str[],ID *head_id){
    ID *temp;
    ID *new;
    temp = head_id;
    while(temp->next!= NULL){
        temp = temp->next;
    }
    new = malloc(sizeof(ID));
    new->next = temp->next;
    temp->next = new;
    new->id = temp->id + 1;
    strcpy(new->variable,str);
    
    return new->id;

}

//判断是否已经将该字符插入到标识符表中,并返回其对应的id
int IsertId(char ch[],ID *head_id){
    ID *temp;
    int num;
    temp = head_id->next;
    if(temp != NULL){
        if(strcmp(ch,temp->variable) != 0){
            temp = temp->next;
        }
        num = temp->id;
    }else{
       num =  InsertId(ch,head_id);
    }
    return num;
}

//插入常数表中
int InsertConst(int a,NUM *head_num){
    printf("----%d----\n",a);
    NUM *temp;
    NUM *new;
    char array[255];
    int i;
    array[254] = '\0';
    for(i = 253;i>0;i--){
        array[i] = (char) (a % 2 + '0');
        a = a/2;
        if(a == 0){
            break;
        }
    }
    //printf("%c\n",&array[i]);
    temp = head_num;
    while(temp->next != NULL){
        temp = temp->next;
    }
    new = malloc(sizeof(NUM));
    new->next = temp->next;
    temp->next = new;
    new->id = temp->id +1;
    strcpy(new->vinary,&array[i]);
    return new->id;
}

//判断该常数是否存在常数表中，若存在返回其id,若不存在，插入
int IsertConst(int a,NUM *head_num){
    printf(" %d \n",a);
    NUM *temp;
    int num = a;
    int num_id;
    char array[255];
    array[254] = '\0';
    int i;
    for(i = 253;i>0;i--){
        array[i] = (char)(a % 2 + '0');
        a = a/2;
        if(a == 0)
            break;
    }
    temp = head_num->next;
    printf("erjinzhi:%s\n",&array[i]);
    while(temp != NULL){
        printf("temp->vinary %s\n",temp->vinary);
        if(strcmp(temp->vinary,&array[i]) == 0 ){
            puts(temp->vinary);
            puts("------------");
            return temp->id;
            printf("%d\n",temp->id);
        }
        temp = temp->next;
    }
    num_id = InsertConst(num,head_num);
    printf("%d %d %s\n",num_id ,num,&array[i]); 
    return num_id;
}
//打印输出保留字结果
void PrintChar(FILE *fr,char ch[],char value){
   fprintf(fr,"(%s,%c)\n",ch,value);
}
//输出常数表
void PrintConst(NUM *head_num){
    NUM *temp;
    FILE *fconst = fopen("./PrintConst.txt","w");
    temp = head_num->next;
    while(temp != NULL){
        fprintf(fconst,"(%d,%s)\n",temp->id,temp->vinary);
        temp = temp->next;
    }
    fclose(fconst);
}

//输出变量名表
void PrintId(ID *head_id){
    ID *temp;
    FILE *fId = fopen("./PrintId.txt","w");
    temp = head_id->next;
    while(temp != NULL){
        fprintf(fId,"(%s,%d)\n",temp->variable,temp->id);
        temp = temp->next;
    }
    fclose(fId);
}
