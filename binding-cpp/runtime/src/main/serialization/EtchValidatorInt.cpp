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

#include "serialization/EtchValidatorInt.h"

capu::SmartPointer<EtchValidator>* EtchValidatorInt::Validators()
{
  static capu::SmartPointer<EtchValidator> ret[MAX_CACHED];
  return &(ret[0]);
}

const EtchObjectType* EtchValidatorInt::TYPE() {
  const static EtchObjectType TYPE(EOTID_VALIDATOR_INT, NULL);
  return &TYPE;
}

EtchValidatorInt::EtchValidatorInt(capu::uint32_t ndim)
: EtchTypeValidator(EtchValidatorInt::TYPE(), EtchInt32::TYPE(), EtchInt32::TYPE(), ndim) {

}

EtchValidatorInt::~EtchValidatorInt() {

}

capu::bool_t EtchValidatorInt::validate(capu::SmartPointer<EtchObject> value) {
  if (value.get() == NULL)
    return false;

  if (mExpectedType == NULL)
    return false;

  if (value->getObjectType()->equals(mExpectedType))
    return true;

  if ((value->getObjectType()->equals(EtchShort::TYPE())) ||
          (value->getObjectType()->equals(EtchByte::TYPE())))
    return true;

  if (value->getObjectType()->equals(EtchLong::TYPE())) {
    EtchLong *v = (EtchLong *) value.get();
    return ((v->get() >= capu::NumericLimitMin<capu::int32_t > ()) && (v->get() <= capu::NumericLimitMax<capu::int32_t > ()));
  }

  //handle array
  if ((value->getObjectType()->isArray()) && (mExpectedType->isArray())) {
    EtchNativeArray<EtchObject*> *array = (EtchNativeArray<EtchObject*> *) value.get();
    if (array->getDim() != mNDims) {
      return false;
    }
    const EtchObjectType* type = array->getObjectType()->getObjectComponentType();
    const EtchObjectType* type2 = mExpectedType->getObjectComponentType();
    return type->equals(type2);
  }

  return false;
}

status_t EtchValidatorInt::validateValue(capu::SmartPointer<EtchObject> value, capu::SmartPointer<EtchObject>& result) {
  if (validate(value)) {
    if ((value->getObjectType()->equals(EtchInt32::TYPE())) || (mNDims > 0)) {
      result = value;
      return ETCH_OK;
    } else {
      if (value->getObjectType()->equals(EtchShort::TYPE())) {
        EtchShort *v = (EtchShort *) value.get();
        result = new EtchInt32((capu::int32_t)v->get());
        return ETCH_OK;
      }

      if (value->getObjectType()->equals(EtchByte::TYPE())) {
        EtchByte *v = (EtchByte *) value.get();
        result = new EtchInt32((capu::int32_t)v->get());
        return ETCH_OK;
      }

      if (value->getObjectType()->equals(EtchLong::TYPE())) {
        EtchLong *v = (EtchLong *) value.get();
        result = new EtchInt32((capu::int32_t)v->get());
        return ETCH_OK;
      }
      return ETCH_ERROR;
    }
  } else {
    return ETCH_ERROR;
  }
}

status_t EtchValidatorInt::Get(capu::uint32_t ndim, capu::SmartPointer<EtchValidator> &val) {
  if (ndim > MAX_NDIMS) {
    return ETCH_EINVAL;
  }
  if (ndim >= MAX_CACHED) {
    val = new EtchValidatorInt(ndim);
    return ETCH_OK;
  }
  if (Validators()[ndim].get() == NULL) {
    Validators()[ndim] = new EtchValidatorInt(ndim);
  }
  val = Validators()[ndim];
  return ETCH_OK;
}

status_t EtchValidatorInt::getElementValidator(capu::SmartPointer<EtchValidator> &val) {
  return EtchValidatorInt::Get(mNDims - 1, val);
}
