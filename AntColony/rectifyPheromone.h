//
//  rectifyPheromone.h
//  Ant Colony
//
//  Created by 朱驊 on 16/6/14.
//  Copyright (c) 2016年 朱驊. All rights reserved.
//

#ifndef Ant_Colony_rectifyPheromone_h
#define Ant_Colony_rectifyPheromone_h
int rectifyPheromone(int lengthS,int lengthT,int rectifyResult[500][500],float pheromone[][3],float scoreLimit,std::string WholeBestSeq,int beforeRecBestScore){
    /*
     for(int i=0;i<lengthS*lengthT;i++){
     for(int j=0;j<3;j++){
     pheromone[i][j]=pheromone[i][j]*0.9;
     }
     }
     for(int i=0;i<lengthS;i++){
     for(int j=0;j<lengthT;j++){
     if(rectifyResult[i][j]==1){
     pheromone[(i-1)*lengthT+j][0]=0.5;
     pheromone[i*lengthT+j-1][1]=0.5;
     pheromone[(i-1)*lengthT+j-1][2]=0.5;
     }
     }
     }
     */
    int row=0,line=0;
    std::cout<<"beforeRecBestScore="<<beforeRecBestScore<<std::endl;
    if(beforeRecBestScore<scoreLimit*0.5){
        for(int i=0;i<WholeBestSeq.length();i++){
            //cout<<row*lengthT+line<<endl;
            if(WholeBestSeq[i]=='2'){
                pheromone[row*lengthT+line][0]=0.1;
                pheromone[row*lengthT+line][1]=0.1;
                pheromone[row*lengthT+line][2]=0.1;
                row++;
                line++;
            }
            if(WholeBestSeq[i]=='1'){
                pheromone[row*lengthT+line][0]=0.1;
                pheromone[row*lengthT+line][1]=0.1;
                pheromone[row*lengthT+line][2]=0.1;
                line++;
            }
            if(WholeBestSeq[i]=='0'){
                pheromone[row*lengthT+line][0]=0.1;
                pheromone[row*lengthT+line][1]=0.1;
                pheromone[row*lengthT+line][2]=0.1;
                row++;
            }
        }
    }
    else{
        for(int i=0;i<WholeBestSeq.length();i++){
            //cout<<row*lengthT+line<<endl;
            if(WholeBestSeq[i]=='2'){
                pheromone[row*lengthT+line][0]=0.2;
                pheromone[row*lengthT+line][1]=0.2;
                pheromone[row*lengthT+line][2]=0.4;
                row++;
                line++;
            }
            if(WholeBestSeq[i]=='1'){
                pheromone[row*lengthT+line][0]=0.1;
                pheromone[row*lengthT+line][1]=0.4;
                pheromone[row*lengthT+line][2]=0.2;
                line++;
            }
            if(WholeBestSeq[i]=='0'){
                pheromone[row*lengthT+line][0]=0.4;
                pheromone[row*lengthT+line][1]=0.1;
                pheromone[row*lengthT+line][2]=0.2;
                row++;
            }
        }
        
        
    }
    return 0;
}

#endif
