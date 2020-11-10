//
//  src_tracing.cpp
//  src.tracing
//
//  Created by 杨丰 on 2020/11/10.
//

#include <iostream>
#include "src_tracing.hpp"
#include "src_tracingPriv.hpp"

void src_tracing::HelloWorld(const char * s)
{
    src_tracingPriv *theObj = new src_tracingPriv;
    theObj->HelloWorldPriv(s);
    delete theObj;
};

void src_tracingPriv::HelloWorldPriv(const char * s) 
{
    std::cout << s << std::endl;
};

