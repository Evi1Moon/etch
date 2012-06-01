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

#include "common/EtchInt32.h"

const EtchObjectType* EtchInt32::TYPE() {
  const static EtchObjectType TYPE(EOTID_INT32, NULL);
  return &TYPE;
}

EtchInt32::EtchInt32()
: EtchObject(EtchInt32::TYPE())
, mValue(0){
}

EtchInt32::EtchInt32(capu::int32_t value)
: EtchObject(EtchInt32::TYPE())
, mValue(value){
}

void EtchInt32::set(capu::int32_t value){
  mValue = value;
}

capu::int32_t EtchInt32::get(){
  return mValue;
}

capu::uint64_t EtchInt32::getHashCode() const{
  return static_cast <capu::uint64_t> (mValue);
}

capu::bool_t EtchInt32::equals(const EtchObject * other) const{
  if (other == NULL)
    return false;
  else if (!other->getObjectType()->equals(EtchInt32::TYPE()))
    return false;
  EtchInt32 *a = (EtchInt32*) other;
  return (a->mValue == this->mValue);
}
