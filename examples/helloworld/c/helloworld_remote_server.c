/*
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 */


// This file automatically generated by:
//   Apache Etch 1.1.0-incubating (LOCAL-0) / c 1.1.0-incubating (LOCAL-0)
//   Fri Aug 28 15:58:20 CEST 2009
// This file is automatically created and should not be edited!

/*
 * helloworld_remote_server.c
 * generated remote procedure calls.
 */

#include "helloworld_remote_server.h"
#include "etch_plain_mailbox_manager.h"
#include "etch_svcobj_masks.h"
#include "etch_objecttypes.h"
#include "etch_exception.h"
#include "etch_url.h" 
#include "etch_log.h" 
#include "etch_general.h"

unsigned short CLASSID_HELLOWORLD_REMOTE_SERVER;	
	
char* HELLOWORLD_ETCHREMS = "REMS";

	
etch_string* helloworld_remote_server_say_hello (void* thisAsVoid, helloworld_user* to_whom);
i_mailbox* helloworld_remote_begin_server_say_hello (void* thisAsVoid, helloworld_user* to_whom);
etch_string* helloworld_remote_end_server_say_hello(void*, i_mailbox*);


int destroy_helloworld_remote_server (void*);

/* - - - - - - - - - -   
 * instantiation etc.
 * - - - - - - - - - -  
 */

/**
 * new_helloworld_remote_server()
 * helloworld_remote_server constructor.
 * @param thisx owner, optional, null in practice.
 * @param ids delivery service interface, caller retains.
 * @param vf service specific value factory, caller retains.
 */
helloworld_remote_server* new_helloworld_remote_server (void* thisx, i_delivery_service* ids, etch_value_factory* vf)
{
    helloworld_remote_server* rs = (helloworld_remote_server*) new_object (sizeof(helloworld_remote_server), 
        ETCHTYPEB_REMOTESERVER, get_dynamic_classid_unique(&CLASSID_HELLOWORLD_REMOTE_SERVER));

    ((etch_object*)rs)->destroy = destroy_helloworld_remote_server;

    rs->remote_base = new_helloworld_remote (thisx, ids, vf, NULL);
    rs->remote_base->remote_type = ETCH_REMOTETYPE_SERVER;

    /* 2/3/09 now passing perf interface not owned by server_base */
    rs->server_base = new_helloworld_remote_server_base (thisx, rs->remote_base->ihelloworld);  

    rs->vf = (helloworld_valufact*) rs->remote_base->vf;

    rs->say_hello = helloworld_remote_server_say_hello;

    rs->async_begin_say_hello = helloworld_remote_begin_server_say_hello;
    rs->async_end_say_hello = helloworld_remote_end_server_say_hello;

    return rs;
}

/**
 * destroy_helloworld_server_base()
 * i_helloworld_server destructor.
 */
int destroy_helloworld_remote_server (void* thisAsVoid)
{
    helloworld_remote_server* thisx = (helloworld_remote_server*)thisAsVoid;
    if (NULL == thisx) return -1;

    if (!is_etchobj_static_content(thisx))
    {
        etch_object_destroy(thisx->remote_base);
		thisx->remote_base = NULL;
		etch_object_destroy(thisx->server_base);
		thisx->server_base = NULL;
		etch_object_destroy(thisx->client_factory);
		thisx->client_factory = NULL;
    }

    return destroy_objectex((etch_object*)thisx);
}


/**
 * perf_remote_dispose_mailbox()
 * dispose of mailbox after use.
 * this is the common means of disposing of a mailbox when we're done with it.
 * this would intuitively be part of base class code but we don't have one.
 * @param thisx the remote server this.
 * @param pibox pointer to pointer to the mailbox interface, passed indirectly
 * such that this method can null out the caller's mailbox reference.
 * @return 0 if mailbox was successfullly closed, otherwise -1. 
 * caller's i_mailbox reference is nulled out regardless of result.
 */
int helloworld_remote_server_dispose_mailbox (helloworld_remote_server* thisx, i_mailbox** pibox)
{
    int result = 0;
    i_mailbox* ibox = 0;
    if (!pibox || !*pibox) return -1;
    ibox = *pibox;
    *pibox = NULL;  /* null out caller's reference */
    
    if (0 != (result = ibox->close_read (ibox)))
    {  
        /* we should not need this failsafe unregister if close_read() 
         * is reliable, since close_read() will do the unregister */
        i_mailbox_manager* imgr = ibox->manager(ibox);
        ETCH_LOG(HELLOWORLD_ETCHREMS, ETCH_LOG_ERROR, "could not close mailbox %x\n", ibox);
        if (imgr) result = imgr->unregister(imgr, ibox);
    }
 
    /* mailbox manager does not destroy the unregistered mailbox since it is    
     * owned by whoever registered it, that being us, so we destroy it here. 
     * debug heap issue note: this is/was the spot.
     */
    etch_object_destroy(ibox);   
    return result;
}

etch_stub* helloworld_remote_server_get_stubbase (helloworld_remote_server* thisx)
{
    etch_stub* stub = NULL;
    xxxx_either_stub* clistub = (xxxx_either_stub*) thisx->client_factory->stub;
    stub = clistub? clistub->stub_base: NULL;
    return stub;
}


/**
 * helloworld_remote_set_session_notify()
 * convenience to override remote server's session_notify().
 * @return the session_notify function that was overridden.
 */
