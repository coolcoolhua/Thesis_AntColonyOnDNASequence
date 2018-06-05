//
//  simpledirection.h
//  Ant Colony
//
//  Created by 朱驊 on 16/5/31.
//  Copyright (c) 2016年 朱驊. All rights reserved.
//

int SimpleDirections(float pheromone[][3], int RowPosition, int LinePosition,int lengthS,int lengthT,int random,int lengthJudge){
    if(abs(lengthJudge)<4){
        int TruePosition = RowPosition*lengthT+LinePosition;
        //cout<<"选择位置为"<<TruePosition<<endl;
        //cout<<"该位置三个方向信息素为"<<endl;
        if(pheromone[TruePosition][2]>=0.8){
            //cout<<"已收敛"<<endl;
            return 2;
        }
        if((pheromone[TruePosition][0]>0.8)||(pheromone[TruePosition][1]>0.8)||(pheromone[TruePosition][2]>0.8)){
            if(pheromone[TruePosition][2]>0.8){
                //cout<<"均过阈值"<<endl;
                return 2;
            }
            if((pheromone[TruePosition][0]>0.8)||(pheromone[TruePosition][1]>0.8)){
                if(pheromone[TruePosition][0]>pheromone[TruePosition][1]){
                    if(pheromone[TruePosition][0]-pheromone[TruePosition][2]>0.25){
                        return 0;
                    }
                    else{
                        if(random>=2){
                            return 2;
                        }
                        else{
                            return 0;
                        }
                    }
                }
                if(pheromone[TruePosition][0]<pheromone[TruePosition][1]){
                    if(pheromone[TruePosition][1]-pheromone[TruePosition][2]>0.25){
                        return 1;
                    }
                    else{
                        if(random>=2){
                            return 2;
                        }
                        else{
                            return 1;
                        }
                    }
                }
            }
        }
        
        if((pheromone[TruePosition][0]==pheromone[TruePosition][1])&&(pheromone[TruePosition][0]==pheromone[TruePosition][2])){
            if(pheromone[TruePosition][2]>=0.7){
                return 2;
            }
            if(random==0){
                return 0;
            }
            if(random==1){
                return 1;
            }
            if((random==2)||(random==3)){
                return 2;
            }
        }
        if((pheromone[TruePosition][0]>pheromone[TruePosition][1])&&(pheromone[TruePosition][0]>pheromone[TruePosition][2])){
            if(pheromone[TruePosition][0]-pheromone[TruePosition][2]>0.3){
                return 0;
            }
            else{
                return 2;
            }
        }
        if((pheromone[TruePosition][1]>pheromone[TruePosition][0])&&(pheromone[TruePosition][1]>pheromone[TruePosition][2])){
            if(pheromone[TruePosition][1]-pheromone[TruePosition][2]>0.3){
                return 1;
            }
            else{
                return 2;
            }
        }
        if((pheromone[TruePosition][2]>pheromone[TruePosition][0])&&pheromone[TruePosition][2]>pheromone[TruePosition][1]){
            return 2;
        }
        return 2;
    }
    return 2;
}
