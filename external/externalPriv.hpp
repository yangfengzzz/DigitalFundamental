//
//  externalPriv.hpp
//  external
//
//  Created by 杨丰 on 2020/11/10.
//

/* The classes below are not exported */
#pragma GCC visibility push(hidden)

class externalPriv
{
    public:
    void HelloWorldPriv(const char *);
};

#pragma GCC visibility pop
