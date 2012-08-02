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

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "support/EtchPlainMailbox.h"
#include "transport/EtchPlainMailboxManager.h"
#include "serialization/EtchDefaultValueFactory.h"

class MockSession : public EtchSessionMessage {
public:

  status_t sessionMessage(capu::SmartPointer<EtchWho> receipent, capu::SmartPointer<EtchMessage> buf) {
    return ETCH_OK;
  }

  status_t sessionQuery(capu::SmartPointer<EtchObject> query, capu::SmartPointer<EtchObject> &result) {
    return ETCH_OK;
  }

  status_t sessionControl(capu::SmartPointer<EtchObject> control, capu::SmartPointer<EtchObject> value) {
    return ETCH_OK;
  }

  status_t sessionNotify(capu::SmartPointer<EtchObject> event) {
    return ETCH_OK;
  }
};

class MockTransport : public EtchTransportMessage {
public:

  status_t transportMessage(capu::SmartPointer<EtchWho> recipient, capu::SmartPointer<EtchMessage> message) {
    return ETCH_OK;
  }

  MOCK_METHOD0(getSession, EtchSessionMessage* ());

  void setSession(EtchSessionMessage* session) {

  }

  status_t transportQuery(capu::SmartPointer<EtchObject> query, capu::SmartPointer<EtchObject> *result) {
    return ETCH_OK;
  }

  status_t transportControl(capu::SmartPointer<EtchObject> control, capu::SmartPointer<EtchObject> value) {
    return ETCH_OK;
  }

  status_t transportNotify(capu::SmartPointer<EtchObject> event) {
    return ETCH_OK;
  }
};

class MockDefaultValueFactory {
public:
  EtchTypeMap types;
  EtchClass2TypeMap class2type;
  EtchDefaultValueFactory * factory;
  /**
   * Constructs the MyValueFactory.
   * @param uri
   */
public:

  MockDefaultValueFactory(EtchString uri) {
    EtchDefaultValueFactory::Init(&types, &class2type);
    factory = new EtchDefaultValueFactory(uri, &types, &class2type);
  }

  ~MockDefaultValueFactory() {
    delete factory;
    types.clear();
  }
};

TEST(EtchPlainMailboxManager, constructorTest) {
  MockTransport transport;
  MockSession session;
  MockDefaultValueFactory *factory;
  EtchString uri("tcp://127.0.0.1:4001");
  factory = new MockDefaultValueFactory(uri);
  //created value factory
  EtchURL u(uri);
  EtchMailboxManager * manager = NULL;
  manager = new EtchPlainMailboxManager(&transport, &u, NULL);

  EXPECT_TRUE(manager != NULL);
  manager->setSession(&session);

  delete manager;
  delete factory;
}

TEST(EtchPlainMailboxManager, transportMessageTest) {
  EtchString uri("tcp://127.0.0.1:4001");
  MockDefaultValueFactory *factory;
  factory = new MockDefaultValueFactory(uri);
  //initialization
  EtchType * type;
  capu::SmartPointer<EtchValidator> val = NULL;
  EtchValidatorLong::Get(0, val);

  factory->types.get("add", type);
  type->putValidator(EtchDefaultValueFactory::_mf__messageId, val);
  capu::SmartPointer<EtchMessage> message = new EtchMessage(type, factory->factory);

  capu::int64_t id;
  EXPECT_TRUE(ETCH_OK != message->getMessageId(id));
  EXPECT_TRUE(ETCH_OK != message->getInReplyToMessageId(id));

  MockTransport transport;
  MockSession session;

  EtchURL u(uri);
  EtchPlainMailboxManager * manager = NULL;
  manager = new EtchPlainMailboxManager(&transport, &u, NULL);

  manager->setSession(&session);

  EXPECT_TRUE(ETCH_OK == manager->transportMessage(NULL, message));
  EXPECT_EQ(0, manager->count());
  EXPECT_TRUE(ETCH_OK == message->getMessageId(id));
  EXPECT_TRUE(ETCH_OK != message->getInReplyToMessageId(id));

  EtchMailbox *mail;
  EXPECT_TRUE(ETCH_OK != manager->getMailbox(id, mail));

  message->clear();
  delete manager;
  delete factory;
}

