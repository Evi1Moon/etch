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

#include "serialization/EtchValidatorFloat.h"

capu::SmartPointer<EtchValidator> EtchValidatorFloat::mValidator[MAX_CACHED];
const EtchObjectType EtchValidatorFloat::TYPE(EOTID_VALIDATOR_FLOAT, NULL);

EtchValidatorFloat::EtchValidatorFloat(capu::uint32_t ndim)
: EtchTypeValidator(&EtchValidatorFloat::TYPE, &EtchFloat::TYPE, &EtchFloat::TYPE, ndim) {

}

EtchValidatorFloat::~EtchValidatorFloat() {

}

capu::bool_t EtchValidatorFloat::validate(EtchObject* value) {

  if (value == NULL)
    return false;
  if (mExpectedType == NULL)
    return false;
  if (value->getObjectType()->equals(mExpectedType))
    return true;

  //array handling
  if ((value->getObjectType()->isArray()) && (mExpectedType->isArray())) {

    EtchNativeArray<EtchObject*> *array = (EtchNativeArray<EtchObject*> *) value;
    const EtchObjectType* type = array->getContentType();
    const EtchObjectType* type2 = mExpectedType->getObjectComponentType();
    while ((type->getObjectComponentType() != NULL) && (mExpectedType->getObjectComponentType() != NULL)) {
      type = type->getObjectComponentType();
      type2 = type2->getObjectComponentType();
    }

    //both of them should be pointing null
    if (type->getObjectComponentType() != type2->getObjectComponentType()) {
      return false;
    }
    return type->equals(type2);
  }
  return false;
}

status_t EtchValidatorFloat::validateValue(EtchObject* value, EtchObject*& result) {
  if (validate(value)) {
    result = value;
    return ETCH_OK;
  } else {
    return ETCH_ERROR;
  }
}

status_t EtchValidatorFloat::Get(capu::uint32_t ndim, capu::SmartPointer<EtchValidator> &val) {
  if (ndim > MAX_NDIMS) {
    return ETCH_EINVAL;
  }
  if (ndim >= MAX_CACHED) {
    val = new EtchValidatorFloat(ndim);
    return ETCH_OK;
  }
  if (mValidator[ndim].get() == NULL) {
    mValidator[ndim] = new EtchValidatorFloat(ndim);
  }
  val = mValidator[ndim];
  return ETCH_OK;
}

status_t EtchValidatorFloat::getElementValidator(capu::SmartPointer<EtchValidator> &val) {
  return EtchValidatorFloat::Get(mNDims - 1, val);
}
