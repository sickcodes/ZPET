//
//  processor.cpp
//  ZPET
//  Last Modified: 11-12-2020
//
//  Created by James Duffy on 21/10/2020.
//  Copyright © 2020 James Duffy. All rights reserved.
//

#include "processor.hpp"
#include "../misc.h"
#include <iostream>

int plistproc(std::string foi, std::string key, std::string piperesult, std::string IP, std::string PORT, std::string DEVICEPW){
    
    if(PLIST_COMPAT!=1) return 2; //1 meaning compatible in this case
    if(iosReceive(foi,IP,DEVICEPW,PORT)==0){
        //implement C++ Strings here
        char proc[800];
        if(key.length()==0){
            std::sprintf(proc,"plutil -p SENSITIVE/local 2>&1");
        } else{
            std::sprintf(proc,"plutil -extract %s xml1 -o - SENSITIVE/local 2>&1 | tail -2 | grep -v plist | cut -f1 -d'/' | sed 's/^<\(.*)/\1/'| sed 's/^>\(.*)/\1/' | cut -f2 -d'>' | cut -f1 -d'<'",key.c_str());
        }
        if(piperesult.length()>2){
            //            std::cout << "PIPE-RES is -> " << piperesult << std::endl;
            std::sprintf(proc,"%s | %s",proc,piperesult.c_str());
        }
        std::system(proc);
        
        return 0;
    } else{
        std::cout << "[!] Module Could Not Execute - Device Communication Failed...\nIs The Target Device Connected?" << std::endl;
        return 1;
    }
}

int sqliteproc(std::string foi, std::string query, std::string piperesult, std::string IP, std::string PORT, std::string DEVICEPW){
    //    printf("sqliteproc hit\n");
    if(SQLITE_COMPAT!=1) return 2; //1 meaning compatible in this case
    if(iosReceive(foi,std::move(IP),DEVICEPW,PORT)==0){
        char proc[800];
        std::sprintf(proc,"sqlite3 SENSITIVE/local -line '%s' | cut -f2 -d'=' | awk NF",query.c_str());
        if(piperesult.length()>2){
            //            std::cout << "PIPE-RES is -> " << piperesult << std::endl;
            std::sprintf(proc,"%s | %s",proc,piperesult.c_str());
        }
        std::system(proc);
        return 0;
    } else{
        std::cout << "[!] Module Could Not Execute - Device Communication Failed...\nIs The Target Device Connected?" << std::endl;
        return 1;
    }
}

int jsonproc(std::string foi, std::string sval, std::string IP, std::string PORT, std::string DEVICEPW){
    if(JSON_COMPAT!=1) return 2;
    std::cout << "[!] Module Could Not Execute - Device Communication Failed...\nIs The Target Device Connected?" << std::endl;
    return 1;
}

int execproc(std::string foi, std::string exec, std::string piperesult, std::string IP, std::string PORT, std::string DEVICEPW, std::string execend){
    if(JSON_COMPAT!=1) return 2; //1 meaning compatible in this case
    //    printf("execproc hit\n");
    iosReceive(foi,std::move(IP),DEVICEPW,PORT);
    char proc[800];
    std::sprintf(proc,"%s",exec.c_str());
    if(piperesult.length()>2){
        //            std::cout << "PIPE-RES is -> " << piperesult << std::endl;
        std::sprintf(proc,"%s | %s",proc,piperesult.c_str());
    }
    std::system(proc);
    return 0;
}
