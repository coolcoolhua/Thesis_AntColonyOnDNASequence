//
//  slidingWindow.h
//  Ant Colony
//
//  Created by 朱驊 on 16/6/14.
//  Copyright (c) 2016年 朱驊. All rights reserved.
//

#ifndef Ant_Colony_slidingWindow_h
#define Ant_Colony_slidingWindow_h

int slidingWindowRectify(int lengthS,int lengthT,char sequence1[],char sequence2[],int rectifyResult[500][500],std::
                         string WholebestSeq){
    int rowPosition=1,linePosition=1;
    int WindowSize=0;
    int combo=0;
    if(lengthS>lengthT){
        WindowSize=lengthT*0.1;
    }
    else if(lengthS<lengthT){
        WindowSize=lengthS*0.1;
    }
    char temp;
    for(int i=0;i<WholebestSeq.length()/5;i++){
        int tempRow=rowPosition,templine=linePosition;
        for(int j=0;j<WindowSize;j++){
            for(int k=0;k<WindowSize;k++){
                for(int t=0;t<5;t++){
                    if(sequence1[rowPosition+t]==sequence2[linePosition+t]){
                        combo++;
                    }
                    if(combo>4){
                        rectifyResult[rowPosition][linePosition]=1;
                        combo=0;
                        break;
                    }
                }
                linePosition++;
            }
            linePosition=templine;
            rowPosition++;
        }
        rowPosition=tempRow;
        for(int p=0;p<WindowSize;p++){
            temp=WholebestSeq[i];
            if(temp=='2'){
                rowPosition++;
                linePosition++;
            }
            if(temp=='1'){
                linePosition++;
            }
            if(temp=='0'){
                rowPosition++;
            }
        }
    }
    
    
    return 0;
}

#endif
