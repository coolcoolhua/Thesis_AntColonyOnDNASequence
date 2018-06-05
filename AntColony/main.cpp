//
//  main.cpp
//  Ant Colony
//
//  Created by 朱驊 on 16/3/25.
//  Copyright (c) 2016年 朱驊. All rights reserved.

//  完成朴素信息素更新矩阵
//  完成朴素方向选择函数
//  完成单线程蚂蚁觅食
//  完成多线程蚂蚁并行觅食
//  完成阶梯时变函数
//  完成阶梯函数更新信息素模式
//  完成限定方向选择函数
//  未完成重做收敛函数                           tggagcgtcctctgaaaaatgtcttagatactgaagtctgccgtcgataatg
//  每一轮保留当前分数最高的序列，作为当前的最优比对  tgcgagcgtcctatgaaaaagtctagataactgaagtcctgccgacgattatg
//  实在没找到方向函数到底哪里出错了。。。。。。。。。。。。。。。。
//  解决方向函数BUG 开始解决线程BUG
//  线程BUG解决
//  如何解决长度差距较大的问题
//  改进信息素更新模式:与序列长度关联,（二选一）将分数进行映射or对当前所有蚂蚁进行排序后通过得分分数段再划分
//  重做方向函数：试图解决因过度替换而忽略的插入结果（不要过早缩小解空间）,使用了M，d复合参数
//  试试改进后的方向函数复合阈值
//  没弄明白在前方已经收敛后如何再调整
//  监测一下每轮中以高分的比值
//  算法收敛性本身有问题：最优解只能可寻得但不能收敛/原论文的收敛判定太过简单，而自己的收敛判定太苛刻
//  最终决定的是通过设定分数限，依靠算法寻觅在分数线之上的可行解
//  加入模拟退火算法的思想：如果在搜索中没有找到达到标准的结果，在已得结果中加入滑动窗口，并扫描附近区域是否有连续匹配位置
//  如果有，就在关键点（片段）重做信息素，或重新调整权值，达到加温破坏的效果，扩大解的搜索范围
//  因为很难找到最优解和较优解的差别节点，可能只差在个别几个位点，因此扫描的效果并不好，可以再试试
//  如果第一次判定收敛找到的解很差，直接全部重置，因为其中包含的信息也用处不大/如果找到的解还可以，就只给小的扰动，在本来路径加大其对四周的探索概率
//  现在最大的问题就是 我到底要怎么判定最优解和满足标准解的界限
//  可以在每次收敛判定的轮数内加入模拟退火算子的扰动 比如已经发现大部分蚂蚁的结果分散时，不需要进行算子，因为可以通过信息素逐步收敛；当大部分蚂蚁开始收敛但结果并不达标的话 就施加高温的算子进行扰动


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <cstdlib>
#include <fstream>
#include <time.h>
#import <pthread.h>
#include <math.h>

#include "complexdirection.h"
#include "simpledirection.h"
#include "slidingWindow.h"
#include "antsAndparameter.h"
#include "TimeVaringPheromone.h"
#include "rectifyPheromone.h"
#include "showResult.h"
#include "showPheromone.h"

#define NaN 1024
#define random(x) (rand()%x)
using namespace std;


int resultSpacePlace1[500];
int resultSpacePlace2[500];
int loop=0;
ANT ants[100];
int antsRandom=0;
int loopBoundary=0;
char whetherTimeVaring='y';
int endflag=0;
int tempscore[30];
int lengthS,lengthT;
float anneal=0;

string WholeBestSeq;
string everyloopBestSeq;
string beforeRecBestSeq;
int beforeRecBestScore=-9999;
int wholeBestScore=-9999;
int everyloopBestScore=-9999;
float ppdown,ppright,ppop,pran,tempposi;
int gapCount=0;
int highestCount=0;
int bestcount=0;
int scoreLimit=0;
int lengthJudge=0;

