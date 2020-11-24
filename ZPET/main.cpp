//
//  main.cpp
//  ZPETv2
//
//  Created by James Duffy on 21/10/2020.
//  Copyright © 2020 James Duffy, DuffyAPP_IT. All rights reserved.
//

#include "moduleproc/module.hpp"
#include "misc.h"
#include "device.hpp"

int main(int argc, char *argv[]) {
    int analytics = 0;
    std::string moduleDir = "modules/moduleloader";
    if(XC==1) system("pwd");
    
    std::cout << "\033[1;34m  ___________  ______ _______  \n"
    "\033[1;35m |___  /  __ \\|  ____|__   __|\n"
    "\033[1;31m    / /| |__) | |__     | |    \n"
    "\033[1;36m   / / |  ___/|  __|    | |    \n"
    "\033[1;34m  / /__| |    | |____   | |    \n"
    "\033[1;31m /_____|_|    |______|  |_|\033[0m v2-internal" << std::endl;

    //Check if running as root... (folder permissions restricted for user safety)
    if(getuid()!=0){
        std::cout << "ZPETv2 Must Be Run As Root! (sudo ./ZPET)" << std::endl;
        return 1;
    }
    
    
    /*
     ModuleLoader - Basic Prerequesite Check
     'moduleloader' *must* be present for ZPETv2 core functionality
     External modules are listed inside moduleloader for, well, loading!
     */
    if (!is_file_exist(moduleDir.c_str())) {
        std::cout << "[!] ZPET component 'moduleloader' is missing!\nIf you downloaded the repository from GitHub, you will be missing components...\nThe extra components are bundled in the release build. Check releases in GitHub!" << std::endl;
        return 1;
    }
    
    //UserConsent Checker
    if(is_file_exist(".analytics")){
        if(load_consent_data()=="y"){
            if (XC==1) std::cout << "[@] user consent enabled" << std::endl;
            analytics=1;
        } else if(load_consent_data()=="n"){
            if (XC==1) std::cout << "[@] user consent disabled" << std::endl;
            analytics=0;
        } else{
            std::cout << "[!] Invalid Consent State" << std::endl;
            exit(1);
        }
    } else{ //first launch consent
        std::cout << "---------\nWelcome To ZPET\n---------" << std::endl;
        std::cout << "ZPET is actively developed by a single developer, myself, @J_Duffy01 !" << std::endl;
        std::cout << "I have implemented very basic, *non-attributable*, optional analytics - i've made this transparent, showing example requests for each event:\n" << std::endl;
        std::cout << "Would you like to submit the following analytics?\n[*] ZPETv2 Launch 'Ping'!\t'{device:'MacBookPro13,3',event:'userStart'}'" << std::endl;
        std::cout << "[*] ZPETv2 Internal Error\t'{device:'MacBookPro13,3',event:'userProcess:iOSRecieveErr'}'" << std::endl;
        std::cout << "[*] ZPETv2 Feature Entry\t'{device:'MacBookPro13,3',event:'userFeatureHit:SPIDER'}'" << std::endl;
        std::cout << "\ny/n [?] ";
        std::string consent_data;
        std::cin >> consent_data;
        if(consent_data=="y"){
            write_consent_data("y");
            analytics=1;
            submit_event("userAnalytic:enable");
            std::cout << "\n";
        } else if(consent_data=="n"){
            write_consent_data("n");
            analytics=0;
            std::cout << "\n";
        } else{
            std::cout << "Invalid Consent Response" << std::endl;
            exit(1);
        }
    }
    
    //BASIC INIT END
    
    /*
     ZPETv2 - Main Menu
     Last Author: James Duffy
     Last Modified: 17-11-2020
     Purpose: Capture user input & direct execution flow based on user choice...
     Notes:
            * Ensure that only required dependencies are loaded based on user choice!
            * Menu item array size based on amount of items (dynamic array? not sure if you can do this easily in c++?)
     */
    
    if(analytics==1) submit_event("userStart");
    
    std::string Menu[7]={"[LIVE]\tCheckra1n CLI\t\t\t(Boot From DFU)","[LIVE]\tExecute SPIDER\t\t\t(User-Data Analysis)","[LIVE]\tExecute ZPETv2 Modules\t\t(Documentation!)","[LIVE]\tAcquire Device Root Filesystem\t(Encryption State Prompt Prior To Acquisition)","[ROOT-FS]\tExecute SPIDER Locally\t\t(User-Data Analysis)","[ROOT-FS]\tExecute Mapper Locally\t\t(Blind Analysis)"};
    
    
    std::cout << "[*] Main Menu" << std::endl;
    for(int i=0;i<((sizeof(Menu)/sizeof(Menu[0]))-1);i++){
        std::cout << "[" << (i+1) << "] " << Menu[i] << std::endl;
    }

    int userOpt=0;
    std::cout << "\n[?] ";
    std::cin >> userOpt; //replace me with something more secure!
    
    switch (userOpt) {
        case 1:
            if (analytics==1) submit_event("userFeatureHit:Checkra1n");
            //check if checkra1n exists in /Applications, launch with '-c' (ctrl+c back to ZPET)
            break;
        case 2:
            if (analytics==1) submit_event("userFeatureHit:SpiderLIVE");
            //check if spider is present in resources/spiderkit... present mini menu in ZPET?
            break;
        case 3:
            if (analytics==1) submit_event("userFeatureHit:ZPETModuleProc");
                //Create 'SENSITIVE' Directory For Device Data Processing/Handling
                //Notes - set permissions appropriately... currently set to world rw... root only!
                if (mkdir("SENSITIVE", 0777)) {
                    std::system("sudo rm -rf SENSITIVE 2>/dev/null");
                    if (mkdir("SENSITIVE", 0777)) {
                        if (analytics==1) submit_event("userProcess:sensitiveDirCreateErr");
                        std::cout << "Does The SENSITIVE Directory Already Exist? Back It Up & Delete It!..." << std::endl;
                        return 1;
                    } else{
                        std::cout << "\n[*] Initialising Device\n" << std::endl;
                        //Initialise 'device' object...
                        Device device = init_device("ssh");
                        //Dump information for debugging purposes
                        if(device.can_connnect){
                            device.info();
                            std::cout << "\n[*] Preparing To Extract Data..." << std::endl;
                            sleep(5);
                            process_modules(device);
                        } else{
                            if (analytics==1) submit_event("userProcess:deviceCanConnectERR");
                            std::cout << "\n[!] Device Did Not Connect Successfully...";
                            exit(1);
                        }
                }
                return 1;
            }
            break;
        
        case 4:
            submit_event("userFeatureHit:FSAcquire");
            return 1;
            
        case 5:
            submit_event("userFeatureHit:SpiderLOCAL");
            return 1;
            
        case 6:
            submit_event("userFeatureHit:MapperLocal");
            return 1;
            
            
        default:
            if (analytics==1) submit_event("userProcess:invalidMenuClick");
            std::cout << "[!] Invalid Option!" << std::endl;
            break;
    }
        
    //Generic iproxy kill on exit - check *nix compatibility
    system("pkill iproxy");
    return 0;
}