TEST(EtchPlainMailboxManager, transportResultMessageTest) {
  EtchString uri("tcp://127.0.0.1:4001");
  MockDefaultValueFactory *factory;
  factory = new MockDefaultValueFactory(uri);
  //initialization
  EtchType * type;
  capu::SmartPointer<EtchValidator> val = NULL;
  EtchValidatorLong::Get(0, val);

  factory->types.get("add_result", type);
  type->putValidator(EtchDefaultValueFactory::_mf__messageId, val);
  type->putValidator(EtchDefaultValueFactory::_mf__inReplyTo, val);
  capu::SmartPointer<EtchMessage> message = new EtchMessage(type, factory->factory);

  capu::int64_t id;
  EXPECT_TRUE(ETCH_OK != message->getMessageId(id));
  EXPECT_TRUE(ETCH_OK != message->getInReplyToMessageId(id));

  message->setInReplyToMessageId(1L);

  MockTransport transport;
  MockSession session;

  EtchURL u(uri);
  EtchPlainMailboxManager * manager = NULL;
  manager = new EtchPlainMailboxManager(&transport, &u, NULL);

  manager->setSession(&session);

  EXPECT_TRUE(ETCH_OK == manager->transportMessage(NULL, message));
  EXPECT_EQ(0, manager->count());
  EXPECT_TRUE(ETCH_OK == message->getMessageId(id));
  EXPECT_TRUE(ETCH_OK == message->getInReplyToMessageId(id));
  EXPECT_TRUE(id == 1L);
  EtchMailbox *mail;
  EXPECT_TRUE(ETCH_OK != manager->getMailbox(id, mail));

  message->clear();
  delete manager;
  delete factory;
}

TEST(EtchPlainMailboxManager, transportCallTest) {
  EtchString uri("tcp://127.0.0.1:4001");
  MockDefaultValueFactory *factory;
  factory = new MockDefaultValueFactory(uri);
  //initialization
  EtchType * type;
  capu::SmartPointer<EtchValidator> val = NULL;
  EtchValidatorLong::Get(0, val);

  factory->types.get("add", type);
  type->putValidator(EtchDefaultValueFactory::_mf__messageId, val);
  capu::SmartPointer<EtchMessage> message = new EtchMessage(type, factory->factory);

  capu::int64_t id;
  EXPECT_TRUE(ETCH_OK != message->getMessageId(id));
  EXPECT_TRUE(ETCH_OK != message->getInReplyToMessageId(id));

  MockTransport transport;
  MockSession session;

  EtchURL u(uri);
  EtchPlainMailboxManager * manager = NULL;
  manager = new EtchPlainMailboxManager(&transport, &u, NULL);

  manager->setSession(&session);
  //in order to notify upper layers that the connection is open
  manager->sessionNotify(new EtchString(EtchSession::UP));

  EtchMailbox *mail;
  EXPECT_TRUE(ETCH_OK == manager->transportCall(NULL, message, mail));
  EXPECT_EQ(1, manager->count());
  EXPECT_TRUE(ETCH_OK == message->getMessageId(id));
  EXPECT_TRUE(ETCH_OK != message->getInReplyToMessageId(id));

  EXPECT_TRUE(ETCH_OK == manager->getMailbox(id, mail));

  message->clear();
  delete manager;
  delete factory;
}