int SimpleDirections(float pheromone[][3], int RowPosition, int LinePosition,int lengthS,int lengthT,int random,int lengthJudge);
void UpdatePheromone(ANT ants[],int antsNumber,int lengthS,int lengthT,float pheromone[][3]);
void StepUpdatePheromone(ANT ants[],int antsNumber,int lengthS,int lengthT,float pheromone[][3],char sequence1[],char sequence2[],int loopBoundary);
std::string ShowResult(char sequenceS[],char sequenceT[],std::string result);
void TimeVaringPheromone(float pheromone[][3],int lengthS,int lengthT,int loop);
void *AntForage(void *args);
void antsScoreSort(int antsNumber);
void showPheromone(int lengthS,int lengthT,float pheromone[][3]);
int ComplexDirections(float pheromone[][3], int RowPosition, int LinePosition,int lengthS,int lengthT,int random,int lengthJudge,char sequence1[],char sequence2[],float superviseConvergence);
int Convergence(ANT ants[],char sequence1[],char sequence2[],int lengthS,int lengthT,float pheromone[][3]);
int slidingWindowRectify(int lengthS,int lengthT,char sequence1[],char sequence2[],int rectifyResult[500][500]);
int rectifyPheromone(int lengthS,int lengthT,int rectifyResult[500][500],float pheromone[][3],float scoreLimit,string WholeBestSeq,int beforeRecBestScore);





