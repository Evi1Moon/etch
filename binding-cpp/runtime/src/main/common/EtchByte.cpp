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

#include "common/EtchByte.h"

EtchByte::EtchByte()
: EtchObject(EtchByte::TYPE_ID)
, mValue(0){
}

EtchByte::EtchByte(capu::int8_t value)
: EtchObject(EtchByte::TYPE_ID)
, mValue(value){
}

void EtchByte::set(capu::int8_t value){
  mValue = value;
}

capu::int8_t EtchByte::get(){
  return mValue;
}

capu::bool_t EtchByte::equals(const EtchObject * other) const{
  if (other == NULL)
    return false;
  else if (other->getObjectTypeId() != EtchByte::TYPE_ID)
    return false;
  EtchByte * a = (EtchByte*) other;
  return (a->mValue == this->mValue);
}

capu::uint64_t EtchByte::getHashCode() const{
  //For better distribution
  capu::uint64_t result = (capu::uint64_t) ((capu::int64_t) mValue + 128); 
  return result;
}
