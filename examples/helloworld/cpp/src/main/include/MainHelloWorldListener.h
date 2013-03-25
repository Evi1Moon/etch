
// This file automatically generated by:
//   Apache Etch 1.3.0-incubating (LOCAL-0) / c 1.3.0-incubating (LOCAL-0)
//   Fri Jul 27 11:35:36 CEST 2012
// This file is automatically created for your convenience and will not be
// overwritten once it exists! Please edit this file as necessary to implement
// your service logic.



#ifndef __MAINHELLOWORLDLISTENER_H__
#define __MAINHELLOWORLDLISTENER_H__
 

#include "capu/Config.h"
#include "HelloWorldHelper.h"

namespace org_apache_etch_examples_helloworld_HelloWorld {
  /**
   * Main program for HelloWorldServer. This program makes a listener to accept
   * connections from MainHelloWorldClient.
   */
  class MainHelloWorldListener
   : public HelloWorldHelper::HelloWorldServerFactory
  {
  public:
    virtual ~MainHelloWorldListener() {}
    HelloWorldServer* newHelloWorldServer(RemoteHelloWorldClient* client);
  };
}

#endif /* __MAINHELLOWORLDLISTENER_H__ */
 