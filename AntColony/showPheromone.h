//
//  showPheromone.h
//  Ant Colony
//
//  Created by 朱驊 on 16/6/16.
//  Copyright (c) 2016年 朱驊. All rights reserved.
//

#ifndef Ant_Colony_showPheromone_h
#define Ant_Colony_showPheromone_h

void showPheromone(int lengthS,int lengthT,float pheromone[][3]){
    std::cout<<"是否显示信息素浓度表"<<std::endl;
    char whetherPheromoneTable;
    std::cin>>whetherPheromoneTable;
    int k=0;
    if((whetherPheromoneTable=='Y')||(whetherPheromoneTable=='y')){
        for(int i=0;i<lengthS*lengthT;i++){
            //cout<<"k="<<k<<"   ";
            k++;
            for(int j=0;j<3;j++){
                //cout<<"k="<<k<<"   ";
                std::cout<<pheromone[i][j]<<" ";
            }
            std::cout<<std::endl;
        }
    }
    std::cout<<std::endl;
}



#endif