etch_session_notify helloworld_remote_server_set_session_notify(helloworld_remote_server* thisx, etch_session_notify newfunc)
{
    etch_session_notify oldfunc = NULL;
    etch_stub* stub = helloworld_remote_server_get_stubbase(thisx);
    if (NULL == stub || NULL == stub->impl_callbacks) return NULL;
    oldfunc = stub->impl_callbacks->_session_notify;
    stub->impl_callbacks->_session_notify = newfunc;
    return oldfunc;
}


/**
 * helloworld_remote_set_session_control()
 * convenience to override remote server's session_control().
 * @return the session_control function that was overridden.
 */
etch_session_control helloworld_remote_server_set_session_control(helloworld_remote_server* thisx, etch_session_control newfunc)
{
    etch_session_control oldfunc = NULL;
    etch_stub* stub = helloworld_remote_server_get_stubbase(thisx);
    if (NULL == stub || NULL == stub->impl_callbacks) return NULL;
    oldfunc = stub->impl_callbacks->_session_control;
    stub->impl_callbacks->_session_control = newfunc;
    return oldfunc;
}


/**
 * helloworld_remote_set_session_query()
 * convenience to override remote server's session_query().
 * @return the session_query function that was overridden.
 */
etch_session_query helloworld_remote_server_set_session_query(helloworld_remote_server* thisx, etch_session_query newfunc)
{
    etch_session_query oldfunc = NULL;
    etch_stub* stub = helloworld_remote_server_get_stubbase(thisx);
    if (NULL == stub || NULL == stub->impl_callbacks) return NULL;
    oldfunc = stub->impl_callbacks->_session_query;
    stub->impl_callbacks->_session_query = newfunc;
    return oldfunc;
}


/* - - - - - - - - - - - - - - - - - - - -  
 * remote procedure call implementations
 * - - - - - - - - - - - - - - - - - - - - 
 */
 
	
/* - - - - - - - - - - -  
 * helloworld.say_hello()
 * - - - - - - - - - - -  
 */
 
/**
 * helloworld_remote_begin_say_hello()
 * helloworld.say_hello async start 
 * TODO: doc generation
 */
i_mailbox* helloworld_remote_begin_server_say_hello(void* thisAsVoid, helloworld_user* to_whom)
{
    helloworld_remote_server* thisx = (helloworld_remote_server*)thisAsVoid;
    int _result = 0;
    i_mailbox* _mbox = NULL;
    etch_message* _msg = NULL;
    etch_type* _msgtype = helloworld_valufact_get_static()->_mt_helloworld_say_hello;

    do
    {   
        _msg = thisx->remote_base->new_message (thisx->remote_base, _msgtype);
        if (!_msg) break;

        _result = message_putc (_msg, helloworld_valufact_get_static()->_mf_helloworld_to_whom, (void**)&to_whom);
		if (to_whom != NULL && 0 != _result) break; 

        /* fyi msg memory is relinquished here regardless of result */ 
        _result = thisx->remote_base->begin_call(thisx->remote_base, _msg, (void**)&_mbox);
        _msg = NULL;

    } while(0);

    /* destroy any unrelinquished objects */
    etch_object_destroy(to_whom);
	to_whom = NULL;
    etch_object_destroy(_msg);
	_msg = NULL;
	
    return _mbox;
}

/**
 * helloworld_remote_end__say_hello()
 * _say_hello async end (read result from mailbox and return result)
 * @param thisx this.
 * @param mbox caller relinquishes
 * @return etch_int32* result of add, caller owns.
 */
etch_string* helloworld_remote_end_server_say_hello (void* thisAsVoid, i_mailbox* ibox)
{
    helloworld_remote_server* thisx = (helloworld_remote_server*)thisAsVoid;
    etch_string* _resobj = NULL;
    int _result = -1;
    etch_type* _restype = helloworld_valufact_get_static()->_mt_helloworld__result_say_hello;
    
    if(ibox == NULL) {
        return NULL;
    }

    thisx->remote_base->end_call(thisx->remote_base, ibox, _restype, (void**)&_resobj);
    
    _result = helloworld_remote_server_dispose_mailbox (thisx, &ibox);
    if(_result) {
     	etch_exception* excp = new_etch_exception_from_errorcode(ETCH_ERROR);
        etch_exception_set_message(excp,new_stringw(L"can not dispose mailbox."));
        return (etch_string*)excp;
    }
    
    return _resobj;
}

/**
 * helloworld_remote_say_hello
 * helloworld.say_hello remote method call.
 * instantiates a mailbox, sends perf.add message, waits for result to arrive
 * in mailbox, disposes mailbox, and returns object containing add() result.
 * @param thisx this.
 * @param x etch_int32* caller relinquishes.
 * @param y etch_int32* caller relinquishes.
 * @return etch_int32* result of add, caller owns.
 */
etch_string* helloworld_remote_server_say_hello(void* thisAsVoid, helloworld_user* to_whom)
{
    helloworld_remote_server* thisx = (helloworld_remote_server*)thisAsVoid;
    etch_string* _resultobj = NULL;

    i_mailbox* _mbox = helloworld_remote_begin_server_say_hello(thisx, to_whom);

	if(_mbox == NULL){
		etch_exception* excp = new_etch_exception_from_errorcode(ETCH_EIO);
        etch_exception_set_message(excp,new_stringw(L"can not create mailbox, connection could be down."));
        return (etch_string*)excp;
	}

    _resultobj = helloworld_remote_end_server_say_hello (thisx, _mbox);

    return _resultobj;
}
