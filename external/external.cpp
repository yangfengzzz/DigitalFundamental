//
//  external.cpp
//  external
//
//  Created by 杨丰 on 2020/11/10.
//

#include <iostream>
#include "external.hpp"
#include "externalPriv.hpp"

void external::HelloWorld(const char * s)
{
    externalPriv *theObj = new externalPriv;
    theObj->HelloWorldPriv(s);
    delete theObj;
};

void externalPriv::HelloWorldPriv(const char * s) 
{
    std::cout << s << std::endl;
};