int main(int argc, const char * argv[]) {
    int i,j;
    //cout<<"是否采用改进方法（阶梯式信息素，信息素时变函数）进行搜索 (是输入Y,否输入N)"<<endl;
    srand((unsigned)time(NULL));
    char tempSequence1[1000]="tgcgagcgtcctatgaaaaagtctagataactgaagtcctgccgacgattatg";
    char tempSequence2[1000]="tggagcgtcctctgaaaaatgtcttagatactgaagtctgccgtcgataatg";
    //std::cout<<"请输入序列1"<<std::endl;
    //std::cin>>tempSequence1;
    //std::cout<<"请输入序列2"<<std::endl;
    //std::cin>>tempSequence2;
    char sequence1[500]=" ";
    char sequence2[500]=" ";
    strcat(sequence1,tempSequence1);
    strcat(sequence2,tempSequence2);
    cout<<sequence1<<endl;
    cout<<sequence2<<endl;
    lengthS=(int)strlen(sequence1);
    lengthT=(int)strlen(sequence2);
    //cout<<lengthS*lengthT<<endl;
    lengthJudge=lengthS-lengthT;
    int rectifyResult[500][500];
    for(int i=0;i<lengthS;i++){
        for(int j=0;j<lengthT;j++){
            rectifyResult[i][j]=0;
        }
    }
    if(lengthS>=lengthT){
        loopBoundary = lengthT*2;
    }
    else if(lengthS<lengthT){
        loopBoundary = lengthS*2;
    }
    float scorelimit=(float)loopBoundary;
    if(loopBoundary>100){
        loopBoundary=100;
    }
    //cout<<lengthS<<" " <<lengthT<<endl;
    if(lengthS>=lengthT){                              //随机选取蚂蚁数目 经验数值以序列长度的25%-33%取随机数
        antsRandom=(lengthS/3+rand() %(1+ abs((lengthS/3-lengthS/4))));
        scoreLimit=lengthT*1.4;
    }
    else if(lengthT>lengthS){
        antsRandom=(lengthT/3+rand() %(1+ abs((lengthT/3-lengthT/4))));
        scoreLimit=lengthS*1.4;
    }
    //cout<<antsRandom<<endl;
    if(antsRandom<5){
        antsRandom = 5;
    }
    if(antsRandom>30){
        antsRandom=20;
    }
    cout<<"随机选取的蚂蚁数目为"<<antsRandom<<endl;
    pthread_t thread[antsRandom];
    //antsRandom=1;
    
    float pheromone[lengthS*lengthT][3];               //初始化信息素矩阵
    for(i = 0;i<lengthS*lengthT;i++){
        for(j = 0;j<3;j++){
            pheromone[i][j]=1;
        }
    }
    int threadCreateFlag=0;                           //创建多线程，蚂蚁同时进行觅食
    tempParameter Parameter[50];
    for(int k=0;k<antsRandom;k++){
        for(i=0;i<lengthS*lengthT;i++){
            for(j=0;j<3;j++){
                Parameter[k].pheromone[i][j]=pheromone[i][j];
            }
        }
    }
    while(endflag!=1){
        cout<<"第"<<loop<<"次搜索"<<endl;
        for(int t = 0; t <antsRandom; t++)
        {
            for(int i=0;i<lengthS*lengthT;i++){
                for(int j=0;j<3;j++){
                    Parameter[t].pheromone[i][j]=pheromone[i][j];
                }
            }
            Parameter[t].lengthS=lengthS;
            Parameter[t].lengthT=lengthT;
            Parameter[t].arraylength=lengthS-1;
            Parameter[t].arrayline=lengthT-1;
            Parameter[t].antsnumber=antsRandom;
            Parameter[t].lengthJudge=lengthJudge;
            if((loop>1)&&(wholeBestScore<loopBoundary*0.99)){
                Parameter[t].annealdelta=anneal;
            }
            strcpy(Parameter[t].temp1, sequence1);
            strcpy(Parameter[t].temp2, sequence2);
            //int random=(rand() % (4));
            
             
            int random=(rand()%(100));
            
            Parameter[t].random=random;
            Parameter[t].thisAnt=t;
            //cout<<"随机数是"<<tempParameter.random<<endl;
            //printf("第%d只蚂蚁开始寻径\n", t+1);
            threadCreateFlag = pthread_create(&thread[t], NULL, AntForage, &Parameter[t]);

            if (threadCreateFlag)
            {
                printf("线程创建错误，错误码是%d\n", threadCreateFlag);
            }

        }

        for(int t=0;t<antsRandom;t++){
             pthread_join(thread[t], NULL);
        }

        //cout<<"更新信息素"<<endl;                                         //更新信息素
        if(loop>loopBoundary*0.1){
            if((whetherTimeVaring=='Y')||(whetherTimeVaring=='y')){
                TimeVaringPheromone(pheromone, lengthS, lengthT,loop);
            }
        }
        StepUpdatePheromone(ants, antsRandom, lengthS, lengthT, pheromone,sequence1,sequence2,loopBoundary);
        //showPheromone(lengthS, lengthT, pheromone);
        std::cout<<std::endl;
        int tempcount[antsRandom];
        for(int i =0;i<antsRandom;i++){
            tempcount[i]=0;
        }
        for(int i=0;i<antsRandom;i++){
            //cout<<"第"<<i<<"只蚂蚁的序列为"<<ants[i].sequence<<endl;
            //cout<<"这只蚂蚁的得分是"<<ants[i].score<<endl;
            for(int j=0;j<antsRandom;j++){
                if(ants[i].sequence==ants[j].sequence){
                    tempcount[i]++;
                }
            }
            //cout<<"第"<<i<<"个序列的相同个数为"<<tempcount[i]<<endl;
        }
        //cout<<"得到的最佳匹配是"<<WholeBestSeq<<endl;
        //cout<<"分数是"<<wholeBestScore<<endl;
        for(int i =0;i<antsRandom;i++){                                //显示结果
            if((bestcount>loopBoundary*0.1)||(loop>loopBoundary)){
                if(bestcount>loopBoundary*0.05){
                    if(wholeBestScore<scorelimit*0.75){
                        if(loop>loopBoundary){
                            cout<<"超过搜索最大轮数,未能获得收敛结果，当前搜索最优结果为："<<endl;
                            cout<<WholeBestSeq<<endl;
                            ShowResult(sequence1,sequence2,WholeBestSeq);
                            cout<<"分数是"<<wholeBestScore<<endl;
                            showPheromone(lengthS, lengthT, pheromone);
                            return 0;
                        }

                        cout<<"求得收敛解"<<endl;
                        ShowResult(sequence1, sequence2, WholeBestSeq);
                        cout<<"该解分数是:"<<wholeBestScore<<"<"<<scorelimit*0.75<<",未满足求解要求 继续求解"<<endl;
                        if(wholeBestScore>beforeRecBestScore){
                            beforeRecBestScore=wholeBestScore;
                            beforeRecBestSeq=WholeBestSeq;
                            cout<<"beforeRecBest是"<<beforeRecBestScore<<endl;
                        }
                        //slidingWindowRectify(lengthS, lengthT, sequence1, sequence2, rectifyResult);
                        rectifyPheromone(lengthS, lengthT, rectifyResult, pheromone,scoreLimit,WholeBestSeq,beforeRecBestScore);
                        bestcount=0;
                        
                        break;
                    }
                    if(wholeBestScore>=scorelimit*0.75){
                        cout<<"求得满足条件解"<<endl;
                        ShowResult(sequence1, sequence2, WholeBestSeq);
                        cout<<"该解分数是:"<<wholeBestScore<<endl;
                        cout<<"该解序列为:"<<WholeBestSeq<<endl;
                        showPheromone(lengthS, lengthT, pheromone);
                        return 0;

                    }
                }
                
            }
        }
        if(loop>loopBoundary){
            cout<<"超过搜索最大轮数,未能获得收敛结果，当前搜索最优结果为："<<endl;
            cout<<beforeRecBestSeq<<endl;
            ShowResult(sequence1,sequence2,beforeRecBestSeq);
            cout<<"分数是"<<beforeRecBestScore<<endl;
            showPheromone(lengthS, lengthT, pheromone);
            return 0;
        }
        for(int i=0;i<antsRandom;i++){
            ants[i].score=0;
        }
        for(int i=0;i<lengthS*lengthT;i++){
            for(int j=0;j<3;j++)
            {
                if(pheromone[i][j]>=2){
                    //pheromone[i][j]=2;
                }
                if(pheromone[i][j]<1){
                    pheromone[i][j]=1;
                }
            }
        }

        for(int i=0;i<antsRandom;i++){
            ants[i].flag=0;
            ants[i].sequence="";
            tempcount[i]=0;
        }
        loop++;
    }
    return 0;
}












