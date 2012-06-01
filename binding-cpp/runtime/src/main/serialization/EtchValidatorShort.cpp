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

#include "serialization/EtchValidatorShort.h"

capu::SmartPointer<EtchValidator> EtchValidatorShort::mValidator[MAX_CACHED];
const EtchObjectType EtchValidatorShort::TYPE(EOTID_VALIDATOR_SHORT, NULL);

EtchValidatorShort::EtchValidatorShort(capu::uint32_t ndim)
: EtchTypeValidator(&EtchValidatorShort::TYPE, &EtchShort::TYPE, &EtchShort::TYPE, ndim) {

}

EtchValidatorShort::~EtchValidatorShort() {

}

capu::bool_t EtchValidatorShort::validate(EtchObject* value) {
  if (value == NULL)
    return false;

  if (mExpectedType == NULL)
    return false;

  if (value->getObjectType()->equals(mExpectedType))
    return true;

  if ((value->getObjectType()->equals(&EtchShort::TYPE)) ||
          (value->getObjectType()->equals(&EtchByte::TYPE)))
    return true;

  if (value->getObjectType()->equals(&EtchInt32::TYPE)) {
    EtchInt32 *v = (EtchInt32 *) value;
    return ((v->get() >= capu::NumericLimitMin<capu::int16_t > ()) && (v->get() <= capu::NumericLimitMax<capu::int16_t > ()));
  }

  if (value->getObjectType()->equals(&EtchLong::TYPE)) {
    EtchLong *v = (EtchLong *) value;
    return ((v->get() >= capu::NumericLimitMin<capu::int16_t > ()) && (v->get() <= capu::NumericLimitMax<capu::int16_t > ()));
  }

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

status_t EtchValidatorShort::validateValue(EtchObject* value, EtchObject*& result) {
  if (validate(value)) {
    if ((value->getObjectType()->equals(&EtchShort::TYPE)) || (mNDims > 0)) {
      result = value;
      return ETCH_OK;
    } else {
      if (value->getObjectType()->equals(&EtchInt32::TYPE)) {
        EtchInt32 *v = (EtchInt32 *) value;
        result = (EtchObject*) (new EtchShort((capu::int16_t)v->get()));
        return ETCH_OK;
      }

      if (value->getObjectType()->equals(&EtchByte::TYPE)) {
        EtchByte *v = (EtchByte *) value;
        result = (EtchObject*) (new EtchShort((capu::int16_t)v->get()));
        return ETCH_OK;
      }

      if (value->getObjectType()->equals(&EtchLong::TYPE)) {
        EtchLong *v = (EtchLong *) value;
        result = (EtchObject*) (new EtchShort((capu::int16_t)v->get()));
        return ETCH_OK;
      }
      return ETCH_ERROR;
    }
  } else {
    return ETCH_ERROR;
  }
}

status_t EtchValidatorShort::Get(capu::uint32_t ndim, capu::SmartPointer<EtchValidator> &val) {
  if (ndim > MAX_NDIMS) {
    return ETCH_EINVAL;
  }
  if (ndim >= MAX_CACHED) {
    val = new EtchValidatorShort(ndim);
    return ETCH_OK;
  }
  if (mValidator[ndim].get() == NULL) {
    mValidator[ndim] = new EtchValidatorShort(ndim);
  }
  val = mValidator[ndim];
  return ETCH_OK;
}

status_t EtchValidatorShort::getElementValidator(capu::SmartPointer<EtchValidator> &val) {
  return EtchValidatorShort::Get(mNDims - 1, val);
}