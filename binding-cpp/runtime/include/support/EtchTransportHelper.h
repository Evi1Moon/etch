/* $Id$
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements. See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to you under the Apache License, Version
 * 2.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __ETCHTRANSPORTHELPER_H__
#define __ETCHTRANSPORTHELPER_H__

#include "common/EtchString.h"
#include "util/EtchResources.h"

/**
 * Class to help construct transport stacks.
 */
class EtchTransportHelper
{
  ///////////////
  // RESOURCES //
  ///////////////
public:

  /**
   * Destructor
   */
  virtual ~EtchTransportHelper();

  /** The Pool to use to execute queued async receiver messages. */
  static EtchString QUEUED_POOL;

  /** The Pool to use to execute free async receiver messages. */
  static EtchString FREE_POOL;

  /** Binary transport format */
  static EtchString BINARY;

  /** Xml transport format */
  static EtchString XML;

  ///////////////
  // UTILITIES //
  ///////////////

  /**
   * Initializes standard resources.
   *
   * @param resources some initial values for resources. May be null to accept
   * all the defaults.
   * @param copy of resources initialized with default values for standard
   * items.
   * @return an error if there is a problem
   */
  static status_t initResources( EtchResources* resources, EtchResources*& result );
  
  /**
   * Dealloc resources 
   */
  static status_t destroyResources(EtchResources* resources);
};

#endif /* __ETCHTRANSPORTHELPER_H__ */