void StepUpdatePheromone(ANT ants[],int antsNumber,int lengthS,int lengthT,float pheromone[][3],char sequence1[],char sequence2[],int loopBoundary){
    float best=5.0/loopBoundary/antsNumber,good=2.0/loopBoundary/antsNumber,soso=0.5/loopBoundary/antsNumber,bad=0/loopBoundary/antsNumber;
    for(int i = 0;i<antsNumber;i++){//阶梯函数模式更新信息素
        //cout<<"序列是"<<ants[i].sequence<<endl;
        int length=(int)ants[i].sequence.length();
        for(int j=0;j<length;j++){
            char temp=ants[i].sequence[j];
            if(temp=='2'){
                ants[i].score=ants[i].score+2;
            }
            if(temp=='3'){
                ants[i].score=ants[i].score-1;
            }
            if(temp=='1'){
                if((j>1)&&(ants[i].sequence[j-1]=='1')){
                    ants[i].score=ants[i].score-2;
                }
                else{
                    ants[i].score=ants[i].score-2;
                }
            }
            if(temp=='0'){
                if((j>1)&&(ants[i].sequence[j-1])=='0'){
                    ants[i].score=ants[i].score-2;
                }
                else{
                    ants[i].score=ants[i].score-2;
                }
            }
        }
        //cout<<"分数是"<<ants[i].score<<endl;
    }
    
    /*       通过排序结果进行更新
    antsScoreSort(antsNumber);
    for(int i=0;i<antsNumber;i++){
        if(i<antsNumber/4){
            ants[i].scorelevel=1;
        }
        if((i>=antsNumber/4)&&(antsNumber/2)){
            ants[i].scorelevel=2;
        }
        if((i>antsNumber/2)&&(i<=antsNumber/4*3)){
            ants[i].scorelevel=3;
        }
        if(i>antsNumber/4*3){
            ants[i].scorelevel=4;
        }
    }
     
     */
    
    int basic=0;
    if(lengthS>lengthT){
        basic=lengthT;
    }
    else{
        basic=lengthS;
    }
    int scoreBasic=basic*2;
    for(int i=0;i<antsNumber;i++){
        if(ants[i].score>=scoreBasic*0.5){
            ants[i].scorelevel=4;
        }
        if((ants[i].score>=scoreBasic*0.2)&&(ants[i].score<scoreBasic*0.5)){
            ants[i].scorelevel=3;
        }
        if((ants[i].score>=0)&&(ants[i].score<scoreBasic*0.2)){
            ants[i].scorelevel=2;
        }
        if(ants[i].score<0){
            ants[i].scorelevel=1;
        }
        
        
    }
    int length;
    if(strlen(sequence1)>strlen(sequence2)){
        length=(int)strlen(sequence1);
    }
    else if(strlen(sequence2)>strlen(sequence1)){
        length=(int)strlen(sequence2);
    }
    for(int i=0;i<antsRandom;i++){
        int row=0;
        int line=0;
        int length=(int)ants[i].sequence.length();
        //cout<<"该蚂蚁的得分为"<<ants[i].score<<endl;
        //cout<<"该蚂蚁的序列为"<<ants[i].sequence<<endl;
        //cout<<"该蚂蚁的序列长度为"<<length<<endl;
        for(int j = 0;j<length;j++){
            //cout<<"row="<<row<<"line="<<line<<endl;
            if(ants[i].sequence[j]=='2'){
                if(ants[i].scorelevel==4){
                    pheromone[row*lengthT+line][2]=pheromone[row*lengthT+line][2]+best;
                    row++;
                    line++;
                }
                else if(ants[i].scorelevel==3){
                    pheromone[row*lengthT+line][2]=pheromone[row*lengthT+line][2]+good;
                    row++;
                    line++;
                }
                else if(ants[i].scorelevel==2){
                    pheromone[row*lengthT+line][2]=pheromone[row*lengthT+line][2]+soso;
                    row++;
                    line++;
                }
                else if(ants[i].scorelevel==1){
                    pheromone[row*lengthT+line][2]=pheromone[row*lengthT+line][2]+bad;
                    row++;
                    line++;
                }
            }
            if(ants[i].sequence[j]=='3'){
                if(ants[i].scorelevel==4){
                    pheromone[row*lengthT+line][2]=pheromone[row*lengthT+line][2]+best/2;
                    row++;
                    line++;
                }
                else if(ants[i].scorelevel==3){
                    pheromone[row*lengthT+line][2]=pheromone[row*lengthT+line][2]+good/2;
                    row++;
                    line++;
                }
                else if(ants[i].scorelevel==2){
                    pheromone[row*lengthT+line][2]=pheromone[row*lengthT+line][2]+soso/2;
                    row++;
                    line++;
                }
                else if(ants[i].scorelevel==1){
                    pheromone[row*lengthT+line][2]=pheromone[row*lengthT+line][2]+bad/2;
                    row++;
                    line++;
                }
            }

            if(ants[i].sequence[j]=='1'){
                /*if(pheromone[RightPosition][2]>0.8){
                    line++;
                    continue;
                }
                 */
                if(ants[i].scorelevel==4){
                    pheromone[row*lengthT+line][1]=pheromone[row*lengthT+line][1]+best;
                    line++;
                }
                else if(ants[i].scorelevel==3){
                    pheromone[row*lengthT+line][1]=pheromone[row*lengthT+line][1]+good;
                    line++;
                }
                else if(ants[i].scorelevel==2){
                    pheromone[row*lengthT+line][1]=pheromone[row*lengthT+line][1]+soso;
                    line++;
                }
                else if(ants[i].scorelevel==1){
                    pheromone[row*lengthT+line][1]=pheromone[row*lengthT+line][1]+bad;
                    line++;
                }
            }
            if(ants[i].sequence[j]=='0'){
                /*if(pheromone[RightPosition][2]>0.8){
                    row++;
                    continue;
                }
                 */
                if(ants[i].scorelevel==4){
                    pheromone[row*lengthT+line][0]=pheromone[row*lengthT+line][0]+best;
                    row++;
                }
                else if(ants[i].scorelevel==3){
                    pheromone[row*lengthT+line][0]=pheromone[row*lengthT+line][0]+good;
                    row++;
                }
                else if(ants[i].scorelevel==2){
                    pheromone[row*lengthT+line][0]=pheromone[row*lengthT+line][0]+soso;
                    row++;
                }
                else if(ants[i].scorelevel==1){
                    pheromone[row*lengthT+line][0]=pheromone[row*lengthT+line][0]+bad;
                    row++;
                }
            }
        }

    }
    for(int i =0;i<antsNumber;i++){
        if(ants[i].score>=everyloopBestScore){
            everyloopBestScore=ants[i].score;
            everyloopBestSeq=ants[i].sequence;
        }
    }
    for(int i=0;i<=antsNumber;i++){
        if(ants[i].score==everyloopBestScore){
            highestCount++;
        }
    }
    cout<<"本轮最高分为"<<everyloopBestScore<<endl;
    anneal=(float)highestCount/(float)antsNumber;
    if(anneal>0.6){
        cout<<"已扰乱"<<endl;
    }
    //cout<<"本轮最高分占比为"<<(float)highestCount/(float)antsNumber<<endl;
    if(everyloopBestScore>wholeBestScore){
        wholeBestScore=everyloopBestScore;
        cout<<WholeBestSeq<<endl;
        WholeBestSeq=everyloopBestSeq;
        cout<<wholeBestScore<<endl;
        cout<<"当前最高分是"<<wholeBestScore<<endl;
        bestcount=0;
    }
    bestcount++;
    highestCount=0;
    everyloopBestScore=-9999;
    everyloopBestSeq="";
}