TEST(EtchPlainMailboxManager, replicatedTransportCallTest) {
  EtchString uri("tcp://127.0.0.1:4001");
  MockDefaultValueFactory *factory;
  factory = new MockDefaultValueFactory(uri);
  //initialization
  EtchType * type;
  capu::SmartPointer<EtchValidator> val = NULL;
  EtchValidatorLong::Get(0, val);

  factory->types.get("add", type);
  type->putValidator(EtchDefaultValueFactory::_mf__messageId, val);
  capu::SmartPointer<EtchMessage> message = new EtchMessage(type, factory->factory);

  capu::int64_t id;
  EXPECT_TRUE(ETCH_OK != message->getMessageId(id));
  EXPECT_TRUE(ETCH_OK != message->getInReplyToMessageId(id));

  MockTransport transport;
  MockSession session;

  // test sending a call message that has already been sent (has a message id)
  message->setMessageId(2);

  EtchURL u(uri);
  EtchPlainMailboxManager * manager = NULL;
  manager = new EtchPlainMailboxManager(&transport, &u, NULL);

  manager->setSession(&session);
  //in order to notify upper layers that the connection is open
  manager->sessionNotify(new EtchString(EtchSession::UP));

  EtchMailbox *mail;
  EXPECT_TRUE(ETCH_ERROR == manager->transportCall(NULL, message, mail));
  //should not create a mailbox
  EXPECT_EQ(0, manager->count());
  EXPECT_TRUE(ETCH_OK == message->getMessageId(id));
  EXPECT_TRUE(ETCH_OK != message->getInReplyToMessageId(id));
  //there should be no mailbox
  EXPECT_TRUE(ETCH_OK != manager->getMailbox(id, mail));

  message->clear();
  delete manager;
  delete factory;
}

TEST(EtchPlainMailboxManager, sessionMessageTest) {
  EtchString uri("tcp://127.0.0.1:4001");
  MockDefaultValueFactory *factory;
  factory = new MockDefaultValueFactory(uri);
  //initialization
  EtchType * type;
  EtchType * replyType;
  capu::SmartPointer<EtchValidator> val = NULL;
  EtchValidatorLong::Get(0, val);

  factory->types.get("add_result", replyType);
  factory->types.get("add", type);
  type->putValidator(EtchDefaultValueFactory::_mf__messageId, val);
  replyType->putValidator(EtchDefaultValueFactory::_mf__inReplyTo, val);
  replyType->putValidator(EtchDefaultValueFactory::_mf__messageId, val);
  capu::SmartPointer<EtchMessage> message = new EtchMessage(type, factory->factory);

  capu::int64_t id;
  EXPECT_TRUE(ETCH_OK != message->getMessageId(id));
  EXPECT_TRUE(ETCH_OK != message->getInReplyToMessageId(id));

  MockTransport transport;
  MockSession session;

  EtchURL u(uri);
  EtchPlainMailboxManager * manager = NULL;
  manager = new EtchPlainMailboxManager(&transport, &u, NULL);

  manager->setSession(&session);
  //in order to notify upper layers that the connection is open
  manager->sessionNotify(new EtchString(EtchSession::UP));

  //perform the call
  EtchMailbox *mail;
  EXPECT_TRUE(ETCH_OK == manager->transportCall(NULL, message, mail));
  EXPECT_EQ(1, manager->count());
  EXPECT_TRUE(ETCH_OK == message->getMessageId(id));
  EXPECT_TRUE(ETCH_OK != message->getInReplyToMessageId(id));
  //get the created mailbox
  EXPECT_TRUE(ETCH_OK == manager->getMailbox(id, mail));

  capu::SmartPointer<EtchMessage> replymessage;
  //create a reply message
  message->createReplyMessage(replyType, replymessage);
  EXPECT_TRUE(ETCH_OK == replymessage->getInReplyToMessageId(id));
  capu::SmartPointer<EtchMessage> replymess = replymessage;

  //test the notification mechanism of mailbox manager
  EXPECT_TRUE(ETCH_OK == manager->sessionMessage(NULL, replymess));
  EtchMailbox::EtchElement* x;
  //check if the reply message is delivered to the correct mailbox
  EXPECT_TRUE(ETCH_OK == mail->read(x));
  EXPECT_TRUE(NULL == x->mSender.get());
  EXPECT_TRUE(replymessage.get() == x->mMsg.get());

  //deallocations
  delete x;
  delete manager;
  delete factory;
}

