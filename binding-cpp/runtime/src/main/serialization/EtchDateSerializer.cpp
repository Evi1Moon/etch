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

#include "serialization/EtchDateSerializer.h"

const EtchString EtchDateSerializer::FIELD_NAME("dateTime");

EtchDateSerializer::EtchDateSerializer(EtchType* type, EtchField* field)
: mField(*field), mType(type) {

}

EtchDateSerializer::~EtchDateSerializer() {

}

status_t EtchDateSerializer::importValue(EtchStructValue* value, capu::SmartPointer<EtchObject> &result) {
  if (value == NULL) {
    return ETCH_EINVAL;
  } else if (!value->isType(mType)) {
    return ETCH_EINVAL;
  }

  capu::SmartPointer<EtchObject> tmp;
  if (value->get(mField, &tmp) != ETCH_OK)
    return ETCH_ERROR;

  EtchLong date = *((EtchLong*) tmp.get());
  result = new EtchDate((capu::time_t)date.get());
  return ETCH_OK;
}

status_t EtchDateSerializer::exportValue(EtchValueFactory* vf, capu::SmartPointer<EtchObject> value, EtchStructValue*& result) {
  if ((value.get() == NULL) || (vf == NULL))
    return ETCH_EINVAL;
  if (value->getObjectType() != EtchDate::TYPE())
    return ETCH_EINVAL;
  result = new EtchStructValue(mType, vf);
  EtchLong *date = new EtchLong(((EtchDate*) value.get())->get());
  if (result->put(mField, date) != ETCH_OK)
    return ETCH_ERROR;

  return ETCH_OK;
}

status_t EtchDateSerializer::Init(EtchType* type, EtchClass2TypeMap* class2type) {
  status_t result;
  EtchField field_ptr;
  result = type->getField(FIELD_NAME, &field_ptr);
  if (result != ETCH_OK)
    return result;
  class2type->put(EtchDate::TYPE(), type);
  type->setComponentType(EtchDate::TYPE());

  //set the import export helper
  type->setImportExportHelper(new EtchDateSerializer(type, &field_ptr));
  capu::SmartPointer<EtchValidator> val;
  result = EtchValidatorLong::Get(0, val);
  if (result != ETCH_OK)
    return result;
  result = type->putValidator(field_ptr, val);
  if (result != ETCH_OK)
    return result;
  type->lock();
  return ETCH_OK;
}