void *AntForage(void *args)
{
    int row=0,line=0;
    struct parameter *pstru;
    pstru = ( struct parameter *) args;
    while(ants[pstru->thisAnt].flag!=1){
        
        //pstru->random=random;
        int tempDirection=0;
        if(row==pstru->arraylength){
            //cout<<"bottom"<<endl;
            line=line+1;
            ants[pstru->thisAnt].sequence=ants[pstru->thisAnt].sequence+"1";
            //cout<<"row="<<row<<" "<<"line="<<line<<endl;
            if((row==pstru->arraylength)&&(line==pstru->arrayline)){
                ants[pstru->thisAnt].flag=1;
                //cout<<"end"<<endl;
                row=0;
                line=0;
            }
            continue;
        }
        if(line==pstru->arrayline){
            //cout<<"side"<<endl;
            row=row+1;
            //cout<<"row="<<row<<" "<<"line="<<line<<endl;
            ants[pstru->thisAnt].sequence=ants[pstru->thisAnt].sequence+"0";
            if((row==pstru->arraylength)&&(line==pstru->arrayline)){
                ants[pstru->thisAnt].flag=1;
                //cout<<"end"<<endl;
                row=0;
                line=0;
            }
            continue;
        }
        tempDirection=ComplexDirections(pstru->pheromone, row, line, pstru->lengthS, pstru->lengthT,pstru->random,pstru->lengthJudge,pstru->temp1,pstru->temp2,pstru->annealdelta);
        //tempDirection=SimpleDirections(pstru->pheromone, row, line, pstru->lengthS, pstru->lengthT, pstru->random, pstru->lengthJudge);
        //std::cout<<tempDirection<<std::endl;
        if(tempDirection==3){
            row++;
            line++;
            ants[pstru->thisAnt].sequence=ants[pstru->thisAnt].sequence+"3";
        }
        if(tempDirection==2){
            row++;
            line++;
            ants[pstru->thisAnt].sequence=ants[pstru->thisAnt].sequence+"2";                         //2右下1右0下
        }
        if(tempDirection==1){
            line++;
            ants[pstru->thisAnt].sequence=ants[pstru->thisAnt].sequence+"1";
        }
        if(tempDirection==0){
            row++;
            ants[pstru->thisAnt].sequence=ants[pstru->thisAnt].sequence+"0";
        }
        //cout<<"row="<<row<<" "<<"line="<<line<<endl;
        if((row==pstru->arraylength)&&(line==pstru->arrayline)){
            ants[pstru->thisAnt].flag=1;
            //cout<<"end once"<<endl;
            row=0;
            line=0;
        }
    }
    return NULL;
}
