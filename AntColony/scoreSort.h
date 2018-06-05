//
//  scoreSort.h
//  Ant Colony
//
//  Created by 朱驊 on 16/6/14.
//  Copyright (c) 2016年 朱驊. All rights reserved.
//
#include "antsAndparameter.h"
#ifndef Ant_Colony_scoreSort_h
#define Ant_Colony_scoreSort_h



void antsScoreSort(int antsNumber,ANT ants[])
{
    int i,j,k;
    string temp;
    int tempscore=0;
    for(i=0;i<antsNumber-1;i++)
    {
        k=i;
        for(j=i+1;j<antsNumber;j++){
            if(ants[k].score>ants[j].score) {
                k=j;
            }
        }
        if(i!=k)
        {
            tempscore=ants[i].score;
            ants[i].score=ants[k].score;
            ants[k].score=tempscore;
            temp=ants[i].sequence;
            ants[i].sequence=ants[k].sequence;
            ants[k].sequence=temp;
        }
    }
}

#endif
