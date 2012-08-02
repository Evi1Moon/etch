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

#include "serialization/EtchValidatorStructValue.h"

const EtchObjectType* EtchValidatorStructValue::TYPE() {
  const static EtchObjectType TYPE(EOTID_VALIDATOR_STRUCT_VALUE, NULL);
  return &TYPE;
}


EtchValidatorStructValue::EtchValidatorStructValue(EtchType* type, capu::uint32_t ndim)
: EtchTypeValidator(EtchValidatorStructValue::TYPE(), EtchStructValue::TYPE(), EtchStructValue::TYPE(), ndim), mType(type) {

}

EtchValidatorStructValue::~EtchValidatorStructValue() {

}

status_t EtchValidatorStructValue::validateValue(capu::SmartPointer<EtchObject> value, capu::SmartPointer<EtchObject>& result) {
  if (validate(value)) {
    result = value;
    return ETCH_OK;
  } else {
    return ETCH_ERROR;
  }
}

capu::bool_t EtchValidatorStructValue::validate(capu::SmartPointer<EtchObject> value) {
  if (mExpectedType == NULL)
    return false;
  if (value.get() == NULL)
    return false;
  if (value->getObjectType()->equals(mExpectedType))
    return true;
  //handle array
  if ((value->getObjectType()->isArray()) && (mExpectedType->isArray())) {
    //TODO: move getDim to EtchObjectType and set it dynamically during creation of the object
    EtchNativeArray<capu::SmartPointer<EtchObject> > *array = (EtchNativeArray<capu::SmartPointer<EtchObject> > *) value.get();
    if (array->getDim() != mNDims) {
      return false;
    }
    const EtchObjectType* type = array->getObjectType()->getObjectComponentType();
    const EtchObjectType* type2 = mExpectedType->getObjectComponentType();
    return type->equals(type2);
  }
  return (dynamic_cast<EtchValidatorStructValue*> (value.get()) != NULL);
}

status_t EtchValidatorStructValue::getElementValidator(capu::SmartPointer<EtchValidator> &val) {
  return Get(mType, mNDims - 1, val);
}

status_t EtchValidatorStructValue::Get(EtchType* type, capu::uint32_t ndim, capu::SmartPointer<EtchValidator> &val) {
  if (ndim > MAX_NDIMS)
    return ETCH_EINVAL;
  val = capu::SmartPointer <EtchValidator > (new EtchValidatorStructValue(type, ndim));
  return ETCH_OK;
}

EtchType* EtchValidatorStructValue::getType() {
  return mType;
}