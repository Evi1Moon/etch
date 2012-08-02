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
#include "transport/EtchPlainMailboxManager.h"

EtchPlainMailboxManager::EtchPlainMailboxManager(EtchTransportMessage* transport, EtchURL* url, EtchResources* resources)
: mSession(NULL), mTransport(transport) {
  mTransport->setSession(this);
}

EtchPlainMailboxManager::~EtchPlainMailboxManager() {
  EtchHashTable<EtchLong, EtchMailbox*>::Iterator it = mMailboxes.begin();
  EtchHashTable<EtchLong, EtchMailbox*>::Pair p;
  while (it.hasNext()) {
    it.next(&p);
    p.second->closeDelivery();
    delete p.second;
  }
}

EtchTransportMessage* EtchPlainMailboxManager::getTransport() {
  return mTransport;
}

status_t EtchPlainMailboxManager::redeliver(capu::SmartPointer<EtchWho> sender, capu::SmartPointer<EtchMessage> msg) {
  return mSession->sessionMessage(sender, msg);
}

status_t EtchPlainMailboxManager::registerMailbox(EtchMailbox* mb) {
  if(mb == NULL) {
    return ETCH_EINVAL;
  }

  const EtchLong msgid = mb->getMessageId();

  mMutex.lock();
  if (!mUp) {
    mMutex.unlock();
    return ETCH_EINVAL;
  }
  
  EtchMailbox* tmp = NULL;
  if (mMailboxes.get(msgid, &tmp) != ETCH_ENOT_EXIST) {
    mMutex.unlock();
    return ETCH_EINVAL;
  }

  mMailboxes.put(msgid, mb);
  mMutex.unlock();
  return ETCH_OK;
}

status_t EtchPlainMailboxManager::unregisterMailbox(EtchMailbox* mb) {
  if(mb == NULL) {
    return ETCH_EINVAL;
  }

  EtchMailbox* tmp = NULL;
  mMailboxes.remove(mb->getMessageId(), &tmp);
  return ETCH_OK;
}

status_t EtchPlainMailboxManager::getMailbox(EtchLong msgid, EtchMailbox*& result) {
  return mMailboxes.get(msgid, &result);
}

status_t EtchPlainMailboxManager::sessionMessage(capu::SmartPointer<EtchWho> sender, capu::SmartPointer<EtchMessage> msg) {
  capu::int64_t msgid;
  if(msg->getInReplyToMessageId(msgid) == ETCH_OK) {
    EtchMailbox* mb = NULL;
    if (getMailbox(msgid, mb) == ETCH_OK) {
      return mb->message(sender, msg);
    }
    return ETCH_ERROR;
  }
  // no msgid - pass off to session
  return mSession->sessionMessage(sender, msg);
}

status_t EtchPlainMailboxManager::transportCall(capu::SmartPointer<EtchWho> recipient, capu::SmartPointer<EtchMessage> msg, EtchMailbox*& result) {
  capu::int64_t tmp;
  if (msg->getMessageId(tmp) == ETCH_OK) {
    // message has already been sent
    return ETCH_ERROR;
  }

  if (msg->getInReplyToMessageId(tmp) == ETCH_OK) {
    // message is marked as a reply
    return ETCH_ERROR;
  }

  capu::int64_t msgid = mIdGen.next();
  if (msg->setMessageId(msgid) != ETCH_OK) {
    return ETCH_ERROR;
  }

  EtchMailbox *mb = new EtchPlainMailbox(this, msgid);
  if (registerMailbox(mb) != ETCH_OK) {
    delete mb;
    return ETCH_ERROR;
  }

  if (mTransport->transportMessage(recipient, msg) == ETCH_OK) {
    result = mb;
    return ETCH_OK;
  } else {
    unregisterMailbox(mb);
    delete mb;
    return ETCH_ERROR;
  }
}

status_t EtchPlainMailboxManager::transportMessage(capu::SmartPointer<EtchWho> recipient, capu::SmartPointer<EtchMessage> message) {
  capu::int64_t tmp;
  if (message->getMessageId(tmp) == ETCH_OK) {
    // message has already been sent
    return ETCH_ERROR;
  }

  if(message->setMessageId(mIdGen.next()) != ETCH_OK) {
    return ETCH_ERROR;
  }
  return mTransport->transportMessage(recipient, message);
}

status_t EtchPlainMailboxManager::sessionQuery(capu::SmartPointer<EtchObject> query, capu::SmartPointer<EtchObject> &result) {
  return mSession->sessionQuery(query, result);
}

status_t EtchPlainMailboxManager::sessionControl(capu::SmartPointer<EtchObject> control, capu::SmartPointer<EtchObject> value) {
  return mSession->sessionControl(control, value);
}

status_t EtchPlainMailboxManager::sessionNotify(capu::SmartPointer<EtchObject> event) {
  if(event->equals(&EtchSession::UP)) {
    mUp = true;
  } else
  if(event->equals(&EtchSession::DOWN)) {
    mUp = false;
    EtchHashTable<EtchLong, EtchMailbox*>::Iterator it = mMailboxes.begin();
    EtchHashTable<EtchLong, EtchMailbox*>::Pair p;
    while (it.hasNext()) {
      it.next(&p);
      p.second->closeDelivery();
    }
  }
  return mSession->sessionNotify(event);
}

status_t EtchPlainMailboxManager::transportQuery(capu::SmartPointer<EtchObject> query, capu::SmartPointer<EtchObject> *result) {
  return mTransport->transportQuery(query, result);
}

status_t EtchPlainMailboxManager::transportControl(capu::SmartPointer<EtchObject> control, capu::SmartPointer<EtchObject> value) {
  return mTransport->transportControl(control, value);
}

status_t EtchPlainMailboxManager::transportNotify(capu::SmartPointer<EtchObject> event) {
  return mTransport->transportNotify(event);
}

EtchSessionMessage* EtchPlainMailboxManager::getSession() {
  return mSession;
}

void EtchPlainMailboxManager::setSession(EtchSessionMessage* session) {
  mSession = session;
}

capu::uint64_t EtchPlainMailboxManager::count() {
  return mMailboxes.count();
}
