"""
$Id: AsyncMode.py 718128 2008-11-16 22:35:35Z dixson $

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

class AsyncMode:
    NONE = "AsyncMode.NONE"
    """Synchronous Mode"""
    
    QUEUED = "AsyncMode.QUEUED"
    """The operation is queued to a thread pool for execution"""
    
    FREE = "AsyncMode.FREE"
    """The operation is executed by a newly allocated thread"""
