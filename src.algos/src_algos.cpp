//
//  src_algos.cpp
//  src.algos
//
//  Created by 杨丰 on 2020/11/10.
//

#include <iostream>
#include "src_algos.hpp"
#include "src_algosPriv.hpp"

void src_algos::HelloWorld(const char * s)
{
    src_algosPriv *theObj = new src_algosPriv;
    theObj->HelloWorldPriv(s);
    delete theObj;
};

void src_algosPriv::HelloWorldPriv(const char * s) 
{
    std::cout << s << std::endl;
};

