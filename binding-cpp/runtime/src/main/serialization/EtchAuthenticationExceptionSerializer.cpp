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

#include "serialization/EtchAuthenticationExceptionSerializer.h"

const EtchString& EtchAuthenticationExceptionSerializer::FIELD_NAME() {
  static const EtchString name("msg");
  return name;
}

EtchAuthenticationExceptionSerializer::EtchAuthenticationExceptionSerializer(EtchType* type, EtchField* field)
: mField(*field), mType(type) {

}

EtchAuthenticationExceptionSerializer::~EtchAuthenticationExceptionSerializer() {

}

status_t EtchAuthenticationExceptionSerializer::importValue(EtchStructValue* value, capu::SmartPointer<EtchObject> &result) {
  if (value == NULL) {
    return ETCH_EINVAL;
  } else if (!value->isType(mType)) {
    return ETCH_EINVAL;
  }

  capu::SmartPointer<EtchObject> tmp;
  if (value->get(mField, &tmp) != ETCH_OK)
    return ETCH_ERROR;

  EtchString str = *((EtchString*) tmp.get());
  result = new EtchAuthenticationException(str);
  return ETCH_OK;
}

status_t EtchAuthenticationExceptionSerializer::exportValue(EtchValueFactory* vf, capu::SmartPointer<EtchObject> value, EtchStructValue*& result) {
  if ((value.get() == NULL) || (vf == NULL))
    return ETCH_EINVAL;
  if (value->getObjectType() != EtchAuthenticationException::TYPE())
    return ETCH_EINVAL;
  result = new EtchStructValue(mType, vf);
  EtchString *str = new EtchString(((EtchAuthenticationException*) value.get())->getErrorMessage());
  if (result->put(mField, str) != ETCH_OK)
    return ETCH_ERROR;

  return ETCH_OK;
}

status_t EtchAuthenticationExceptionSerializer::Init(EtchType* type, EtchClass2TypeMap* class2type) {
  status_t result;
  EtchField field_ptr;
  result = type->getField(FIELD_NAME(), &field_ptr);
  if (result != ETCH_OK)
    return result;
  class2type->put(EtchAuthenticationException::TYPE(), type);
  type->setComponentType(EtchAuthenticationException::TYPE());
  //set the import export helper
  type->setImportExportHelper(new EtchAuthenticationExceptionSerializer(type, &field_ptr));
  capu::SmartPointer<EtchValidator> tmp;
  result = EtchValidatorString::Get(0, tmp);
  if (result != ETCH_OK)
    return result;
  result = type->putValidator(field_ptr, tmp);
  if (result != ETCH_OK)
    return result;
  type->lock();
  return ETCH_OK;
}
