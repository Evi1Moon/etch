"""
$Id: MailboxManager.py 712749 2008-08-18 03:26:52Z dixson3 $

# Copyright 2007-2008 Cisco Systems Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License. You may obtain a copy
# of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations
# under the License.
#
"""
from __future__ import absolute_import
from ...python.Exceptions import *
from ..msg.Message import *
from ..support.DeliveryService import *
from ..support.Mailbox import *
from ...util.core.Who import *

class MailboxManager(SessionMessage,TransportMessage):
    """
    Interface to the mailbox manager as needed by DeliveryService and Mailbox implementors
    """
    
    def transportCall(self, recipient, msg):
        """
        Sends a message which begins a call after allocating a Mailbox to receive any responses
        
        @param recipient specifies the recipient when there is the possibility of more than one
        @param msg the message which begins the call
        @return the mailbox which will receive responses to this call
        """
        raise UndefinedInterfaceMethodException
    
    def unregister(self, mb):
        """
        Removes the mailbox from the set of mailboxes receiving responses to messages.
        @param mb a mailbox as returned by 'transportCall(Who, Message)
        """
        raise UndefinedInterfaceMethodException
    
    def redeliver(self, sender, msg):
        """
        Re-delivers dead letter message from a closed mailbox
        
        @param sender
        @param msg
        """
        raise UndefinedInterfaceMethodException
    
    
        
    

    
     
     