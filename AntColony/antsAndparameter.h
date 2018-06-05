//
//  antsAndparameter.h
//  Ant Colony
//
//  Created by 朱驊 on 16/6/14.
//  Copyright (c) 2016年 朱驊. All rights reserved.
//

#ifndef Ant_Colony_antsAndparameter_h
#define Ant_Colony_antsAndparameter_h
typedef struct ant{
    int flag=0;
    std::string sequence="";
    int score=0;
    int scorelevel=0;
}ANT;

typedef struct parameter{
    int arraylength;
    int arrayline;
    int lengthS;
    int lengthT;
    float pheromone[10000][3];
    int random;
    int antsnumber;
    int thisAnt=0;
    int lengthJudge=0;
    char temp1[500];
    char temp2[500];
    float annealdelta=0;
}tempParameter;

#endif
