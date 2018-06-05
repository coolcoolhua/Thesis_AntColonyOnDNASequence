//
//  showResult.h
//  Ant Colony
//
//  Created by 朱驊 on 16/6/16.
//  Copyright (c) 2016年 朱驊. All rights reserved.
//

#ifndef Ant_Colony_showResult_h
#define Ant_Colony_showResult_h
std::string ShowResult(char sequenceS[],char sequenceT[],std::string result){
    std::string showResult;
    int sPosition=1,tPosition=1;
    std::string sSequence="";
    std::string tSequence="";
    for(int i =0;i<result.length();i++){
        if(result[i]=='3'){
            sSequence=sSequence+sequenceS[sPosition];
            tSequence=tSequence+sequenceT[tPosition];
            sPosition++;
            tPosition++;
        }
        if(result[i]=='2'){
            sSequence=sSequence+sequenceS[sPosition];
            tSequence=tSequence+sequenceT[tPosition];
            sPosition++;
            tPosition++;
        }
        if(result[i]=='1'){
            sSequence=sSequence+'-';
            tSequence=tSequence+sequenceT[tPosition];
            tPosition++;
        }
        if(result[i]=='0'){
            tSequence=tSequence+'-';
            sSequence=sSequence+sequenceS[sPosition];
            sPosition++;
        }
    }
    if(tPosition!=strlen(sequenceT)){
        for(int i=(int)strlen(sequenceT)-tPosition;i<strlen(sequenceT);i++){
            tSequence=tSequence+sequenceT[i];
        }
    }
    if(sPosition!=strlen(sequenceS)){
        for(int i=(int)strlen(sequenceS)-sPosition;i<strlen(sequenceS);i++){
            sSequence=sSequence+sequenceS[i];
        }
    }
    std::cout<<sSequence<<std::endl<<tSequence<<std::endl;
    
    return showResult;
}


#endif
