//
//  src_raster.cpp
//  src.raster
//
//  Created by 杨丰 on 2020/11/10.
//

#include <iostream>
#include "src_raster.hpp"
#include "src_rasterPriv.hpp"

void src_raster::HelloWorld(const char * s)
{
    src_rasterPriv *theObj = new src_rasterPriv;
    theObj->HelloWorldPriv(s);
    delete theObj;
};

void src_rasterPriv::HelloWorldPriv(const char * s) 
{
    std::cout << s << std::endl;
};

