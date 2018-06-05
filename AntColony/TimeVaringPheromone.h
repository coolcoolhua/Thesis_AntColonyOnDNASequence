//
//  TimeVaringPheromone.h
//  Ant Colony
//
//  Created by 朱驊 on 16/6/14.
//  Copyright (c) 2016年 朱驊. All rights reserved.
//

#ifndef Ant_Colony_TimeVaringPheromone_h
#define Ant_Colony_TimeVaringPheromone_h

void TimeVaringPheromone(float pheromone[][3],int lengthS,int lengthT,int loop){
    int i=0;
    int j=0;
    float time=1;
    if(loop<20){
        time=0.6;
    }
    if((loop>20)&&(loop<50)){
        time=0.8;
    }
    if(loop>50){
        time=0.9;
    }
    for(i=0;i<lengthS*lengthT;i++){
        for(j=0;j<3;j++){
            if(pheromone[i][j]>0.7){
                continue;
            }
            if((pheromone[i][j]>0.4)&&(pheromone[i][j]<=0.7)){
                pheromone[i][j]=pheromone[i][j]*0.8*time;
            }
            if((pheromone[i][j]<=0.4)&&(pheromone[i][j]>0.01)){
                pheromone[i][j]=pheromone[i][j]*0.6*time;
            }
        }
    }
}
#endif
