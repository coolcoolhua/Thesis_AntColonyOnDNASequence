//
//  complexdirection.h
//  Ant Colony
//
//  Created by 朱驊 on 16/5/31.
//  Copyright (c) 2016年 朱驊. All rights reserved.
//

int ComplexDirections(float pheromone[][3], int RowPosition, int LinePosition,int lengthS,int lengthT,int random,int lengthJudge,char sequence1[],char sequence2[],float anneal){      //alpha 3-5 beta 2-4 gamma 1-2
    int gapLimit=0;
    double d0=1,d1=1,d2=1;
    float directionRight=1,directionDown=1,directionOpposite=1;
    int TruePosition = RowPosition*lengthT+LinePosition;
    //cout<<"true"<<TruePosition<<endl;
    double mOppositeMatch=10,mDown=0.5,mRight=0.5,mOppositeReplace=0.5;
    double alpha=5,beta=3,gamma=2;
    float pDown=0.1,pRight=0.1,pOpposite=0.1;
    int annealOffset=5;
    
    if(lengthS>lengthT){
        gapLimit=lengthT*0.1+(lengthS-lengthT);
    }
    else if(lengthS<lengthT){
        gapLimit=lengthS*0.1+(lengthT-lengthS);
    }
    if(RowPosition-LinePosition>0){
        d1=d1+(RowPosition-LinePosition)/10;
        d2=d2+(RowPosition-LinePosition)/10;
        if(RowPosition-LinePosition>gapLimit){
            d0=0.5;
            d1=2;
            d2=2;
        }
    }
    if(RowPosition-LinePosition<0){
        d0=d0+(LinePosition-RowPosition)/10;
        d2=d2+(RowPosition-LinePosition)/10;
        if(LinePosition-RowPosition>gapLimit){
            d1=0.5;
            d0=2;
            d2=2;
        }
    }
    
    
   //std::cout<<"位置是"<<RowPosition<<" "<<LinePosition<<std::endl;
    //cout<<sequence1[RowPosition+1]<<" "<<sequence2[LinePosition+1]<<endl;
    //and 如果往下往右，M=0.5
    //cout<<TruePosition<<endl;
    //cout<<"random="<<random<<endl;
    //cout<<"temprandom="<<tempRandom<<endl;
    directionDown=pow((pheromone[TruePosition][0]), alpha) *pow(mDown,beta) *pow(d0,gamma);
    directionRight=pow((pheromone[TruePosition][1]), alpha) *pow(mRight,beta) *pow(d1,gamma);
    if(anneal>0.6){
        int annealRandom=(rand()%(100));
        if(annealRandom>50){
            directionDown=directionDown*annealOffset;
        }
        else{
            directionRight=directionRight*annealOffset;
        }
        
    }
    if(sequence1[RowPosition+1]==sequence2[LinePosition+1]){
        directionOpposite=pow((pheromone[TruePosition][2]), alpha) *pow(mOppositeMatch,beta) *pow(d2,gamma);
    }
    else if(sequence1[RowPosition+1]!=sequence2[LinePosition+1]){
        directionOpposite=pow((pheromone[TruePosition][2]), alpha)*pow(mOppositeReplace,beta)*pow(d2,gamma);
    }
    int tempran=(rand()%(100));
    int rrandom=(rand()%(100));
    float tempRandom=(float)rrandom/100;
    //cout<<tempRandom<<endl;
    //cout<<rrandom<<endl;
    //std::cout<<directionDown<<"  "<<directionRight<<"  "<<directionOpposite<<std::endl;
    /*
     if(pheromone[TruePosition][0]==1){
     return 1;
     }
     if(pheromone[TruePosition][1]==1){
     return 0;
     }
     */
    if(tempran<=20){
        //std::cout<<"<20  "<<directionDown<<" "<<directionRight<<" "<<directionOpposite<<std::endl;
        if((directionOpposite>=directionRight)&&(directionOpposite>=directionDown)){
            if(sequence1[RowPosition+1]==sequence2[LinePosition+1]){
                return 2;
            }
            if(sequence1[RowPosition+1]!=sequence2[LinePosition+1]){
                return 3;
            }
            
        }
        if((directionDown>=directionOpposite)&&(directionDown>=directionRight)){
            return 0;
        }
        if((directionRight>=directionDown)&&(directionRight>=directionOpposite)){
            return 1;
        }
    }
    else if(tempran>20){
        pDown=directionDown/(directionDown+directionOpposite+directionRight);
        pRight=directionRight/(directionRight+directionOpposite+directionDown);
        pOpposite=directionOpposite/(directionOpposite+directionDown+directionRight);
        //ppdown=pDown,ppright=pRight,ppop=pOpposite,pran=tempRandom;
        //std::cout<<">20  "<<pDown<<" "<<pRight<<" "<<pOpposite<<std::endl;
        if(tempRandom<=pDown){
            return 0;
        }
        else if((tempRandom>pDown)&&(tempRandom<=pDown+pRight)){
            return 1;
        }
        else if(tempRandom>pRight+pDown){
            if(sequence1[RowPosition+1]!=sequence2[LinePosition+1]){
                return 3;
            }
            if(sequence1[RowPosition+1]==sequence2[LinePosition+1]){
                return 2;
            }
        }
    }
    
    return 0;
}
