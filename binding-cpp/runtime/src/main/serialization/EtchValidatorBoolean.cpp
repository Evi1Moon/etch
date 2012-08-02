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

#include "serialization/EtchValidatorBoolean.h"

capu::SmartPointer<EtchValidator>* EtchValidatorBoolean::Validators()
{
  static capu::SmartPointer<EtchValidator> ret[MAX_CACHED];
  return &(ret[0]);
}

const EtchObjectType* EtchValidatorBoolean::TYPE() {
  const static EtchObjectType TYPE(EOTID_VALIDATOR_BOOLEAN, NULL);
  return &TYPE;
}

EtchValidatorBoolean::EtchValidatorBoolean(capu::uint32_t ndim)
: EtchTypeValidator(EtchValidatorBoolean::TYPE(), EtchBool::TYPE(), EtchBool::TYPE(), ndim) {

}

EtchValidatorBoolean::~EtchValidatorBoolean() {

}

capu::bool_t EtchValidatorBoolean::validate(capu::SmartPointer<EtchObject> value) {
  if (value.get() == NULL) {
    return false;
  }
  if (mExpectedType == NULL) {
    return false;
  }
  if (value->getObjectType()->equals(mExpectedType)) {
    return true;
  }
  //array handling
  if ((value->getObjectType()->isArray()) && (mExpectedType->isArray())) {
    EtchNativeArray<capu::SmartPointer<EtchObject> > *array = (EtchNativeArray<capu::SmartPointer<EtchObject> > *) value.get();
    if (array->getDim() != mNDims) {
      return false;
    }
    const EtchObjectType* type = array->getObjectType()->getObjectComponentType();
    const EtchObjectType* type2 = mExpectedType->getObjectComponentType();
    return type->equals(type2);
  }
  return false;
}

status_t EtchValidatorBoolean::validateValue(capu::SmartPointer<EtchObject> value, capu::SmartPointer<EtchObject>& result) {
  if (validate(value)) {
    result = value;
    return ETCH_OK;
  } else {
    return ETCH_ERROR;
  }
}

status_t EtchValidatorBoolean::Get(capu::uint32_t ndim, capu::SmartPointer<EtchValidator> &val) {
  if (ndim > MAX_NDIMS) {
    return ETCH_EINVAL;
  }
  if (ndim >= MAX_CACHED) {
    val = new EtchValidatorBoolean(ndim);
    return ETCH_OK;
  }
  if (Validators()[ndim].get() == NULL) {
    Validators()[ndim] = new EtchValidatorBoolean(ndim);
  }
  val = Validators()[ndim];
  return ETCH_OK;
}

status_t EtchValidatorBoolean::getElementValidator(capu::SmartPointer<EtchValidator> &val) {
  return EtchValidatorBoolean::Get(mNDims - 1, val);
}

